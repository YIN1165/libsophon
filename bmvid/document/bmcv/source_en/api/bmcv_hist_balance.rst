bmcv_hist_balance
===================

Perform histogram equalization on the image to improve the contrast of the image.


**Processor model support**

This interface supports BM1684X.


**Interface form:**

    .. code-block:: c

        bm_status_t bmcv_hist_balance(
                    bm_handle_t handle,
                    bm_device_mem_t input,
                    bm_device_mem_t output,
                    int H,
                    int W);


**Description of parameters:**

* bm_handle_t handle

  Input parameters. The handle of bm_handle.

* bm_device_mem_t input

  Input parameter. The device memory space stores the input data. Its size is H * W * sizeof (uint8_t).

* bm_device_mem_t output

  Output parameter. The device memory space stores the input data. Its size is H * W * sizeof (uint8_t).

* int H

  Input parameter. The height of the input image.

* int W

  Input parameter. The width of the input image.


**Return value description:**

* BM_SUCCESS: success

* Other: failed


**Note**

1. The data type only support uint8_t。

2. The min height and width of the input image is H = 1, W = 1。

3. The max height and width of the input image is H = 8192, W = 8192。


**Code example:**

    .. code-block:: c

        #include <math.h>
        #include <stdio.h>
        #include <stdint.h>
        #include <stdlib.h>
        #include <string.h>
        #include "bmcv_api_ext.h"

        int main()
        {
            int H = 1024;
            int W = 1024;
            uint8_t* input_addr = (uint8_t*)malloc(H * W * sizeof(uint8_t));
            uint8_t* output_addr = (uint8_t*)malloc(H * W * sizeof(uint8_t));
            bm_handle_t handle;
            bm_device_mem_t input, output;

            for (int i = 0; i < W * H; ++i) {
                input_addr[i] = (uint8_t)rand() % 256;
            }

            bm_dev_request(&handle, 0);
            bm_malloc_device_byte(handle, &input, H * W * sizeof(uint8_t));
            bm_malloc_device_byte(handle, &output, H * W * sizeof(uint8_t));

            bm_memcpy_s2d(handle, input, input_addr);
            bmcv_hist_balance(handle, input, output, H, W);
            bm_memcpy_d2s(handle, output_addr, output);

            free(input_addr);
            free(output_addr);
            bm_free_device(handle, input);
            bm_free_device(handle, output);
            bm_dev_free(handle);
            return 0;
        }