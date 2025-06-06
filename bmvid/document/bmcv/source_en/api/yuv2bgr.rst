bmcv_image_yuv2bgr_ext
========================

This interface convert YUV format to RGB format.


**Processor model support**

This interface supports BM1684/BM1684X.


**Interface form:**

    .. code-block:: c

        bm_status_t bmcv_image_yuv2bgr_ext(
                    bm_handle_t handle,
                    int image_num,
                    bm_image* input,
                    bm_image* output);


**Description of incoming parameters:**

* bm_handle_t handle

  Input parameter. HDC (handle of device’s capacity) obtained by calling bm_dev_request.

* int image_num

  Input parameter. The number of input/output images.

* bm_image* input

  Input parameter. The input bm_image object pointer.

* bm_image* output

  Output parameter. The output bm_image object pointer.


**Description of returning parameters:**

* BM_SUCCESS: success

* Other: failed


**Code example**

    .. code-block:: c

        #include <iostream>
        #include <vector>
        #include "bmcv_api_ext.h"
        #include "stdio.h"
        #include "stdlib.h"
        #include "string.h"
        #include <memory>

        static void readBin(const char* path, unsigned char* input_data, int size)
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
            bm_handle_t handle;
            int image_n = 1;
            int image_h = 1080;
            int image_w = 1920;
            bm_image src, dst;
            unsigned char* src_data = new unsigned char[image_h * image_w * 3 / 2];
            unsigned char* res_data = new unsigned char[image_h * image_w * 3];
            const char *filename_src = "path/to/src";
            const char *filename_dst = "path/to/dst";

            bm_dev_request(&handle, 0);
            readBin(filename_src, src_data, image_h * image_w * 3 / 2);
            bm_image_create(handle, image_h, image_w, FORMAT_NV12, DATA_TYPE_EXT_1N_BYTE, &src);
            bm_image_create(handle, image_h, image_w, FORMAT_BGR_PLANAR, DATA_TYPE_EXT_1N_BYTE, &dst);
            memset(src_data, 148, image_h * image_w * 3 / 2);
            bm_image_copy_host_to_device(src, (void**)&src_data);
            bmcv_image_yuv2bgr_ext(handle, image_n, &src, &dst);
            bm_image_copy_device_to_host(dst, (void**)&res_data);
            writeBin(filename_dst, res_data, image_h * image_w * 3);

            bm_image_destroy(src);
            bm_image_destroy(dst);
            bm_dev_free(handle);
            delete[] src_data;
            delete[] res_data;
            return 0;
        }


**Note:**

1. This API inputs image objects in NV12/NV21/NV16/NV61/YUV420P formats, and fills the converted RGB data results into the device memory associated with the output image object

2. The API only supports :

-  The API supports the following image formats of input bm_image:

+-----+-------------------------------+
| num | input image_format            |
+=====+===============================+
|  1  | FORMAT_NV12                   |
+-----+-------------------------------+
|  2  | FORMAT_NV21                   |
+-----+-------------------------------+
|  3  | FORMAT_NV16                   |
+-----+-------------------------------+
|  4  | FORMAT_NV61                   |
+-----+-------------------------------+
|  5  | FORMAT_YUV420P                |
+-----+-------------------------------+
|  6  | FORMAT_YUV422P                |
+-----+-------------------------------+

-  The API supports the following image formats of output bm_image:

+-----+-------------------------------+
| num | output image_format           |
+=====+===============================+
|  1  | FORMAT_RGB_PLANAR             |
+-----+-------------------------------+
|  2  | FORMAT_BGR_PLANAR             |
+-----+-------------------------------+

-  bm1684 supports the following data formats:

+-----+------------------------+-------------------------------+
| num | input data type        | output data type              |
+=====+========================+===============================+
|  1  |                        | DATA_TYPE_EXT_FLOAT32         |
+-----+                        +-------------------------------+
|  2  | DATA_TYPE_EXT_1N_BYTE  | DATA_TYPE_EXT_1N_BYTE         |
+-----+                        +-------------------------------+
|  3  |                        | DATA_TYPE_EXT_4N_BYTE         |
+-----+------------------------+-------------------------------+

-  bm1684x supports the following data formats

+-----+------------------------+-------------------------------+
| num | input data type        | output data type              |
+=====+========================+===============================+
|  1  | DATA_TYPE_EXT_1N_BYTE  | DATA_TYPE_EXT_FLOAT32         |
+-----+                        +-------------------------------+
|  2  |                        | DATA_TYPE_EXT_1N_BYTE         |
+-----+------------------------+-------------------------------+

It will return fail if the required input/output formats are not met.

3. It will return fail if all input and output bm_image structures not created in advance.

4. The image_format, data_type, width and height of all input bm_image objects must be the same. The image_format, data_type, width and height of all output bm_image objects must be the same. The width and height of all input/output bm_image objects must be the same. Otherwise, a failure will be returned.

5. image_num indicates the number of input objects. If the data format of output bm_image is DATA_TYPE_EXT_4N_BYTE, only output one bm_image 4N object. On the contrary, the number of output objects is image_num.

6. image_num must be greater than or equal to 1 and less than or equal to 4, otherwise, a failure will be returned.

7. All input objects must attach device memory, otherwise, a failure will be returned.

8. If the output object does not attach device memory, it will internally call bm_image_alloc_dev_mem to apply for internally managed device memory and fills the converted RGB data into device memory.