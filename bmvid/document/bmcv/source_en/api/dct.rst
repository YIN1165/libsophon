bmcv_image_dct
===============

DCT transformation of the image.


**Interface Form**

    .. code-block:: c

        bm_status_t bmcv_image_dct(
                    bm_handle_t handle,
                    bm_image input,
                    bm_image output,
                    bool is_inversed);


**Processor model support**

This interface only supports BM1684.


**Description of input parameters:**

* bm_handle_t handle

  Input parameter. The handle of bm_handle.

* bm_image input

  Input parameter. The bm_image of the input image. The creation of bm_image requires an external call to bmcv_image_create. The image memory can use bm_image_alloc_dev_mem or bm_image_copy_host_to_device to create new memory, or use bmcv_image_attach to attach existing memory.

* bm_image output

  Output parameter. Output bm_image. The creation of bm_image requires an external call to bmcv_image_create. Image memory can use bm_image_alloc_dev_mem to create new memory, or use bmcv_image_attach to attach existing memory. If users do not actively allocate, it will be allocated automatically within the API.

* bool is_inversed

  Input parameter. Whether it is inverse transformation.


**Return value description::**

* BM_SUCCESS: success

* Other: failed


The coefficients of DCT transformation are only related to the width and height of the image, and the transformation coefficients need to be recalculated every time the above interface is called. Therefore, for images of the same size, in order to avoid repeated calculation of transformation coefficients, the above interface can be divided into two steps:

1. Calculate the transformation coefficient of a specific size;

2. Reuse the reorganized coefficients to perform DCT transformation on images of the same size.


The interface form of calculation coefficient is as follows:

    .. code-block:: c

        bm_status_t bmcv_dct_coeff(
                bm_handle_t handle,
                int H,
                int W,
                bm_device_mem_t hcoeff_output,
                bm_device_mem_t wcoeff_output,
                bool is_inversed);


**Description of input parameters:**

* bm_handle_t handle

  Input parameter. The handle of bm_handle.

* int H

  Input parameter. The height of the image.

* int W

  Input parameter. The width of the image.

* bm_device_mem_t hcoeff_output

  Output parameter. The device memory space stores the DCT transformation coefficients of the h dimension. For images with the size of H*W, the size of the space is H*H*sizeof (float).

* bm_device_mem_t wcoeff_output

  Output parameter. The device memory space stores DCT transformation coefficients of the w dimension. For images with the size of H*W, the size of the space is W*W*sizeof (float).

* bool is_inversed

  Input parameter. Whether it is inverse transformation.


**Return value description:**

* BM_SUCCESS: success

* Other: failed


After obtaining the coefficient, transfer it to the following interfaces to start the calculation:

    .. code-block:: c

        bm_status_t bmcv_image_dct_with_coeff(
                bm_handle_t handle,
                bm_image input,
                bm_device_mem_t hcoeff,
                bm_device_mem_t wcoeff,
                bm_image output);


**Description of input parameters:**

* bm_handle_t handle

  Input parameters. The handle of bm_handle.

* bm_image input

  Input parameter. The bm_image of the input image. The creation of bm_image requires an external call to bmcv_image_create. The image memory can use bm_image_alloc_dev_mem or bm_image_copy_host_to_device to create new memory, or use bmcv_image_attach to attach existing memory.

* bm_device_mem_t hcoeff

  Input parameter. The device memory space stores the DCT transformation coefficients of the h dimension. For the image with the size of H*W, the size of the space is H*H*sizeof (float).

* bm_device_mem_t wcoeff

  Input parameter. The device memory space stores the DCT transformation coefficients of the w dimension. For the image with the size of H*W, the size of the space is W*W*sizeof (float).

* bm_image output

  Output bm_image. The creation of bm_image requires an external call to bmcv_image_create. Image memory can use bm_image_alloc_dev_mem to create new memory, or use bmcv_image_attach to attach existing memory. If users do not actively allocate, it will be allocated automatically within the API.


**Return value description:**

* BM_SUCCESS: success

* Other: failed


**Format support:**

The interface currently supports the following image_format:

+-----+------------------------+------------------------+
| num | input image_format     | output image_format    |
+=====+========================+========================+
| 1   | FORMAT_GRAY            | FORMAT_GRAY            |
+-----+------------------------+------------------------+

The interface currently supports the following data_type:

+-----+--------------------------------+
| num | data_type                      |
+=====+================================+
| 1   | DATA_TYPE_EXT_FLOAT32          |
+-----+--------------------------------+


**Note**

1. Before calling this interface, users must ensure that the input image memory has been applied for.

2. The data_type of input and output must be the same.


**Sample code**

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