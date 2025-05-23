bmcv_image_dct
===============

对图像进行DCT变换。


**处理器型号支持：**

该接口仅支持BM1684。


**接口形式：**

    .. code-block:: c

        bm_status_t bmcv_image_dct(
                    bm_handle_t handle,
                    bm_image input,
                    bm_image output,
                    bool is_inversed);


**输入参数说明：**

* bm_handle_t handle

  输入参数。bm_handle 句柄

* bm_image input

  输入参数。输入 bm_image，bm_image 需要外部调用 bmcv_image _create 创建。image 内存可以使用 bm_image_alloc_dev_mem 或者 bm_image_copy_host_to_device 来开辟新的内存，或者使用 bmcv_image_attach 来 attach 已有的内存。

* bm_image output

  输出参数。输出 bm_image，bm_image 需要外部调用 bmcv_image_create 创建。image 内存可以通过 bm_image_alloc_dev_mem 来开辟新的内存，或者使用 bmcv_image_attach 来 attach 已有的内存。如果不主动分配将在 api 内部进行自行分配。

* bool is_inversed

  输入参数。是否为逆变换。


**返回值说明:**

* BM_SUCCESS: 成功

* 其他: 失败


**注意事项**

由于DCT变换的系数仅与图像的width和height相关，而上述接口每次调用都需要重新计算变换系数，对于相同大小的图像，为了避免重复计算变换系数的过程，可以将上述接口拆分成两步完成：

1. 首先算特定大小的变换系数;

2. 然后可以重复利用改组系数对相同大小的图像做DCT变换。


计算系数的接口形式如下：

    .. code-block:: c

        bm_status_t bmcv_dct_coeff(
                    bm_handle_t handle,
                    int H,
                    int W,
                    bm_device_mem_t hcoeff_output,
                    bm_device_mem_t wcoeff_output,
                    bool is_inversed);


**输入参数说明：**

* bm_handle_t handle

  输入参数。bm_handle 句柄

* int H

  输入参数。图像的高度。

* int W

  输入参数。图像的宽度。

* bm_device_mem_t hcoeff_output

  输出参数。该device memory空间存储着h维度的DCT变换系数，对于H*W大小的图像，该空间的大小为H*H*sizeof(float)。

* bm_device_mem_t wcoeff_output

  输出参数。该device memory空间存储着w维度的DCT变换系数，对于H*W大小的图像，该空间的大小为W*W*sizeof(float)。

* bool is_inversed

  输入参数。是否为逆变换。


**返回值说明:**

* BM_SUCCESS: 成功

* 其他: 失败


得到系数之后，将其传给下列接口开始计算过程：

    .. code-block:: c

        bm_status_t bmcv_image_dct_with_coeff(
                    bm_handle_t handle,
                    bm_image input,
                    bm_device_mem_t hcoeff,
                    bm_device_mem_t wcoeff,
                    bm_image output);


**输入参数说明：**

* bm_handle_t handle

  输入参数。bm_handle 句柄

* bm_image input

  输入参数。输入 bm_image，bm_image 需要外部调用 bmcv_image _create 创建。image 内存可以使用 bm_image_alloc_dev_mem 或者 bm_image_copy_host_to_device 来开辟新的内存，或者使用 bmcv_image_attach 来 attach 已有的内存。

* bm_device_mem_t hcoeff

  输入参数。该device memory空间存储着h维度的DCT变换系数，对于H*W大小的图像，该空间的大小为H*H*sizeof(float)。

* bm_device_mem_t wcoeff

  输入参数。该device memory空间存储着w维度的DCT变换系数，对于H*W大小的图像，该空间的大小为W*W*sizeof(float)。

* bm_image output

  输出参数。输出 bm_image，bm_image 需要外部调用 bmcv_image_create 创建。image 内存可以通过 bm_image_alloc_dev_mem 来开辟新的内存，或者使用 bmcv_image_attach 来 attach 已有的内存。如果不主动分配将在 api 内部进行自行分配。


**返回值说明:**

* BM_SUCCESS: 成功

* 其他: 失败


**格式支持：**

该接口目前支持以下 image_format:

+-----+------------------------+------------------------+
| num | input image_format     | output image_format    |
+=====+========================+========================+
| 1   | FORMAT_GRAY            | FORMAT_GRAY            |
+-----+------------------------+------------------------+

目前支持以下 data_type:

+-----+--------------------------------+
| num | data_type                      |
+=====+================================+
| 1   | DATA_TYPE_EXT_FLOAT32          |
+-----+--------------------------------+


**注意事项：**

1、在调用该接口之前必须确保输入的 image 内存已经申请。

2、input output 的 data_type必须相同。


**示例代码**

    .. code-block:: c

        #include <iostream>
        #include <fstream>
        #include <assert.h>
        #include <memory>
        #include <string>
        #include <numeric>
        #include <vector>
        #include <cmath>
        #include <cassert>
        #include <algorithm>
        #include "bmcv_api_ext.h"
        #include "test_misc.h"

        static void readBin(const char* path, unsigned char* input_data, int size)
        {
            FILE *fp_src = fopen(path, "rb");

            if (fread((void *)input_data, 4, size, fp_src) < (unsigned int)size) {
                printf("file size is less than %d required bytes\n", size);
            };

            fclose(fp_src);
        }

        static void writeBin(const char * path, unsigned char* input_data, int size)
        {
            FILE *fp_dst = fopen(path, "wb");
            if (fwrite((void *)input_data, 4, size, fp_dst) < (unsigned int)size) {
                printf("file size is less than %d required bytes\n", size);
            };

            fclose(fp_dst);
        }

        int main()
        {
            int channel = 1;
            int width = 1920;
            int height = 1080;
            int dev_id = 0;
            bm_handle_t handle;
            bm_image bm_input, bm_output;
            float * src_data = new float[channel * width * height];
            float * res_data = new float[channel * width * height];
            bm_device_mem_t hcoeff_mem;
            bm_device_mem_t wcoeff_mem;
            bool is_inversed = true;
            const char *input_path = "path/to/input";
            const char *output_path = "path/to/output";

            bm_dev_request(&handle, dev_id);
            readBin(input_path, (unsigned char*)src_data, channel * width * height);
            bm_image_create(handle, height, width, FORMAT_GRAY, DATA_TYPE_EXT_FLOAT32, &bm_input);
            bm_image_alloc_dev_mem(bm_input);
            bm_image_copy_host_to_device(bm_input, (void **)&src_data);
            bm_image_create(handle, height, width, FORMAT_GRAY, DATA_TYPE_EXT_FLOAT32, &bm_output);
            bm_image_alloc_dev_mem(bm_output);
            bm_malloc_device_byte(handle, &hcoeff_mem, height * height * sizeof(float));
            bm_malloc_device_byte(handle, &wcoeff_mem, width * width * sizeof(float));
            bmcv_dct_coeff(handle, bm_input.height, bm_input.width, hcoeff_mem, wcoeff_mem, is_inversed);
            bmcv_image_dct_with_coeff(handle, bm_input, hcoeff_mem, wcoeff_mem, bm_output);
            bm_image_copy_device_to_host(bm_output, (void **)&res_data);
            writeBin(output_path, (unsigned char*)res_data, channel * width * height);

            bm_image_destroy(bm_input);
            bm_image_destroy(bm_output);
            bm_free_device(handle, hcoeff_mem);
            bm_free_device(handle, wcoeff_mem);
            delete[] src_data;
            delete[] res_data;
            bm_dev_free(handle);
            return 0;
        }