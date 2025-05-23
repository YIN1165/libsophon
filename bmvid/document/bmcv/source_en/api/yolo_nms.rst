bmcv_nms_yolo
==============

This interface supports yolov3/yolov7, which is used to eliminate too many object boxes obtained by network calculation and find the best object box.


**Processor model support**

This interface supports BM1684/BM1684X.


**Interface form:**

    .. code-block:: c

        bm_status_t bmcv_nms_yolo(
                    bm_handle_t handle,
                    int input_num,
                    bm_device_mem_t bottom[3],
                    int batch_num,
                    int hw_shape[3][2],
                    int num_classes,
                    int num_boxes,
                    int mask_group_size,
                    float nms_threshold,
                    float confidence_threshold,
                    int keep_top_k,
                    float bias[18],
                    float anchor_scale[3],
                    float mask[9],
                    bm_device_mem_t output,
                    int yolo_flag,
                    int len_per_batch,
                    void *ext);


**Parameter Description:**

* bm_handle_t handle

  Input parameter. The handle of bm_handle.

* int input_num

  Input parameter. Input the number of feature maps.

* bm_device_mem_t bottom[3]

  Input parameter.The bottom device address needs to be called bm_mem_from_system() to convert the data address into the structure corresponding to bm_device_mem_t.

* int batch_num

  Input parameter.The number of batches.

* int hw_shape[3][2]

  Input parameter. Input the h, w of the feature map.

* int num_classes

  Input parameter. Number of categories of images.

* int num_boxes

  Input parameter. How many anchor boxes of different scales each grid contains.

* int mask_group_size

  Input parameter. size of mask.

* float nms_threshold

  Input parameter. Threshold for filtering object boxes, object boxes with scores less than this threshold will be filtered out.

* int confidence_threshold

  Input parameter. Confidence.

* int keep_top_k

  Input parameter. Save the first k numbers.

* int bias[18]

  Input parameter. Bias.

* float anchor_scale[3]

  Input parameter. The size of anchor.

* float mask[9]

  Input parameter.Mask.

* bm_device_mem_t output

  Input parameter. For the output device address, you need to call bm_mem_from_system() to convert the data address into the structure corresponding to bm_device_mem_t.

* int yolo_flag

  Input parameter. yolo_flag=0 when yolov3, yolo_flag=2 when yolov7.

* int len_per_batch

  Input parameter. This parameter is not valid and is only intended to maintain interface compatibility.

* int scale

  Input parameter. Target size. This parameter only takes effect in yolov7.

* int \*orig_image_shape

  Input parameter. The w/h of the original image is arranged in batches, such as batch4: w1 h1 w2 h2 w3 h3 w4 h4. This parameter only takes effect in yolov7.

* int model_h

  Input parameter.The shape h of the model, this parameter only takes effect in yolov7.

* int model_w

  Input parameter. The shape w of the model, this parameter only takes effect in yolov7.

* void \*ext

  Reserved parameters. If you need to add new parameters, you can add them here. Four new parameters have been added to yolov7 as:

    .. code-block:: c

        typedef struct yolov7_info{
            int scale;
            int *orig_image_shape;
            int model_h;
            int model_w;
        } yolov7_info_t;

In the above structure, int scale: scale_flag. int* orig_image_shape: w/h of the original image, sorted by batch cloth, such as batch4: w1 h1 w2 h2 w3 h3 w4 h4. int model_h: The shape h of the model. int model_w: The shape w of the model. These parameters only take effect in yolov7.


**Return value:**

* BM_SUCCESS: success

* Other: failed


**Code example::**

    .. code-block:: c

        #include <time.h>
        #include <random>
        #include <algorithm>
        #include <map>
        #include <vector>
        #include <iostream>
        #include <cmath>
        #include <getopt.h>
        #include "bmcv_api_ext.h"
        #include "bmcv_common_bm1684.h"
        #include "math.h"
        #include "stdio.h"
        #include "stdlib.h"
        #include "string.h"
        #include <iostream>
        #include <new>
        #include <fstream>

        typedef struct yolov7_info{
            int scale;
            int *orig_image_shape;
            int model_h;
            int model_w;
        } yolov7_info_t;

        int main()
        {
            int DEV_ID = 0;
            int H = 16, W = 30;
            int bottom_num = 3;
            int dev_count;
            int f_tpu_forward = 1;
            int batch_num = 32;
            int num_classes = 6;
            int num_boxes = 3;
            int yolo_flag = 0; //yolov3: 0, yolov7: 2
            int len_per_batch = 0;
            int keep_top_k = 100;
            float nms_threshold = 0.1;
            float conf_threshold = 0.98f;
            int mask_group_size = 3;
            float bias[18] = {10, 13, 16, 30, 33, 23, 30, 61, 62, 45, 59, 119, 116, 90, 156, 198, 373, 326};
            float anchor_scale[3] = {32, 16, 8};
            float mask[9] = {6, 7, 8, 3, 4, 5, 0, 1, 2};
            int scale = 0; //for yolov7 post handle
            int model_h = 0;
            int model_w = 0;
            int mode_value_end = 0;
            int hw_shape[3][2] = {{H * 1, W * 1},
                                {H * 2, W * 2},
                                {H * 4, W * 4},};
            int size_bottom[3];
            float* data_bottom[3];
            int origin_image_shape[batch_num * 2] = {0};
            float* output_bmdnn;
            float* output_native;
            bm_handle_t handle;
            int output_size = 1;

            bm_dev_request(&handle, 0);
            if (yolo_flag == 1) {
                num_boxes = 1;
                len_per_batch = 12096 * 18;
                bottom_num = 1;
            } else if (yolo_flag == 2) {
                //yolov7 post handle;
                num_boxes = 1;
                bottom_num = 3;
                mask_group_size = 1;
                scale = 1;
                model_h = 512;
                model_w = 960;
                for (int i = 0 ; i < 3; i++) {
                    mask[i] = i;
                }
                for (int i = 0; i < 6; i++) {
                    bias[i] = 1;
                }
                for (int i = 0; i < 3; i++) {
                    anchor_scale[i] = 1;
                }
                for (int i = 0; i < batch_num; i++) {
                    origin_image_shape[i * 2 + 0] = 1920;
                    origin_image_shape[i * 2 + 1] = 1080;
                }
            }
            // alloc input data
            for (int i = 0; i < 3; ++i) {
                if (yolo_flag == 1){
                    size_bottom[i] = batch_num * len_per_batch;
                } else {
                    size_bottom[i] = batch_num * num_boxes * (num_classes + 5) * hw_shape[i][0] * hw_shape[i][1];
                }
                data_bottom[i] = new float[size_bottom[i]];
            }
            // alloc and init input data
            for (int j = 0; j < size_bottom[0]; ++j) {
                data_bottom[0][j] = (rand() % 1000 - 999.0f) / (124.0f);
            }
            for (int j = 0; j < size_bottom[1]; ++j) {
                data_bottom[1][j] = (rand() % 1000 - 999.0f) / (124.0f);
            }
            for (int j = 0; j < size_bottom[2]; ++j) {
                data_bottom[2][j] = (rand() % 1000 - 999.0f) / (124.0f);
            }

            output_bmdnn = new float[output_size];
            memset(output_bmdnn, 0, output_size * sizeof(float));

            bm_dev_request(&handle, 0);
            bm_device_mem_t bottom[3] = {
                                    bm_mem_from_system((void*)data_bottom[0]),
                                    bm_mem_from_system((void*)data_bottom[1]),
                                    bm_mem_from_system((void*)data_bottom[2])};
            yolov7_info_t *ext = (yolov7_info_t*)malloc (sizeof(yolov7_info_t));
            ext->scale = scale;
            ext->orig_image_shape = origin_image_shape;
            ext->model_h = model_h;
            ext->model_w = model_w;

            bmcv_nms_yolo(handle, bottom_num, bottom, batch_num, hw_shape, num_classes, num_boxes, mask_group_size,
                        nms_threshold, conf_threshold, keep_top_k, bias, anchor_scale, mask,
                        bm_mem_from_system((void*)output_bmdnn), yolo_flag,
                        len_per_batch, (void*)ext);

            bm_dev_free(handle);
            free(ext);
            delete[] data_bottom[0];
            delete[] data_bottom[1];
            delete[] data_bottom[2];
            delete[] output_bmdnn;
            return 0;
        }