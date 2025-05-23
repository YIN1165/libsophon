bmcv_image_resize
=================

该接口用于实现图像尺寸的变化,如放大、缩小、抠图等功能。


**处理器型号支持：**

该接口支持BM1684/BM1684X。


**接口形式:**

    .. code-block:: c

        bm_status_t bmcv_image_resize(
                bm_handle_t handle,
                int input_num,
                bmcv_resize_image resize_attr[4],
                bm_image* input,
                bm_image* output);


**参数说明:**

* bm_handle_t handle

  输入参数。bm_handle 句柄。

* int input_num

  输入参数。输入图片数，最多支持 4 ，如果 input_num > 1, 那么多个输入图像必须是连续存储的（可以使用 bm_image_alloc_contiguous_mem 给多张图申请连续空间）。

* bmcv_resize_image resize_attr [4]

  输入参数。每张图片对应的 resize 参数,最多支持 4 张图片。

* bm_image\* input

  输入参数。输入 bm_image。每个 bm_image 需要外部调用 bmcv_image _create 创建。image 内存可以使用 bm_image_alloc_dev_mem 或者 bm_image_copy_host_to_device 来开辟新的内存，或者使用 bmcv_image_attach 来 attach 已有的内存。

* bm_image\* output

  输出参数。输出 bm_image。每个 bm_image 需要外部调用 bmcv_image_create 创建，image 内存可以通过 bm_image_alloc_dev_mem 来开辟新的内存，或者使用 bmcv_image_attach 来 attach 已有的内存，如果不主动分配将在 api 内部进行自行分配。


**返回值说明:**

* BM_SUCCESS: 成功

* 其他: 失败


**数据类型说明:**

    .. code-block:: c

        typedef struct bmcv_resize_s{
                int start_x;
                int start_y;
                int in_width;
                int in_height;
                int out_width;
                int out_height;
        }bmcv_resize_t;

        typedef struct bmcv_resize_image_s{
                bmcv_resize_t *resize_img_attr;
                int roi_num;
                unsigned char stretch_fit;
                unsigned char padding_b;
                unsigned char padding_g;
                unsigned char padding_r;
                unsigned int interpolation;
        }bmcv_resize_image;

* bmcv_resize_image 描述了一张图中 resize 配置信息。

* roi_num 描述了一副图中需要进行 resize 的子图总个数。

* stretch_fit 表示是否按照原图比例对图片进行缩放，1 表示无需按照原图比例进行缩放，0 表示按照原图比例进行缩放，当采用这种方式的时候，结果图片中进行缩放的地方将会被填充成特定值。

* padding_b 表示当 stretch_fit 设成 0 的情况下，b 通道上被填充的值。

* padding_r 表示当 stretch_fit 设成 0 的情况下，r 通道上被填充的值。

* padding_g 表示当 stretch_fit 设成 0 的情况下，g 通道上被填充的值。

* interpolation表示缩图所使用的算法。BMCV_INTER_NEAREST表示最近邻算法，BMCV_INTER_LINEAR表示线性插值算法, BMCV_INTER_BICUBIC 表示双三次插值算法。

  bm1684 支持 BMCV_INTER_NEAREST，BMCV_INTER_LINEAR，BMCV_INTER_BICUBIC。

  bm1684x支持BMCV_INTER_NEAREST， BMCV_INTER_LINEAR。

* start_x 描述了 resize 起始横坐标(相对于原图)，常用于抠图功能。

* start_y 描述了 resize 起始纵坐标(相对于原图)，常用于抠图功能。

* in_width 描述了crop图像的宽。

* in_height 描述了crop图像的高。

* out_width 描述了输出图像的宽。

* out_height 描述了输出图像的高。


**代码示例:**

    .. code-block:: c

        #include <memory>
        #include <assert.h>
        #include <iostream>
        #include <set>
        #include <stdint.h>
        #include <stdio.h>
        #include <string>
        #include <vector>
        #include <cmath>
        #include <cstring>
        #include "bmcv_api.h"
        #include "bmcv_api_ext.h"
        #include "test_misc.h"

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
            int image_num = 4;
            int crop_w = 711, crop_h = 400, resize_w = 711, resize_h = 400;
            int image_w = 1920, image_h = 1080;
            int img_size_i = image_w * image_h * 3;
            int img_size_o = resize_w * resize_h * 3;
            unsigned char* img_data = new unsigned char[img_size_i * image_num];
            unsigned char* res_data = new unsigned char[img_size_o * image_num];
            bmcv_resize_image resize_attr[image_num];
            bmcv_resize_t resize_img_attr[image_num];
            bm_image input[image_num];
            bm_image output[image_num];
            bm_handle_t handle;
            const char *src_names[4] = {"path/to/src0", "path/to/src1", "path/to/src2", "path/to/src3"};
            const char *dst_names[4] = {"path/to/dst0", "path/to/dst1", "path/to/dst2", "path/to/dst3"};

            bm_dev_request(&handle, 0);

            for(int i = 0; i < image_num; ++i) {
                readBin(src_names[i], img_data + i * img_size_i, img_size_i);
            }

            memset(res_data, 0, img_size_o * image_num);

            for (int img_idx = 0; img_idx < image_num; img_idx++) {
                resize_img_attr[img_idx].start_x = 0;
                resize_img_attr[img_idx].start_y = 0;
                resize_img_attr[img_idx].in_width = crop_w;
                resize_img_attr[img_idx].in_height = crop_h;
                resize_img_attr[img_idx].out_width = resize_w;
                resize_img_attr[img_idx].out_height = resize_h;
            }

            for (int img_idx = 0; img_idx < image_num; img_idx++) {
            resize_attr[img_idx].resize_img_attr = &resize_img_attr[img_idx];
            resize_attr[img_idx].roi_num = 1;
            resize_attr[img_idx].stretch_fit = 1;
            resize_attr[img_idx].interpolation = BMCV_INTER_NEAREST;
            }

            for (int img_idx = 0; img_idx < image_num; img_idx++) {
                bm_image_create(handle, image_h, image_w, FORMAT_BGR_PLANAR, DATA_TYPE_EXT_1N_BYTE, &input[img_idx]);
            }
            bm_image_alloc_contiguous_mem(image_num, input, 1);
            for (int img_idx = 0; img_idx < image_num; img_idx++) {
                unsigned char * input_img_data = img_data + img_size_i * img_idx;
                bm_image_copy_host_to_device(input[img_idx], (void **)&input_img_data);
            }
            for (int img_idx = 0; img_idx < image_num; img_idx++) {
                bm_image_create(handle, resize_h, resize_w, FORMAT_BGR_PLANAR, DATA_TYPE_EXT_1N_BYTE, &output[img_idx]);
            }
            bm_image_alloc_contiguous_mem(image_num, output, 1);
            bmcv_image_resize(handle, image_num, resize_attr, input, output);
            for (int img_idx = 0; img_idx < image_num; img_idx++) {
                unsigned char *res_img_data = res_data + img_size_o * img_idx;
                bm_image_copy_device_to_host(output[img_idx], (void **)&res_img_data);
                for(int i = 0; i < image_num; ++i) {
                    writeBin(dst_names[i], res_img_data, img_size_o);
                }
            }

            bm_image_free_contiguous_mem(image_num, input);
            bm_image_free_contiguous_mem(image_num, output);
            for(int i = 0; i < image_num; i++) {
                bm_image_destroy(input[i]);
                bm_image_destroy(output[i]);
            }
            delete[] img_data;
            delete[] res_data;
            bm_dev_free(handle);
            return 0;
        }


**格式支持:**

1. resize 支持下列 image_format 的转化：

+-----+------------------------------------------------+
| 1   | FORMAT_BGR_PLANAR  ——>    FORMAT_BGR_PLANAR    |
+-----+------------------------------------------------+
| 2   | FORMAT_RGB_PLANAR  ——>    FORMAT_RGB_PLANAR    |
+-----+------------------------------------------------+
| 3   | FORMAT_BGR_PACKED  ——>    FORMAT_BGR_PACKED    |
+-----+------------------------------------------------+
| 4   | FORMAT_RGB_PACKED  ——>    FORMAT_RGB_PACKED    |
+-----+------------------------------------------------+
| 5   | FORMAT_BGR_PACKED  ——>    FORMAT_BGR_PLANAR    |
+-----+------------------------------------------------+
| 6   | FORMAT_RGB_PACKED  ——>    FORMAT_RGB_PLANAR    |
+-----+------------------------------------------------+

1. resize 支持下列情形data type之间的转换：

bm1684支持：

  - 1 vs 1 ： 1幅图像 resize (crop) 一幅图像的情形
  - 1 vs N ： 1幅图像 resize (crop) 多幅图像的情形


+-----+----------------------------------------------------+--------+
| 1   | DATA_TYPE_EXT_1N_BYTE ——> DATA_TYPE_EXT_1N_BYTE    | 1 vs 1 |
+-----+----------------------------------------------------+--------+
| 2   | DATA_TYPE_EXT_FLOAT32 ——> DATA_TYPE_EXT_FLOAT32    | 1 vs 1 |
+-----+----------------------------------------------------+--------+
| 3   | DATA_TYPE_EXT_4N_BYTE ——> DATA_TYPE_EXT_4N_BYTE    | 1 vs 1 |
+-----+----------------------------------------------------+--------+
| 4   | DATA_TYPE_EXT_4N_BYTE ——> DATA_TYPE_EXT_1N_BYTE    | 1 vs 1 |
+-----+----------------------------------------------------+--------+
| 5   | DATA_TYPE_EXT_1N_BYTE ——> DATA_TYPE_EXT_1N_BYTE    | 1 vs N |
+-----+----------------------------------------------------+--------+
| 6   | DATA_TYPE_EXT_FLOAT32 ——> DATA_TYPE_EXT_FLOAT32    | 1 vs N |
+-----+----------------------------------------------------+--------+
| 7   | DATA_TYPE_EXT_4N_BYTE ——> DATA_TYPE_EXT_1N_BYTE    | 1 vs N |
+-----+----------------------------------------------------+--------+


bm1684x支持：

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

**注意事项:**

1. 在调用 bmcv_image_resize()之前必须确保输入的 image 内存已经申请。

2. bm1684支持最大尺寸为2048*2048，最小尺寸为16*16，最大缩放比为32。

3. bm1684x支持最大尺寸为8192*8192，最小尺寸为8*8，最大缩放比为128。