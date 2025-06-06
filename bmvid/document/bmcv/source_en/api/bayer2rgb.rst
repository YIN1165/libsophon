bmcv_image_bayer2rgb
======================

Converts bayerBG8 or bayerRG8 format images to RGB Plannar format.


**Processor model support**

This interface only supports BM1684X.


**Interface form:**

    .. code-block:: c

        bm_status_t bmcv_image_bayer2rgb(
                    bm_handle_t handle,
                    unsigned char* convd_kernel,
                    bm_image input
                    bm_image output);


**Parameter Description:**

* bm_handle_t handle

  Input parameter. Handle of bm_handle.

* unsigned char* convd_kernel

  Input parameter. Convolutional kernel for convolutional computation.

* bm_image input

  Input parameter. The bm_image of the input image. The creation of bm_image requires an external call to bmcv_image_create. The image memory can use bm_image_alloc_dev_mem or bm_image_copy_host_to_device to create new memory, or use bmcv_image_attach to attach existing memory.

* bm_image output

  Output parameter. Output bm_image. The creation of bm_image requires an external call to bmcv_image_create. Image memory can use bm_image_alloc_dev_mem to create new memory, or use bmcv_image_attach to attach existing memory. If users do not actively allocate, it will be allocated automatically within the API.


**Return value description:**

* BM_SUCCESS: success

* Other: failed


**Format support:**

The interface currently supports the following input format:

+-----+--------------------------------+
| num | image_format                   |
+=====+================================+
| 1   | FORMAT_BAYER                   |
+-----+--------------------------------+
| 2   | FORMAT_BAYER_RG8               |
+-----+--------------------------------+

The interface currently supports the following output format:

+-----+--------------------------------+
| num | image_format                   |
+=====+================================+
| 1   | FORMAT_RGB_PLANAR              |
+-----+--------------------------------+

The interface currently supports the following data_type:

+-----+--------------------------------+
| num | data_type                      |
+=====+================================+
| 1   | DATA_TYPE_EXT_1N_BYTE          |
+-----+--------------------------------+


**Note**

1. The input format currently supports bayerBG8 or bayerRG8. In the bm_image_create step, bayerBG8 is created in the FORMAT_BAYER format, and bayerRG8 is created in the FORMAT_BAYER_RG8 format.

2. The output format is rgb plannar, and data_type is uint8 type.

3. The size range supported by this interface is 2*2 ~ 8192*8192, and the width and height of the image need to be an even number.

4. If the program calling this interface is a multi-threaded program, thread locks need to be added before creating bm_image and after destroying bm_image.


**Code example:**

    .. code-block:: c

        #include "bmcv_api_ext.h"
        #include "stdio.h"
        #include "stdlib.h"
        #include "string.h"
        #include <assert.h>

        #define KERNEL_SIZE 3 * 3 * 3 * 4 * 64
        #define CONVD_MATRIX 12 * 9
        const unsigned char convd_kernel_bg8[CONVD_MATRIX] = {1, 0, 1, 0, 0, 0, 1, 0, 1, //Rb
                                                              0, 0, 2, 0, 0, 0, 0, 0, 2, //Rg1
                                                              0, 0, 0, 0, 0, 0, 2, 0, 2, //Rg2
                                                              0, 0, 0, 0, 0, 0, 0, 0, 4, //Rr
                                                              4, 0, 0, 0, 0, 0, 0, 0, 0, //Bb
                                                              2, 0, 2, 0, 0, 0, 0, 0, 0, //Bg1
                                                              2, 0, 0, 0, 0, 0, 2, 0, 0, //Bg2
                                                              1, 0, 1, 0, 0, 0, 1, 0, 1, //Br
                                                              0, 1, 0, 1, 0, 1, 0, 1, 0, //Gb
                                                              0, 0, 0, 0, 0, 4, 0, 0, 0, //Gg1
                                                              0, 0, 0, 0, 0, 0, 0, 4, 0, //Gg2
                                                              0, 1, 0, 1, 0, 1, 0, 1, 0};//Gr

        const unsigned char convd_kernel_rg8[CONVD_MATRIX] = {4, 0, 0, 0, 0, 0, 0, 0, 0, //Rr
                                                              2, 0, 2, 0, 0, 0, 0, 0, 0, //Rg1
                                                              2, 0, 0, 0, 0, 0, 2, 0, 0, //Rg2
                                                              1, 0, 1, 0, 0, 0, 1, 0, 1, //Rb
                                                              1, 0, 1, 0, 0, 0, 1, 0, 1, //Br
                                                              0, 0, 2, 0, 0, 0, 0, 0, 2, //Bg1
                                                              0, 0, 0, 2, 0, 2, 0, 0, 0, //Bg2
                                                              0, 0, 0, 0, 0, 0, 0, 0, 4, //Bb
                                                              1, 0, 1, 0, 0, 0, 1, 0, 1, //Gr
                                                              0, 0, 0, 0, 0, 4, 0, 0, 0, //Gg1
                                                              0, 0, 0, 0, 0, 0, 0, 4, 0, //Gg2
                                                              0, 1, 0, 1, 0, 1, 0, 1, 0};//Gb

        static void readBin(const char * path, unsigned char* input_data, int size)
        {
            FILE *fp_src = fopen(path, "rb");
            if (fread((void *)input_data, 1, size, fp_src) < (unsigned int)size) {
                printf("file size is less than %d required bytes\n", size);
            };

            fclose(fp_src);
        }

        static void writeBin(const char * path, unsigned char* input_data, int size)
        {
            FILE *fp_dst = fopen(path, "wb");
            if (fwrite((void *)input_data, 1, size, fp_dst) < (unsigned int)size) {
                printf("file size is less than %d required bytes\n", size);
            };

            fclose(fp_dst);
        }

        int main()
        {
            int width = 1920;
            int height = 1080;
            int dev_id = 0;
            unsigned char* input = (unsigned char*)malloc(width * height);
            unsigned char* output_tpu = (unsigned char*)malloc(width * height * 3);
            unsigned char kernel_data[KERNEL_SIZE] = {0};
            bm_handle_t handle;
            bm_image input_img;
            bm_image output_img;
            const char *src_name = "path/to/src";
            const char *dst_name = "path/to/dst";
            int src_type = 0;
            unsigned char* out_ptr[3] = {output_tpu, output_tpu + height * width, output_tpu + 2 * height * width};

            bm_dev_request(&handle, dev_id);
            bm_image_create(handle, height, width, FORMAT_GRAY, DATA_TYPE_EXT_1N_BYTE, &input_img, NULL);
            bm_image_create(handle, height, width, FORMAT_RGB_PLANAR, DATA_TYPE_EXT_1N_BYTE, &output_img, NULL);
            bm_image_alloc_dev_mem(input_img, BMCV_HEAP_ANY);
            bm_image_alloc_dev_mem(output_img, BMCV_HEAP_ANY);


            for (int i = 0;i < height;i++) {
                for (int j = 0;j < width;j++) {
                    input[i * width + j] = rand() % 255;
                }
            }

            for (int i = 0;i < 12;i++) {
                for (int j = 0;j < 9;j++) {
                    if (src_type == 0) {
                        kernel_data[i * 9 * 64 + 64 * j] = convd_kernel_bg8[i * 9 + j];
                    } else {
                        kernel_data[i * 9 * 64 + 64 * j] = convd_kernel_rg8[i * 9 + j];
                    }
                }
            }

            readBin(src_name, input, height * width);
            bm_image_copy_host_to_device(input_img, (void**)&input);
            bmcv_image_bayer2rgb(handle, kernel_data, input_img, output_img);
            bm_image_copy_device_to_host(output_img, (void **)out_ptr);
            writeBin(dst_name, output_tpu, width * height * 3);

            bm_image_destroy(input_img);
            bm_image_destroy(output_img);
            free(input);
            free(output_tpu);
            bm_dev_free(handle);
            return 0;
        }