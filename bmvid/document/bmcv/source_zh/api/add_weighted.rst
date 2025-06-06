bmcv_image_add_weighted
=======================

实现两张相同大小图像的加权融合，具体如下：

.. math::
    \begin{array}{c}
    output = alpha * input1 + beta * input2 + gamma
    \end{array}


**处理器型号支持：**

该接口支持BM1684/BM1684X。


**接口形式：**

    .. code-block:: c

        bm_status_t bmcv_image_add_weighted(
                    bm_handle_t handle,
                    bm_image input1,
                    float alpha,
                    bm_image input2,
                    float beta,
                    float gamma,
                    bm_image output);


**参数说明：**

* bm_handle_t handle

  输入参数。 bm_handle 句柄。

* bm_image input1

  输入参数。输入第一张图像的 bm_image，bm_image 需要外部调用 bmcv_image_create 创建。image 内存可以使用 bm_image_alloc_dev_mem 或者 bm_image_copy_host_to_device 来开辟新的内存，或者使用 bmcv_image_attach 来 attach 已有的内存。

* float alpha

  第一张图像的权重。

* bm_image input2

  输入参数。输入第二张图像的 bm_image，bm_image 需要外部调用 bmcv_image_create 创建。image 内存可以使用 bm_image_alloc_dev_mem 或者 bm_image_copy_host_to_device 来开辟新的内存，或者使用 bmcv_image_attach 来 attach 已有的内存。

* float beta

  第二张图像的权重。

* float gamma

  融合之后的偏移量。

* bm_image output

  输出参数。输出 bm_image，bm_image 需要外部调用 bmcv_image_create 创建。image 内存可以通过 bm_image_alloc_dev_mem 来开辟新的内存，或者使用 bmcv_image_attach 来 attach 已有的内存。如果不主动分配将在 api 内部进行自行分配。


**返回值说明：**

* BM_SUCCESS: 成功

* 其他: 失败


**格式支持：**

该接口目前支持以下 image_format:

+-----+------------------------+
| num | image_format           |
+=====+========================+
| 1   | FORMAT_BGR_PACKED      |
+-----+------------------------+
| 2   | FORMAT_BGR_PLANAR      |
+-----+------------------------+
| 3   | FORMAT_RGB_PACKED      |
+-----+------------------------+
| 4   | FORMAT_RGB_PLANAR      |
+-----+------------------------+
| 5   | FORMAT_RGBP_SEPARATE   |
+-----+------------------------+
| 6   | FORMAT_BGRP_SEPARATE   |
+-----+------------------------+
| 7   | FORMAT_GRAY            |
+-----+------------------------+
| 8   | FORMAT_YUV420P         |
+-----+------------------------+
| 9   | FORMAT_YUV422P         |
+-----+------------------------+
| 10  | FORMAT_YUV444P         |
+-----+------------------------+
| 11  | FORMAT_NV12            |
+-----+------------------------+
| 12  | FORMAT_NV21            |
+-----+------------------------+
| 13  | FORMAT_NV16            |
+-----+------------------------+
| 14  | FORMAT_NV61            |
+-----+------------------------+
| 15  | FORMAT_NV24            |
+-----+------------------------+

目前支持以下 data_type:

+-----+--------------------------------+
| num | data_type                      |
+=====+================================+
| 1   | DATA_TYPE_EXT_1N_BYTE          |
+-----+--------------------------------+
| 2   | DATA_TYPE_EXT_FLOAT32          |
+-----+--------------------------------+


**注意事项：**

1、在调用该接口之前必须确保输入的 image 内存已经申请。

2、input output 的 data_type，image_format必须相同。


**代码示例：**

    .. code-block:: c

        #include "bmcv_api_ext.h"
        #include <math.h>
        #include <stdint.h>
        #include <stdio.h>
        #include <stdlib.h>
        #include <string.h>

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
            int channel = 3;
            int width = 1920;
            int height = 1080;
            int dev_id = 0;
            bm_handle_t handle;
            bm_image input1, input2, output;
            unsigned char* input1_data = (unsigned char*)malloc(width * height * channel);
            unsigned char* input2_data = (unsigned char*)malloc(width * height * channel);
            unsigned char* output_tpu = (unsigned char*)malloc(width * height * channel);
            const char *src1_name = "path/to/src1";
            const char *src2_name = "path/to/src2";
            const char *dst_name = "path/to/dst";
            unsigned char* in1_ptr[3] = {input1_data, input1_data + height * width, input1_data + 2 * height * width};
            unsigned char* in2_ptr[3] = {input2_data, input2_data + height * width, input2_data + 2 * height * width};
            unsigned char* out_ptr[3] = {output_tpu, output_tpu + height * width, output_tpu + 2 * height * width};
            int img_size = width * height * channel;

            readBin(src1_name, input1_data, img_size);
            readBin(src2_name, input2_data, img_size);

            bm_dev_request(&handle, dev_id);
            bm_image_create(handle, height, width, FORMAT_RGB_PLANAR, DATA_TYPE_EXT_1N_BYTE, &input1);
            bm_image_alloc_dev_mem(input1);
            bm_image_copy_host_to_device(input1, (void **)&in1_ptr);
            bm_image_create(handle, height, width, FORMAT_RGB_PLANAR, DATA_TYPE_EXT_1N_BYTE, &input2);
            bm_image_alloc_dev_mem(input2);
            bm_image_copy_host_to_device(input2, (void **)&in2_ptr);
            bm_image_create(handle, height, width, FORMAT_RGB_PLANAR, DATA_TYPE_EXT_1N_BYTE, &output);
            bm_image_alloc_dev_mem(output);
            bmcv_image_add_weighted(handle, input1, 0.5, input2, 0.5, 0, output);
            bm_image_copy_device_to_host(output, (void **)&out_ptr);
            writeBin(dst_name, output_tpu, img_size);

            bm_image_destroy(input1);
            bm_image_destroy(input2);
            bm_image_destroy(output);
            bm_dev_free(handle);
            free(input1_data);
            free(input2_data);
            free(output_tpu);
            return 0;
        }