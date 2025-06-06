bmcv_image_storage_convert
==========================

The interface converts the data corresponding to the source image format into the format data of the target image and fills it in the device memory associated with the target image.


**Processor model support**

This interface supports BM1684/BM1684X.


**Interface form:**

    .. code-block:: c

        bm_status_t bmcv_image_storage_convert(
                    bm_handle_t handle,
                    int image_num,
                    bm_image* input_image,
                    bm_image* output_image);


**Incoming parameters description:**

* bm_handle_t handle

  Input parameter. HDC (handle of device’s capacity) obtained by calling bm_dev_request.

* int image_num

  Input parameter. The number of input/output images.

* bm_image* input

  Input parameter. Input bm_image object pointer.

* bm_image* output

  Output parameter. Output bm_image object pointer.


**Return parameters description:**

* BM_SUCCESS: success

* Other: failed


**Note**

1. The API supports the following formats:
- bm1684 supports two-two conversion of all the following formats:

+-----+------------------------+-------------------------------+
| num | image_format           | data type                     |
+=====+========================+===============================+
|  1  |                        | DATA_TYPE_EXT_FLOAT32         |
+-----+                        +-------------------------------+
|  2  | FORMAT_RGB_PLANAR      | DATA_TYPE_EXT_1N_BYTE         |
+-----+                        +-------------------------------+
|  3  |                        | DATA_TYPE_EXT_4N_BYTE         |
+-----+------------------------+-------------------------------+
|  4  |                        | DATA_TYPE_EXT_FLOAT32         |
+-----+                        +-------------------------------+
|  5  | FORMAT_BGR_PLANAR      | DATA_TYPE_EXT_1N_BYTE         |
+-----+                        +-------------------------------+
|  6  |                        | DATA_TYPE_EXT_4N_BYTE         |
+-----+------------------------+-------------------------------+
|  7  |                        | DATA_TYPE_EXT_FLOAT32         |
+-----+                        +-------------------------------+
|  8  | FORMAT_RGB_PACKED      | DATA_TYPE_EXT_1N_BYTE         |
+-----+                        +-------------------------------+
|  9  |                        | DATA_TYPE_EXT_4N_BYTE         |
+-----+------------------------+-------------------------------+
|  10 |                        | DATA_TYPE_EXT_FLOAT32         |
+-----+                        +-------------------------------+
|  11 | FORMAT_BGR_PACKED      | DATA_TYPE_EXT_1N_BYTE         |
+-----+                        +-------------------------------+
|  12 |                        | DATA_TYPE_EXT_4N_BYTE         |
+-----+------------------------+-------------------------------+
|  13 |                        | DATA_TYPE_EXT_FLOAT32         |
+-----+                        +-------------------------------+
|  14 | FORMAT_RGBP_SEPARATE   | DATA_TYPE_EXT_1N_BYTE         |
+-----+                        +-------------------------------+
|  15 |                        | DATA_TYPE_EXT_4N_BYTE         |
+-----+------------------------+-------------------------------+
|  16 |                        | DATA_TYPE_EXT_FLOAT32         |
+-----+                        +-------------------------------+
|  17 | FORMAT_BGRP_SEPARATE   | DATA_TYPE_EXT_1N_BYTE         |
+-----+                        +-------------------------------+
|  18 |                        | DATA_TYPE_EXT_4N_BYTE         |
+-----+------------------------+-------------------------------+
|  19 | FORMAT_NV12            | DATA_TYPE_EXT_1N_BYTE         |
+-----+------------------------+-------------------------------+
|  20 | FORMAT_NV21            | DATA_TYPE_EXT_1N_BYTE         |
+-----+------------------------+-------------------------------+
|  21 | FORMAT_NV16            | DATA_TYPE_EXT_1N_BYTE         |
+-----+------------------------+-------------------------------+
|  22 | FORMAT_NV61            | DATA_TYPE_EXT_1N_BYTE         |
+-----+------------------------+-------------------------------+
|  23 | FORMAT_YUV420P         | DATA_TYPE_EXT_1N_BYTE         |
+-----+------------------------+-------------------------------+
|  24 | FORMAT_YUV444P         | DATA_TYPE_EXT_1N_BYTE         |
+-----+------------------------+-------------------------------+
|  25 | FORMAT_GRAY            | DATA_TYPE_EXT_1N_BYTE         |
+-----+------------------------+-------------------------------+

If the input and output image objects are not in the above format, return failure.

- bm1684x supports the following data_type:

+-----+------------------------+-------------------------------+
| num | input data type        | output data type              |
+=====+========================+===============================+
|  1  |                        | DATA_TYPE_EXT_FLOAT32         |
+-----+                        +-------------------------------+
|  2  |                        | DATA_TYPE_EXT_1N_BYTE         |
+-----+                        +-------------------------------+
|  3  | DATA_TYPE_EXT_1N_BYTE  | DATA_TYPE_EXT_1N_BYTE_SIGNED  |
+-----+                        +-------------------------------+
|  4  |                        | DATA_TYPE_EXT_FP16            |
+-----+                        +-------------------------------+
|  5  |                        | DATA_TYPE_EXT_BF16            |
+-----+------------------------+-------------------------------+

- bm1684x supports the following color formats of input bm_image:

+-----+-------------------------------+
| num | input image_format            |
+=====+===============================+
|  1  | FORMAT_YUV420P                |
+-----+-------------------------------+
|  2  | FORMAT_YUV422P                |
+-----+-------------------------------+
|  3  | FORMAT_YUV444P                |
+-----+-------------------------------+
|  4  | FORMAT_NV12                   |
+-----+-------------------------------+
|  5  | FORMAT_NV21                   |
+-----+-------------------------------+
|  6  | FORMAT_NV16                   |
+-----+-------------------------------+
|  7  | FORMAT_NV61                   |
+-----+-------------------------------+
|  8  | FORMAT_RGB_PLANAR             |
+-----+-------------------------------+
|  9  | FORMAT_BGR_PLANAR             |
+-----+-------------------------------+
|  10 | FORMAT_RGB_PACKED             |
+-----+-------------------------------+
|  11 | FORMAT_BGR_PACKED             |
+-----+-------------------------------+
|  12 | FORMAT_RGBP_SEPARATE          |
+-----+-------------------------------+
|  13 | FORMAT_BGRP_SEPARATE          |
+-----+-------------------------------+
|  14 | FORMAT_GRAY                   |
+-----+-------------------------------+
|  15 | FORMAT_COMPRESSED             |
+-----+-------------------------------+
|  16 | FORMAT_YUV444_PACKED          |
+-----+-------------------------------+
|  17 | FORMAT_YVU444_PACKED          |
+-----+-------------------------------+
|  18 | FORMAT_YUV422_YUYV            |
+-----+-------------------------------+
|  19 | FORMAT_YUV422_YVYU            |
+-----+-------------------------------+
|  20 | FORMAT_YUV422_UYVY            |
+-----+-------------------------------+
|  21 | FORMAT_YUV422_VYUY            |
+-----+-------------------------------+


- bm1684x supports the following color formats of output bm_image:

+-----+-------------------------------+
| num | output image_format           |
+=====+===============================+
|  1  | FORMAT_YUV420P                |
+-----+-------------------------------+
|  2  | FORMAT_YUV444P                |
+-----+-------------------------------+
|  3  | FORMAT_NV12                   |
+-----+-------------------------------+
|  4  | FORMAT_NV21                   |
+-----+-------------------------------+
|  5  | FORMAT_RGB_PLANAR             |
+-----+-------------------------------+
|  6  | FORMAT_BGR_PLANAR             |
+-----+-------------------------------+
|  7  | FORMAT_RGB_PACKED             |
+-----+-------------------------------+
|  8  | FORMAT_BGR_PACKED             |
+-----+-------------------------------+
|  9  | FORMAT_RGBP_SEPARATE          |
+-----+-------------------------------+
|  10 | FORMAT_BGRP_SEPARATE          |
+-----+-------------------------------+
|  11 | FORMAT_GRAY                   |
+-----+-------------------------------+
|  12 | FORMAT_RGBYP_PLANAR           |
+-----+-------------------------------+
|  13 | FORMAT_BGRP_SEPARATE          |
+-----+-------------------------------+
|  14 | FORMAT_HSV180_PACKED          |
+-----+-------------------------------+
|  15 | FORMAT_HSV256_PACKED          |
+-----+-------------------------------+

If the input/output image object is not in the above format, a failure will be returned.

2. All input and output bm_image structures must be created in advance, or a failure will be returned.

3. All the image_format, data_type, width and height of all input bm_image objects must be the same. All the image_format, data_type, width and height of all output bm_image objects must be the same. The width and height of the input and output bm_image object must be the same, or a failure will be returned.

4. image_num indicates the number of input images. If the input image data format is DATA_TYPE_EXT_4N_BYTE, the number of input bm_image object is one, and the number of valid images in 4N is image_num. If the input image data format is not DATA_TYPE_EXT_4N_BYTE, the number of input bm_image is image_num. If the output image data format is DATA_TYPE_EXT_4N_BYTE, the number of output bm_image object is one, and the number of valid images in 4N is image_num. If the output image data format is not DATA_TYPE_EXT_4N_BYTE, the number of output bm_image is image_num.

5. image_num must be greater than or equal to 1 and less than or equal to 4, otherwise, a failure will be returned.

6. All input objects must attach device memory, otherwise, a failure will be returned.

7. If the output object does not attach device memory, the device will externally call bm_image_alloc_dev_mem to apply for internally managed device memory and fills the converted data into device memory.

8. If the input image and output image have the same format a direct success will be returned, and the original data will not be copied to the output image.

9. Currently do not support the image format conversion when image_w > 8192. A failure will be returned when image_w > 8192.


**Code example:**

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
            const char *src_name = "/path/to/src";
            const char *dst_name = "path/to/dst";

            bm_dev_request(&handle, 0);
            readBin(src_name, src_data, image_h * image_w * 3 / 2);

            bm_image_create(handle, image_h, image_w, FORMAT_NV12, DATA_TYPE_EXT_1N_BYTE, &src);
            bm_image_create(handle, image_h, image_w, FORMAT_BGR_PLANAR, DATA_TYPE_EXT_1N_BYTE, &dst);
            bm_image_copy_host_to_device(src, (void**)&src_data);
            bmcv_image_storage_convert(handle, image_n, &src, &dst);
            bm_image_copy_device_to_host(dst, (void**)&res_data);
            writeBin(dst_name, res_data, image_h * image_w * 3);

            bm_image_destroy(src);
            bm_image_destroy(dst);
            bm_dev_free(handle);
            delete[] src_data;
            delete[] res_data;
            return 0;
        }