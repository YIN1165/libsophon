bmcv_image_mosaic
=========================

该接口用于在图像上打一个或多个马赛克。


**处理器型号支持：**

该接口仅支持BM1684X。


**接口形式：**
    .. code-block:: c

        bm_status_t bmcv_image_mosaic(
                    bm_handle_t handle,
                    int mosaic_num,
                    bm_image input,
                    bmcv_rect_t* mosaic_rect,
                    int is_expand);


**传入参数说明:**

* bm_handle_t handle

  输入参数。设备环境句柄，通过调用 bm_dev_request 获取。

* int mosaic_num

  输入参数。马赛克数量，指 mosaic_rect 指针中所包含的 bmcv_rect_t 对象个数。

* bm_image input

  输入参数。需要打马赛克的 bm_image 对象。

* bmcv_rect_t\* mosaic_rect

  输入参数。马赛克对象指针，包含每个马赛克起始点和宽高。具体内容参考下面的数据类型说明。

* int is_expand

  输入参数。是否扩列。值为0时表示不扩列, 值为1时表示在原马赛克周围扩列一个宏块(8个像素)。


**返回值说明:**

* BM_SUCCESS: 成功

* 其他: 失败


**数据类型说明：**


    .. code-block:: c

        typedef struct bmcv_rect {
            int start_x;
            int start_y;
            int crop_w;
            int crop_h;
        } bmcv_rect_t;

* start_x 描述了马赛克在原图中所在的起始横坐标。自左而右从 0 开始，取值范围 [0, width)。

* start_y 描述了马赛克在原图中所在的起始纵坐标。自上而下从 0 开始，取值范围 [0, height)。

* crop_w 描述的马赛克的宽度。

* crop_h 描述的马赛克的高度。


**注意事项:**

1. 输入和输出的数据类型必须为：

+-----+-------------------------------+
| num | data_type                     |
+=====+===============================+
|  1  | DATA_TYPE_EXT_1N_BYTE         |
+-----+-------------------------------+

- 输入的色彩格式可支持：

+-----+-------------------------------+
| num | image_format                  |
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

如果不满足输入输出格式要求，则返回失败。

2. 输入输出所有 bm_image 结构必须提前创建，否则返回失败。

3. 如果马赛克宽高非8对齐，则会自动向上8对齐，若在边缘区域，则8对齐时会往非边缘方向延展。

4. 如果马赛克区域超出原图宽高，超出部分会自动贴到原图边缘。

5. 仅支持8x8以上的马赛克尺寸。


**代码示例：**

    .. code-block:: c

        #include <iostream>
        #include <vector>
        #include "bmcv_api_ext.h"
        #include <stdio.h>
        #include <stdlib.h>
        #include <sstream>
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
            bm_handle_t handle = NULL;
            int width = 1024;
            int height = 1024;
            int dev_id = 0;
            int mosaic_num = 1;
            bm_image_format_ext src_fmt = FORMAT_GRAY;
            bm_image src;
            bmcv_rect_t* rect = new bmcv_rect_t [mosaic_num];
            unsigned char* data_ptr = new unsigned char[width * height];
            unsigned int is_expand = 1;
            const char *src_name = "path/to/src";
            const char *dst_name = "path/to/dst";

            for(int i = 0; i < mosaic_num; i++){
                rect[i].start_x = 8 + i * 8;
                rect[i].start_y = 8 + i * 8;
                rect[i].crop_w = 8 + i * 8;
                rect[i].crop_h = 8 + i * 8;
            }

            readBin(src_name, data_ptr, width * height);
            bm_dev_request(&handle, dev_id);
            bm_image_create(handle, height, width, src_fmt, DATA_TYPE_EXT_1N_BYTE, &src);
            bm_image_alloc_dev_mem(src);
            bm_image_copy_host_to_device(src, (void**)&data_ptr);
            bmcv_image_mosaic(handle, mosaic_num, src, rect, is_expand);
            bm_image_copy_device_to_host(src, (void**)&data_ptr);
            writeBin(dst_name, data_ptr,  width * height);

            bm_image_destroy(src);
            bm_dev_free(handle);
            delete[] rect;
            delete[] data_ptr;
            return 0;
        }