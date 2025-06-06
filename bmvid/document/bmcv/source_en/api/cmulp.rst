bmcv_cmulp
==========

This interface is used to implement the complex number multiplication, as shown in the following formula:

  .. math::
    \text{outputReal} + \text{outputImag} \times i = (\text{inputReal} + \text{inputImag} \times i) \times (\text{pointReal} + \text{pointImag} \times i)
  .. math::
    \text{outputReal} = \text{inputReal} \times \text{pointReal} - \text{inputImag} \times \text{pointImag}
  .. math::
    \text{outputImag} = \text{inputReal} \times \text{pointImag} + \text{inputImag} \times \text{pointReal}

  Among that, :math:`i` is the imaginary unit and satisfying the equation :math:`i^2 = -1`.


**Processor model support**

This interface supports BM1684/BM1684X.


**Interface form:**

    .. code-block:: c++

        bm_status_t bmcv_cmulp(
                    bm_handle_t handle,
                    bm_device_mem_t inputReal,
                    bm_device_mem_t inputImag,
                    bm_device_mem_t pointReal,
                    bm_device_mem_t pointImag,
                    bm_device_mem_t outputReal,
                    bm_device_mem_t outputImag,
                    int batch,
                    int len);


**Input parameter description:**

* bm_handle_t handle

  Input parameter. The handle of bm_handle.

* bm_device_mem_t inputReal

  Input parameter. Device addr information of the real part of the input.

* bm_device_mem_t inputImag

  Input parameter. Device addr information of the imaginary part of the input.

* bm_device_mem_t pointReal

  Input parameter. Device addr information of the real part of the point.

* bm_device_mem_t pointImag

  Input parameter. Device addr information of the imaginary part of the point.

* bm_device_mem_t outputReal

  Output parameter. Device addr information of the real part of the output.

* bm_device_mem_t outputImag

  Output parameter. Device addr information of the imaginary part of the output.

* int batch

  Input parameter. The number of batches.

* int len

  Input parameter. The number of the complex numbers in a batch.


**Return value description:**

* BM_SUCCESS: success

* Other: failed


**Note:**

1. Data type: only support float.


**Sample code**

    .. code-block:: c++

        #include "bmcv_api_ext.h"
        #include <stdio.h>
        #include <stdlib.h>

        int main()
        {
            int L = 1 + rand() % 4096;
            int batch = 1 + rand() % 1980;
            float *XRHost = new float[L * batch];
            float *XIHost = new float[L * batch];
            float *PRHost = new float[L];
            float *PIHost = new float[L];
            float *YRHost = new float[L * batch];
            float *YIHost = new float[L * batch];
            bm_handle_t handle;
            bm_device_mem_t XRDev, XIDev, PRDev, PIDev, YRDev, YIDev;

            for (int i = 0; i < L * batch; ++i) {
                XRHost[i] = rand() % 5 - 2;
                XIHost[i] = rand() % 5 - 2;
            }
            for (int i = 0; i < L; ++i) {
                PRHost[i] = rand() % 5 - 2;
                PIHost[i] = rand() % 5 - 2;
            }

            bm_dev_request(&handle, 0);
            bm_malloc_device_byte(handle, &XRDev, L * batch * sizeof(float));
            bm_malloc_device_byte(handle, &XIDev, L * batch * sizeof(float));
            bm_malloc_device_byte(handle, &PRDev, L * sizeof(float));
            bm_malloc_device_byte(handle, &PIDev, L * sizeof(float));
            bm_malloc_device_byte(handle, &YRDev, L * batch * sizeof(float));
            bm_malloc_device_byte(handle, &YIDev, L * batch * sizeof(float));
            bm_memcpy_s2d(handle, XRDev, XRHost);
            bm_memcpy_s2d(handle, XIDev, XIHost);
            bm_memcpy_s2d(handle, PRDev, PRHost);
            bm_memcpy_s2d(handle, PIDev, PIHost);

            bmcv_cmulp(handle, XRDev, XIDev, PRDev, PIDev, YRDev, YIDev, batch, L);
            bm_memcpy_d2s(handle, YRHost, YRDev);
            bm_memcpy_d2s(handle, YIHost, YIDev);

            delete[] XRHost;
            delete[] XIHost;
            delete[] PRHost;
            delete[] PIHost;
            delete[] YRHost;
            delete[] YIHost;
            bm_free_device(handle, XRDev);
            bm_free_device(handle, XIDev);
            bm_free_device(handle, YRDev);
            bm_free_device(handle, YIDev);
            bm_free_device(handle, PRDev);
            bm_free_device(handle, PIDev);
            bm_dev_free(handle);
            return 0;
        }