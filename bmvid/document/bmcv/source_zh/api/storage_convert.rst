bmcv_image_storage_convert
==========================

该接口将源图像格式的对应的数据转换为目的图像的格式数据，并填充在目的图像关联的 device memory 中。


**处理器型号支持：**

该接口支持BM1684/BM1684X。


**接口形式:**

    .. code-block:: c

        bm_status_t bmcv_image_storage_convert(
                    bm_handle_t handle,
                    int image_num,
                    bm_image* input_image,
                    bm_image* output_image);


**传入参数说明:**

* bm_handle_t handle

  输入参数。设备环境句柄，通过调用 bm_dev_request 获取。

* int image_num

  输入参数。输入/输出 image 数量。

* bm_image* input

  输入参数。输入 bm_image 对象指针。

* bm_image* output

  输出参数。输出 bm_image 对象指针。


**返回值说明:**

* BM_SUCCESS: 成功

* 其他: 失败


**注意事项**

1. bm1684 下该 API 支持以下格式的两两相互转换：

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

如果输入输出 image 对象不在以上格式中，则返回失败。

bm1684x时，该API，

- 支持数据类型为：

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

- 输入支持色彩格式为：

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


- 输出支持色彩格式为：

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

2. 输入输出所有 bm_image 结构必须提前创建，否则返回失败。

3. 所有输入 bm_image 对象的 image_format，data_type，width，height 必须相等，所有输出 bm_image 对象的 image_format，data_type，width，height 必须相等，所有输入输出 bm_image 对象的 width，height 必须相等，否则返回失败。

4. image_num 表示输入图像个数，如果输入图像数据格式为 DATA_TYPE_EXT_4N_BYTE，则输入 bm_image 对象为 1 个，在 4N 中有 image_num 个有效图片。如果输入图像数据格式不是 DATA_TYPE_EXT_4N_BYTE，则输入 image_num 个 bm_image 对象。如果输出 bm_image 数据格式为 DATA_TYPE_EXT_4N_BYTE，则输出 1 个 bm_image 4N 对象，对象中有 bm_image 个有效图片。反之如果输出图像数据格式不是 DATA_TYPE_EXT_4N_BYTE，则输出 image_num 个对象。

5. image_num 必须大于等于 1，小于等于 4，否则返回失败。

6. 所有输入对象必须 attach device memory，否则返回失败。

7. 如果输出对象未 attach device memory，则会内部调用 bm_image_alloc_dev_mem 申请内部管理的 device memory，并将转化后的数据填充到 device memory 中。

8. 如果输入图像和输出图像格式相同，则直接返回成功，且不会将原数据拷贝到输出图像中。

9. 暂不支持 image_w > 8192 时的图像格式转换，如果 image_w > 8192 则返回失败。


**代码示例:**

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