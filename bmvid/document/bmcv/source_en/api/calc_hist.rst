bmcv_calc_hist
==================

Histogram
_________


**Interface form:**

    .. code-block:: c

        bm_status_t bmcv_calc_hist(
                    bm_handle_t handle,
                    bm_device_mem_t input,
                    bm_device_mem_t output,
                    int C,
                    int H,
                    int W,
                    const int *channels,
                    int dims,
                    const int *histSizes,
                    const float *ranges,
                    int inputDtype);


**Processor model support**

This interface supports BM1684/BM1684X.


**Parameter Description:**

* bm_handle_t handle

  Input parameter. The handle of bm_handle.

* bm_device_mem_t input

  Input parameter. The device memory space stores the input data. The type can be float32 or uint8, which is determined by the parameter inputDtype. Its size is C*H*W*sizeof (Dtype).

* bm_device_mem_t output

  Output parameter. The device memory space stores the output results. The type is float and its size is histSizes[0]*histSizes[1]*...*histSizes[n]*sizeof (float).

* int C

  Input parameter. Number of channels for input data.

* int H

  Input parameter. The height of each channel of the input data.

* int W

  Input parameter. The width of each channel of the input data.

* const int \*channels

  Input parameter. The channel list of histogram needs to be calculated. Its length is dims, and the value of each element must be less than C.

* int dims

  Input parameter. The output histogram dimension, which shall not be greater than 3.

* const int \*histSizes

  Input parameter. Corresponding to the number of copies of each channel statistical histogram. Its length is dims.

* const float \*ranges

  Input parameter. The range of each channel participating in statistics, with a length of 2*dims.

* int inputDtype

  Input parameter. Type of input data: 0 means float, 1 means uint8.


**Return value description:**

* BM_SUCCESS: success

* Other: failed


**Code example:**

    .. code-block:: c

        #include "bmcv_api_ext.h"
        #include <math.h>
        #include <stdio.h>
        #include <stdint.h>
        #include <stdlib.h>
        #include <string.h>

        int main()
        {
            int H = 1024;
            int W = 1024;
            int C = 3;
            int dim = 3;
            int channels[3] = {0, 1, 2};
            int histSizes[3] = {32, 32, 32};
            float ranges[6] = {0, 256, 0, 256, 0, 256};
            int totalHists = 1;
            bm_handle_t handle;
            float *inputHost = new float[C * H * W];
            float *outputHost = new float[totalHists];
            bm_device_mem_t input, output;

            for (int i = 0; i < dim; ++i) {
                totalHists *= histSizes[i];
            }

            bm_dev_request(&handle, 0);

            for (int i = 0; i < C; ++i) {
                for (int j = 0; j < H * W; ++j) {
                    inputHost[i * H * W + j] = (float)(rand() % 256);
                }
            }

            bm_malloc_device_byte(handle, &input, C * H * W * sizeof(float));
            bm_memcpy_s2d(handle, input, inputHost);
            bm_malloc_device_byte(handle, &output, totalHists * sizeof(float));
            bmcv_calc_hist(handle, input, output, C, H, W, channels, dim, histSizes, ranges, 0);
            bm_memcpy_d2s(handle, outputHost, output);

            bm_free_device(handle, input);
            bm_free_device(handle, output);
            bm_dev_free(handle);
            delete[] inputHost;
            delete[] outputHost;
            return 0;
        }


Weighted Histogram
__________________

**Processor model support**

This interface supports BM1684/BM1684X.


**Interface form:**

    .. code-block:: c

        bm_status_t bmcv_calc_hist_with_weight(
                    bm_handle_t handle,
                    bm_device_mem_t input,
                    bm_device_mem_t output,
                    const float *weight,
                    int C,
                    int H,
                    int W,
                    const int *channels,
                    int dims,
                    const int *histSizes,
                    const float *ranges,
                    int inputDtype);


**Parameter Description:**

* bm_handle_t handle

  Input parameter. The handle of bm_handle.

* bm_device_mem_t input

  Input parameter. The device memory space stores the input data, and its size is C*H*W* sizeof (Dtype).

* bm_device_mem_t output

  Output parameter. The device memory space stores the output results. The type is float, and its size is histSizes[0]* histSizes[1]*...*histSizes[n]*sizeof (float).

* const float \*weight

  Input parameter. The weight of each element in the channel during histogram statistics. Its size is H*W*sizeof (float). If all values are 1, it has the same function as the ordinary histogram.

* int C

  Input parameter. Number of channels for input data.

* int H

  Input parameter. The height of each channel of the input data

* int W

  Input parameter. The width of each channel of the input data.

* const int \*channels

  Input parameter. The channel list of histogram needs to be calculated. Its length is dims, and the value of each element must be less than C.

* int dims

  Input parameter. The output histogram dimension shall not be greater than 3.

* const int \*histSizes

  Input parameter. Corresponding to the number of copies of each channel statistical histogram. Its length is dims.

* const float \*ranges

  Input parameter. The range of each channel participating in statistics, with a length of 2*dims.

* int inputDtype

  Input parameter. Type of input data: 0 means float, 1 means uint8.


**Return value description:**

* BM_SUCCESS: success

* Other: failed