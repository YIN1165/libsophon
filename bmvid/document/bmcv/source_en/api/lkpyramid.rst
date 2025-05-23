bmcv_image_lkpyramid
====================

LK pyramid optical flow algorithm. The complete call flow include creation, execution and destruction. The first half of the algorithm uses Tensor Computing Processor, and the second half uses Processor for serial operation. Therefore, for PCIe mode, it is recommended to enable Processor to accelerate. Please refer to Chapter 5 for specific steps.


Create
______

The internal implementation of the algorithm requires some cache space. Therefore, in order to avoid releasing the space repeatedly, some preparatory work is encapsulated in the creation interface. Users can call the execute interface multiple times by calling it once before starting (create function parameters unchanged). The interface form is as follows:

    .. code-block:: c

        bm_status_t bmcv_image_lkpyramid_create_plan(
                    bm_handle_t handle,
                    void*& plan,
                    int width,
                    int height,
                    int winW = 21,
                    int winH = 21,
                    int maxLevel = 3);


**Processor model support**

This interface only supports BM1684.


**Input parameter description:**

* bm_handle_t handle

  Input parameter. The handle of bm_handle.

* void*& plan

  Output parameter. Handle required by the execution phase.

* int width

  Input parameter. The width of the image to be processed.

* int height

  Input parameter. The height of the image to be processed.

* int winW

  Input parameter. The width of the algorithm processing window, and the default value is 21.

* int winH

  Input parameter. The height of the algorithm processing window, and the default value is 21.

* int maxLevel

  Input parameter. The height of pyramid processing. The default value is 3, and the maximum value currently supported is 5. The larger the parameter value, the longer the execution time of the algorithm. It is recommended to select the acceptable minimum value according to the actual effect.


**Return value description:**

* BM_SUCCESS: success

* Other: failed


Execute
_______

The plan created with the above interface can start the real execution phase. The interface format is as follows:

    .. code-block:: c

        typedef struct {
            float x;
            float y;
        } bmcv_point2f_t;

        typedef struct {
            int type;   // 1: maxCount   2: eps   3: both
            int max_count;
            double epsilon;
        } bmcv_term_criteria_t;

        bm_status_t bmcv_image_lkpyramid_execute(
                bm_handle_t handle,
                void* plan,
                bm_image prevImg,
                bm_image nextImg,
                int ptsNum,
                bmcv_point2f_t* prevPts,
                bmcv_point2f_t* nextPts,
                bool* status,
                bmcv_term_criteria_t criteria = {3, 30, 0.01});


**Input parameter description:**

* bm_handle_t handle

  Input parameter. The handle of bm_handle.

* const void \*plan

  Input parameter. The handle obtained during the creation phase.

* bm_image prevImg

  Input parameter. The bm_image of the previous image. The creation of bm_image requires an external call to bmcv_image_create. Image memory can use bm_image_alloc_dev_mem or bm_image_copy_host_to_device to create new memory, or use bmcv_image_attach to attach existing memory.

* bm_image nextImg

  Input parameter. The bm_image of the next image. The creation of bm_image requires an external call to bmcv_image_create. Image memory can use bm_image_alloc_dev_mem or bm_image_copy_host_to_device to create new memory, or use bmcv_image_attach to attach existing memory.

* int ptsNum

  Input parameter. The number of points to be tracked.

* bmcv_point2f_t* prevPts

  Input parameter. It is required to track the coordinate pointer of the point in the previous image. Its pointing length is ptsNum.

* bmcv_point2f_t* nextPts

  Output parameter. The coordinate pointer of calculated tracking point in the next image. Its pointing length is ptsNum.

* bool* status

  Output parameter. Whether each tracking point in nextPts is valid or not. Its pointing length is ptsNum, which corresponds to the coordinates in nextPts one by one. If it is valid, it is true, otherwise it is false (it means that the corresponding tracking point is not found in the next image, which may exceed the image range).


* bmcv_term_criteria_t criteria

  Input parameter. Iteration end criteria. Type indicates which parameter is used as the judgment condition of end: if it is 1, it is determined by the number of iterations max_count as the end judgment parameter. If it is 2, the error epsilon is the end judgment parameter. If it is 3, both must be met. This parameter will affect the execution time. It is suggested to select the optimal standard according to the actual effect.


**Return value description:**

* BM_SUCCESS: success

* Other: failed


Destruction
___________

When the execution is completed, the created handle needs to be destroyed. This interface must be the same as the creation interface bmcv_image_lkpyramid_create_plan and used in pairs.

    .. code-block:: c

        void bmcv_image_lkpyramid_destroy_plan(bm_handle_t handle, void *plan);


**Format support:**

The interface currently supports the following image_format:

+-----+------------------------+
| num | image_format           |
+=====+========================+
| 1   | FORMAT_GRAY            |
+-----+------------------------+

The interface currently supports the following data_type:

+-----+--------------------------------+
| num | data_type                      |
+=====+================================+
| 1   | DATA_TYPE_EXT_1N_BYTE          |
+-----+--------------------------------+


Sample Code
___________

    .. code-block:: c

        #include <iostream>
        #include <fstream>
        #include <thread>
        #include <mutex>
        #include <stdio.h>
        #include <stdlib.h>
        #include <string.h>
        #include <assert.h>
        #include <math.h>
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

        int main()
        {
            bm_handle_t handle;
            int width = 1024;
            int height = 1024;
            bm_image_format_ext fmt = FORMAT_GRAY;
            bm_image prevImg;
            bm_image nextImg;
            void *plan = nullptr;
            unsigned char* prevPtr = new unsigned char[width * height];
            unsigned char* nextPtr = new unsigned char[width * height];
            bmcv_term_criteria_t criteria = {3, 10, 0.03};
            int ptsNum = 10;
            int kw = 41;
            int kh = 47;
            int maxLevel = 3;
            bool* status = new bool [ptsNum];
            bmcv_point2f_t* prevPts = new bmcv_point2f_t [ptsNum];
            bmcv_point2f_t* nextPts = new bmcv_point2f_t [ptsNum];
            const char *src_names[2] = {"path/to/src0", "path/to/src1"};

            readBin(src_names[0], prevPtr, width * height);
            readBin(src_names[1], nextPtr, width * height);

            for (int i = 0; i < ptsNum; ++i) {
                prevPts[i].x = (float)rand() / RAND_MAX;
                nextPts[i].y = (float)rand() / RAND_MAX;
            }

            bm_dev_request(&handle, 0);
            bmcv_open_cpu_process(handle);
            bm_image_create(handle, height, width, fmt, DATA_TYPE_EXT_1N_BYTE, &prevImg);
            bm_image_create(handle, height, width, fmt, DATA_TYPE_EXT_1N_BYTE, &nextImg);
            bm_image_alloc_dev_mem(prevImg);
            bm_image_alloc_dev_mem(nextImg);
            bm_image_copy_host_to_device(prevImg, (void **)(&prevPtr));
            bm_image_copy_host_to_device(nextImg, (void **)(&nextPtr));

            bmcv_image_lkpyramid_create_plan(handle, plan, width, height, kw, kh, maxLevel);
            bmcv_image_lkpyramid_execute(handle, plan, prevImg, nextImg, ptsNum,
                                        prevPts, nextPts, status, criteria);
            bmcv_image_lkpyramid_destroy_plan(handle, plan);
            bmcv_close_cpu_process(handle);

            bm_image_destroy(prevImg);
            bm_image_destroy(nextImg);
            bm_dev_free(handle);
            delete[] prevPtr;
            delete[] nextPtr;
            delete[] prevPts;
            delete[] nextPts;
            delete[] status;
            return 0;
        }