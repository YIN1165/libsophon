bmcv_image_vpp_basic
=========================

There is a special video post-processing module VPP on BM1684 and BM1684X. Under certain conditions, it can do the functions of clip, color-space-convert, resize and padding at one time, faster than Tensor Computing Processor.
The API can combine  crop, color-space-convert, resize, padding and any number of functions for multiple images.


**Interface form::**

    .. code-block:: c

        bm_status_t bmcv_image_vpp_basic(
                    bm_handle_t handle,
                    int in_img_num,
                    bm_image* input,
                    bm_image* output,
                    int* crop_num_vec = NULL,
                    bmcv_rect_t* crop_rect = NULL,
                    bmcv_padding_atrr_t* padding_attr = NULL,
                    bmcv_resize_algorithm algorithm = BMCV_INTER_LINEAR,
                    csc_type_t csc_type = CSC_MAX_ENUM,
                    csc_matrix_t* matrix = NULL);


**Processor model support**

This interface supports BM1684/BM1684X.


**Description of incoming parameters:**

* bm_handle_t handle

  Input parameter. HDC (handle of device’s capacity) obtained by calling bm_dev_request.

* int in_img_num

  Input parameter. The number of input bm_image.

* bm_image* input

  Input parameter. Input bm_image object pointer whose length to the space is decided by in_img_num.

* bm_image* output

  Output parameter. Output bm_image image object pointer whose length to the space is jointly decided by in_img_num and crop_num_vec, that is, the sum of the number of crops of all input images.

* int* crop_num_vec = NULL

  Input parameter. The pointer points to the number of crops for each input image, and the length of the pointing space is decided by in_img_num. NULL can be filled in if the crop function is not used.

* bmcv_rect_t * crop_rect = NULL

  Input parameter. The specific format is defined as follows:

    .. code-block:: c

        typedef struct bmcv_rect {
            int start_x;
            int start_y;
            int crop_w;
            int crop_h;
        } bmcv_rect_t;

  The parameters of the crop on the input image corresponding to each output bm_image object, including the X coordinate of the starting point, the Y coordinate of the starting point, the width of the crop image and the height of the crop image. The top left vertex of the image is used as the coordinate origin. If you do not use the crop function, you can fill in NULL.

* bmcv_padding_atrr_t*  padding_attr = NULL

  Input parameter. The location information of the target thumbnails of all crops in the dst image and the pixel values of each channel to be padding. If you do not use the padding function, you can set the function to NULL.

    .. code-block:: c

        typedef struct bmcv_padding_atrr_s {
            unsigned int  dst_crop_stx;
            unsigned int  dst_crop_sty;
            unsigned int  dst_crop_w;
            unsigned int  dst_crop_h;
            unsigned char padding_r;
            unsigned char padding_g;
            unsigned char padding_b;
            int           if_memset;
        } bmcv_padding_atrr_t;

    1. Offset information of the top left corner vertex of the target thumbnail relative to the dst image origin (top left corner): dst_crop_stx and dst_crop_sty;
    2. The width and height of the target thumbnail after resize: dst_crop_w and dst_crop_h;
    3. If dst image is in RGB format, the required pixel value information of each channel: padding_r, padding_g, padding_b. When if_memset=1, it is valid. If it is a GRAY image, you can set all three values to the same value;
    4. if_memset indicates whether to memset dst image according to the padding value of each channel within the API. Only images in RGB and GRAY formats are supported. If it is set to 0, users need to call the API in bmlib according to the pixel value information of padding before calling the API to directly perform memset operation on device memory. If users are not concerned about the value of padding, it can be set to 0 to ignore this step.

* bmcv_resize_algorithm algorithm = BMCV_INTER_LINEAR

  Input parameter. Resize algorithm selection, including BMCV_INTER_NEAREST, BMCV_INTER_LINEAR and BMCV_INTER_BICUBIC, which is the bilinear difference by default.

  - bm1684 supports : BMCV_INTER_NEAREST,

    BMCV_INTER_LINEAR, BMCV_INTER_BICUBIC.

  - bm1684x supports:

    BMCV_INTER_NEAREST, BMCV_INTER_LINEAR.

* csc_type_t csc_type = CSC_MAX_ENUM

  Input parameters. color space convert Parameter type selection, fill CSC_MAX_ENUM then use the default value. The default is CSC_YCbCr2RGB_BT601 or CSC_RGB2YCbCr_BT601. The supported types include:

  +----------------------------+
  | CSC_YCbCr2RGB_BT601        |
  +----------------------------+
  | CSC_YPbPr2RGB_BT601        |
  +----------------------------+
  | CSC_RGB2YCbCr_BT601        |
  +----------------------------+
  | CSC_YCbCr2RGB_BT709        |
  +----------------------------+
  | CSC_RGB2YCbCr_BT709        |
  +----------------------------+
  | CSC_RGB2YPbPr_BT601        |
  +----------------------------+
  | CSC_YPbPr2RGB_BT709        |
  +----------------------------+
  | CSC_RGB2YPbPr_BT709        |
  +----------------------------+
  | CSC_USER_DEFINED_MATRIX    |
  +----------------------------+
  | CSC_MAX_ENUM               |
  +----------------------------+

* csc_matrix_t* matrix = NULL

Input parameter for the selection of color space convert parameter type. Fill in CSC_MAX_ENUM to use the default value, which is by default CSC_YCbCr2RGB_BT601 or CSC_RGB2YCbCr_BT601. The supported types include:

    .. code-block:: c

          typedef struct {
              int csc_coe00;
              int csc_coe01;
              int csc_coe02;
              int csc_add0;
              int csc_coe10;
              int csc_coe11;
              int csc_coe12;
              int csc_add1;
              int csc_coe20;
              int csc_coe21;
              int csc_coe22;
              int csc_add2;
          } __attribute__((packed)) csc_matrix_t;


**Return value description:**

* BM_SUCCESS: success

* Other: failed


**Note:**

- bm1684x supports the following:

1. bm1684x supports the following data_type:

+-----+------------------------+-------------------------------+
| num | input data_type        | output data_type              |
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


2. bm1684x supports the following color formats of input bm_image:

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


3. bm1684x supports the following color formats of output bm_image:

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

4. bm1684x vpp does not support FORMAT_COMPRESSED to FORMAT_HSV180_PACKED or FORMAT_HSV256_PACKED

5. The zoom ratio of the image ((crop.width / output.width) and (crop.height / output.height)) is limited to 1 / 128 ~ 128.

6. The width and height (src.width, src.height, dst.width, dst.height) of input and output are limited to 8 ~ 8192.

7. The input must be associated with device memory, otherwise, a failure will be returned.

8. The usage of FORMAT_COMPRESSED format is described in the bm1684 section.


- bm1684 supports the following:

1. The format and some requirements that the API needs to meet are shown in the following table:

+------------------+---------------------+-----------------+
| src format       | dst format          | Other Limitation|
+==================+=====================+=================+
|                  | RGB_PACKED          |  Condition 1    |
|                  +---------------------+-----------------+
| RGB_PACKED       | RGB_PLANAR          |  Condition 1    |
|                  +---------------------+-----------------+
|                  | BGR_PLANAR          |  Condition 1    |
|                  +---------------------+-----------------+
|                  | BGR_PACKED          |  Condition 1    |
|                  +---------------------+-----------------+
|                  | RGBP_SEPARATE       |  Condition 1    |
|                  +---------------------+-----------------+
|                  | BGRP_SEPARATE       |  Condition 1    |
+------------------+---------------------+-----------------+
|                  | RGB_PACKED          |  Condition 1    |
|                  +---------------------+-----------------+
| BGR_PACKED       | RGB_PLANAR          |  Condition 1    |
|                  +---------------------+-----------------+
|                  | BGR_PACKED          |  Condition 1    |
|                  +---------------------+-----------------+
|                  | BGR_PLANAR          |  Condition 1    |
|                  +---------------------+-----------------+
|                  | RGBP_SEPARATE       |  Condition 1    |
|                  +---------------------+-----------------+
|                  | BGRP_SEPARATE       |  Condition 1    |
+------------------+---------------------+-----------------+
|                  | RGB_PACKED          |  Condition 1    |
|                  +---------------------+-----------------+
| RGB_PLANAR       | RGB_PLANAR          |  Condition 1    |
|                  +---------------------+-----------------+
|                  | BGR_PACKED          |  Condition 1    |
|                  +---------------------+-----------------+
|                  | BGR_PLANAR          |  Condition 1    |
|                  +---------------------+-----------------+
|                  | RGBP_SEPARATE       |  Condition 1    |
|                  +---------------------+-----------------+
|                  | BGRP_SEPARATE       |  Condition 1    |
+------------------+---------------------+-----------------+
|                  | RGB_PACKED          |  Condition 1    |
|                  +---------------------+-----------------+
| BGR_PLANAR       | RGB_PLANAR          |  Condition 1    |
|                  +---------------------+-----------------+
|                  | BGR_PACKED          |  Condition 1    |
|                  +---------------------+-----------------+
|                  | BGR_PLANAR          |  Condition 1    |
|                  +---------------------+-----------------+
|                  | RGBP_SEPARATE       |  Condition 1    |
|                  +---------------------+-----------------+
|                  | BGRP_SEPARATE       |  Condition 1    |
+------------------+---------------------+-----------------+
|                  | RGB_PACKED          |  Condition 1    |
|                  +---------------------+-----------------+
| RGBP_SEPARATE    | RGB_PLANAR          |  Condition 1    |
|                  +---------------------+-----------------+
|                  | BGR_PACKED          |  Condition 1    |
|                  +---------------------+-----------------+
|                  | BGR_PLANAR          |  Condition 1    |
|                  +---------------------+-----------------+
|                  | RGBP_SEPARATE       |  Condition 1    |
|                  +---------------------+-----------------+
|                  | BGRP_SEPARATE       |  Condition 1    |
+------------------+---------------------+-----------------+
|                  | RGB_PACKED          |  Condition 1    |
|                  +---------------------+-----------------+
| BGRP_SEPARATE    | RGB_PLANAR          |  Condition 1    |
|                  +---------------------+-----------------+
|                  | BGR_PACKED          |  Condition 1    |
|                  +---------------------+-----------------+
|                  | BGR_PLANAR          |  Condition 1    |
|                  +---------------------+-----------------+
|                  | RGBP_SEPARATE       |  Condition 1    |
|                  +---------------------+-----------------+
|                  | BGRP_SEPARATE       |  Condition 1    |
+------------------+---------------------+-----------------+
| GRAY             | GRAY                |  Condition 1    |
+------------------+---------------------+-----------------+
| YUV420P          | YUV420P             |  Condition 2    |
+------------------+---------------------+-----------------+
| COMPRESSED       | YUV420P             |  Condition 2    |
+------------------+---------------------+-----------------+
| RGB_PACKED       | YUV420P             |  Condition 3    |
+------------------+                     +-----------------+
| RGB_PLANAR       |                     |  Condition 3    |
+------------------+                     +-----------------+
| BGR_PACKED       |                     |  Condition 3    |
+------------------+                     +-----------------+
| BGR_PLANAR       |                     |  Condition 3    |
+------------------+                     +-----------------+
| RGBP_SEPARATE    |                     |  Condition 3    |
+------------------+                     +-----------------+
| BGRP_SEPARATE    |                     |  Condition 3    |
+------------------+---------------------+-----------------+
|                  | RGB_PACKED          |  Condition 4    |
|                  +---------------------+-----------------+
| YUV420P          | RGB_PLANAR          |  Condition 4    |
|                  +---------------------+-----------------+
|                  | BGR_PACKED          |  Condition 4    |
|                  +---------------------+-----------------+
|                  | BGR_PLANAR          |  Condition 4    |
|                  +---------------------+-----------------+
|                  | RGBP_SEPARATE       |  Condition 4    |
|                  +---------------------+-----------------+
|                  | BGRP_SEPARATE       |  Condition 4    |
+------------------+---------------------+-----------------+
|                  | RGB_PACKED          |  Condition 4    |
|                  +---------------------+-----------------+
| NV12             | RGB_PLANAR          |  Condition 4    |
|                  +---------------------+-----------------+
|                  | BGR_PACKED          |  Condition 4    |
|                  +---------------------+-----------------+
|                  | BGR_PLANAR          |  Condition 4    |
|                  +---------------------+-----------------+
|                  | RGBP_SEPARATE       |  Condition 4    |
|                  +---------------------+-----------------+
|                  | BGRP_SEPARATE       |  Condition 4    |
+------------------+---------------------+-----------------+
|                  | RGB_PACKED          |  Condition 4    |
|                  +---------------------+-----------------+
| COMPRESSED       | RGB_PLANAR          |  Condition 4    |
|                  +---------------------+-----------------+
|                  | BGR_PACKED          |  Condition 4    |
|                  +---------------------+-----------------+
|                  | BGR_PLANAR          |  Condition 4    |
|                  +---------------------+-----------------+
|                  | RGBP_SEPARATE       |  Condition 4    |
|                  +---------------------+-----------------+
|                  | BGRP_SEPARATE       |  Condition 4    |
+------------------+---------------------+-----------------+

of which:

     - Condition 1: src.width >= crop.x + crop.width, src.height >= crop.y + crop.height
     - Condition 2: src.width, src.height, dst.width, dst.height must be an integral multiple of 2, src.width >= crop.x + crop.width, src.height >= crop.y + crop.heigh
     - Condition 3: dst.width, dst.height must be an integral multiple of 2, src.width == dst.width, src.height == dst.height, crop.x == 0, crop.y == 0, src.width >= crop.x + crop.width, src.height >= crop.y + crop.height
     - Condition 4: src.width, src.height must be an integral multiple of 2, src.width >= crop.x + crop.width, src.height >= crop.y + crop.height

2. The device mem of input bm_image cannot be on heap0.

3. The stride of all input and output images must be 64 aligned.

4. The addresses of all input and output images must be aligned with 32 byte.

5. The zoom ratio of the image ((crop.width / output.width) and (crop.height / output.height)) is limited to 1 / 32 ~ 32.

6. The width and height (src.width, src.height, dst.width, dst.height) of input and output are limited to 16 ~ 4096.

7. The input must be associated with device memory, otherwise, a failure will be returned.

8.  FORMAT_COMPRESSED is a built-in compression format after VPU decoding. It includes four parts: Y compressed table, Y compressed data, CbCr compressed table and CbCr compressed data. Please note the order of the four parts in bm_image is slightly different from that of the AVFrame in FFMPEG. If you need to attach the device memory data in AVFrame to bm_image, the corresponding relationship is as follows. For details of AVFrame, please refer to "VPU User Manual".

    .. code-block:: c

        bm_device_mem_t src_plane_device[4];
        src_plane_device[0] = bm_mem_from_device((u64)avframe->data[6], avframe->linesize[6]);
        src_plane_device[1] = bm_mem_from_device((u64)avframe->data[4], avframe->linesize[4] * avframe->h);
        src_plane_device[2] = bm_mem_from_device((u64)avframe->data[7], avframe->linesize[7]);
        src_plane_device[3] = bm_mem_from_device((u64)avframe->data[5], avframe->linesize[4] * avframe->h / 2);
        bm_image_attach(*compressed_image, src_plane_device);


**Code example**

    .. code-block:: c

        #include <stdio.h>
        #include <stdlib.h>
        #include <memory>
        #include "bmcv_api_ext.h"
        #include "test_misc.h"
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
            int iw = 1280;
            int ih = 720;
            int rw = 32;
            int rh = 32;
            int crop_w = 32;
            int crop_h = 32;
            int crop_num = 1;
            int pad_h = 0;
            int pad_w = 0;
            bm_image_format_ext fmt_i = FORMAT_YUV420P;
            bm_image_format_ext fmt_o = FORMAT_RGB_PLANAR;
            int ow = rw + 2 * pad_w;
            int oh = rh + 2 * pad_h;
            bmcv_rect_t rect[10];
            bmcv_padding_atrr_t pad[10];
            bm_image src;
            bm_image dst[10];
            bm_handle_t handle;
            int dst_rgb_stride[3] = {(ow + 63) / 64 * 64, (ow + 63) / 64 * 64, (ow + 63) / 64 * 64};
            int src_yuv_stride[3] = {(iw + 63) / 64 * 64, (iw + 31) / 32 * 32, (iw + 31) / 32 * 32};
            unsigned char* input = (unsigned char*)malloc(ih * iw * 3 / 2);
            unsigned char* output = (unsigned char*)malloc(oh * ow * 3);
            unsigned char *host_ptr[3] = {input, input + ih * iw, input + ih * iw * 5 / 4};
            unsigned char *dst_ptr[3] = {output, output + oh * ow, output + oh * ow};
            const char *src_name = "/path/to/src";
            const char *dst_name = "path/to/dst";

            bm_dev_request(&handle, 0);
            readBin(src_name, input, ih * iw * 3 / 2);
            // creat input bm_image and alloc device memory for it
            bm_image_create(handle, ih, iw, fmt_i, DATA_TYPE_EXT_1N_BYTE, &src, src_yuv_stride);
            bm_image_alloc_dev_mem_heap_mask(src, 6);
            bm_image_copy_host_to_device(src, (void **)host_ptr);

            for (int i = 0; i < crop_num; i++) {
                pad[i].dst_crop_stx = pad_w;
                pad[i].dst_crop_sty = pad_h;
                pad[i].dst_crop_w = rw;
                pad[i].dst_crop_h = rh;
                pad[i].padding_r = 0;
                pad[i].padding_g = 0;
                pad[i].padding_b = 0;
                pad[i].if_memset = 0;
                rect[i].start_x = 50 * i;
                rect[i].start_y = 50 * i;
                rect[i].crop_h = crop_h;
                rect[i].crop_w = crop_w;
                rect[i].start_x = rect[i].start_x + crop_w > iw ? iw - crop_w : rect[i].start_x;
                rect[i].start_y = rect[i].start_y + crop_h > ih ? ih - crop_h : rect[i].start_y;
                bm_image_create(handle, oh, ow, fmt_o, DATA_TYPE_EXT_1N_BYTE, dst + i, dst_rgb_stride);
                bm_image_alloc_dev_mem(dst[i]);
            }

            if (pad_h || pad_w) {
                for (int j = 0; j < crop_num; j++) {
                    bm_device_mem_t dev_mem[3];
                    bm_image_get_device_mem(dst[j], dev_mem);
                    for (int k = 0; k < bm_image_get_plane_num(dst[j]); k++) {
                        bm_memset_device(handle, 0, dev_mem[k]);
                    }
                }
            }

            bmcv_image_vpp_basic(handle, 1, &src, dst, &crop_num, rect, pad, BMCV_INTER_NEAREST);
            bm_image_copy_device_to_host(dst[0], (void **)dst_ptr);
            writeBin(dst_name, output, oh * ow * 3);

            bm_image_destroy(src);
            for (int i = 0; i < crop_num; i++) {
                bm_image_destroy(dst[i]);
            }
            bm_dev_free(handle);
            return 0;
        }