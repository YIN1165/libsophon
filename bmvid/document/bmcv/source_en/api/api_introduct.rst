BMCV API
===============================
Briefly explain which part of the hardware implements the BMCV API

**The following interfaces BM1684X have not yet implemented:**

*  bmcv_image_canny
*  bmcv_image_dct
*  bmcv_image_draw_lines
*  bmcv_fft
*  bmcv_image_lkpyramid
*  bmcv_image_morph
*  bmcv_image_sobel

+-----+----------------------------------+-----------+-----------+
| num |         API                      |   BM1684  | BM1684X   |
+=====+==================================+===========+===========+
| 1   | bmcv_as_strided                  |NOT SUPPORT|   TPU     |
+-----+----------------------------------+-----------+-----------+
| 2   | bmcv_image_absdiff               |   TPU     |   TPU     |
+-----+----------------------------------+-----------+-----------+
| 3   | bmcv_image_add_weighted          |   TPU     |   TPU     |
+-----+----------------------------------+-----------+-----------+
| 4   | bmcv_base64                      |  SPACC    |  SPACC    |
+-----+----------------------------------+-----------+-----------+
| 5   | bmcv_image_bayer2rgb             |NOT SUPPORT|   TPU     |
+-----+----------------------------------+-----------+-----------+
| 6   | bmcv_image_bitwise_and           |   TPU     |   TPU     |
+-----+----------------------------------+-----------+-----------+
| 7   | bmcv_image_bitwise_or            |   TPU     |   TPU     |
+-----+----------------------------------+-----------+-----------+
| 8   | bmcv_image_bitwise_xor           |   TPU     |   TPU     |
+-----+----------------------------------+-----------+-----------+
| 9   | bmcv_calc_hist                   |   TPU     |   TPU     |
+-----+----------------------------------+-----------+-----------+
| 10  | bmcv_image_canny                 |   TPU     |   TPU     |
+-----+----------------------------------+-----------+-----------+
| 11  | bmcv_image_convert_to            |   TPU     |  VPP+TPU  |
+-----+----------------------------------+-----------+-----------+
| 12  | bmcv_image_copy_to               |   TPU     |  VPP+TPU  |
+-----+----------------------------------+-----------+-----------+
| 13  | bmcv_image_dct                   |   TPU     |   TPU     |
+-----+----------------------------------+-----------+-----------+
| 14  | bmcv_distance                    |   TPU     |   TPU     |
+-----+----------------------------------+-----------+-----------+
| 15  | bmcv_image_draw_lines            |   CPU     |   VPP     |
+-----+----------------------------------+-----------+-----------+
| 16  | bmcv_image_draw_rectangle        |   TPU     |   VPP     |
+-----+----------------------------------+-----------+-----------+
| 17  | bmcv_feature_match               |   TPU     |   TPU     |
+-----+----------------------------------+-----------+-----------+
| 18  | bmcv_fft                         |   TPU     |   TPU     |
+-----+----------------------------------+-----------+-----------+
| 19  | bmcv_image_fill_rectangle        |   TPU     |   VPP     |
+-----+----------------------------------+-----------+-----------+
| 20  | bmcv_image_gaussian_blur         |   TPU     |   TPU     |
+-----+----------------------------------+-----------+-----------+
| 21  | bmcv_gemm                        |   TPU     |   TPU     |
+-----+----------------------------------+-----------+-----------+
| 22  | bmcv_image_jpeg_enc              |   JPU     |   JPU     |
+-----+----------------------------------+-----------+-----------+
| 23  | bmcv_image_jpeg_dec              |   JPU     |   JPU     |
+-----+----------------------------------+-----------+-----------+
| 24  | bmcv_image_laplacian             |   TPU     |   TPU     |
+-----+----------------------------------+-----------+-----------+
| 25  | bmcv_matmul                      |   TPU     |   TPU     |
+-----+----------------------------------+-----------+-----------+
| 26  | bmcv_min_max                     |   TPU     |   TPU     |
+-----+----------------------------------+-----------+-----------+
| 27  | bmcv_nms_ext                     |   TPU     |   TPU     |
+-----+----------------------------------+-----------+-----------+
| 28  | bmcv_nms                         |   TPU     |   TPU     |
+-----+----------------------------------+-----------+-----------+
| 29  | bmcv_image_resize                |  VPP+TPU  |   VPP     |
+-----+----------------------------------+-----------+-----------+
| 30  | bmcv_image_sobel                 |   TPU     |   TPU     |
+-----+----------------------------------+-----------+-----------+
| 31  | bmcv_sort                        |   TPU     |   TPU     |
+-----+----------------------------------+-----------+-----------+
| 32  | bmcv_image_storage_convert       |  VPP+TPU  |   VPP     |
+-----+----------------------------------+-----------+-----------+
| 33  | bmcv_image_threshold             |   TPU     |   TPU     |
+-----+----------------------------------+-----------+-----------+
| 34  | bmcv_image_transpose             |   TPU     |   TPU     |
+-----+----------------------------------+-----------+-----------+
| 35  | bmcv_image_vpp_basic             |   VPP     |   VPP     |
+-----+----------------------------------+-----------+-----------+
| 36  | bmcv_image_vpp_convert_padding   |   VPP     |   VPP     |
+-----+----------------------------------+-----------+-----------+
| 37  | bmcv_image_vpp_convert           |   VPP     |   VPP     |
+-----+----------------------------------+-----------+-----------+
| 38  | bmcv_image_vpp_csc_matrix_convert|   VPP     |   VPP     |
+-----+----------------------------------+-----------+-----------+
| 39  | bmcv_image_vpp_stitch            |   VPP     |   VPP     |
+-----+----------------------------------+-----------+-----------+
| 40  | bmcv_image_warp_affine           |   TPU     |   TPU     |
+-----+----------------------------------+-----------+-----------+
| 41  | bmcv_image_warp_perspective      |   TPU     |   TPU     |
+-----+----------------------------------+-----------+-----------+
| 42  | bmcv_image_watermark_superpose   |NOT SUPPORT|   TPU     |
+-----+----------------------------------+-----------+-----------+
| 43  | bmcv_nms_yolo                    |   TPU     |   TPU     |
+-----+----------------------------------+-----------+-----------+
| 44  | bmcv_cmulp                       |   TPU     |   TPU     |
+-----+----------------------------------+-----------+-----------+
| 45  | bmcv_faiss_indexflatIP           |NOT SUPPORT|   TPU     |
+-----+----------------------------------+-----------+-----------+
| 46  | bmcv_faiss_indexflatL2           |NOT SUPPORT|   TPU     |
+-----+----------------------------------+-----------+-----------+
| 47  | bmcv_image_yuv2bgr_ext           |   TPU     |   VPP     |
+-----+----------------------------------+-----------+-----------+
| 48  | bmcv_image_yuv2hsv               |   TPU     |  VPP+TPU  |
+-----+----------------------------------+-----------+-----------+
| 49  | bmcv_batch_topk                  |   TPU     |   TPU     |
+-----+----------------------------------+-----------+-----------+
| 50  | bmcv_image_put_text              |   CPU     |   CPU     |
+-----+----------------------------------+-----------+-----------+
| 51  | bmcv_hm_distance                 |NOT SUPPORT|   TPU     |
+-----+----------------------------------+-----------+-----------+
| 52  | bmcv_axpy                        |    TPU    |   TPU     |
+-----+----------------------------------+-----------+-----------+
| 53  | bmcv_image_pyramid_down          |    TPU    |   TPU     |
+-----+----------------------------------+-----------+-----------+
| 54  | bmcv_image_quantify              |NOT SUPPORT|   TPU     |
+-----+----------------------------------+-----------+-----------+
| 55  | bmcv_image_rotate                |NOT SUPPORT|   TPU     |
+-----+----------------------------------+-----------+-----------+
| 56  | bmcv_cos_similarity              |NOT SUPPORT|   TPU     |
+-----+----------------------------------+-----------+-----------+
| 57  | bmcv_matrix_prune                |NOT SUPPORT|   TPU     |
+-----+----------------------------------+-----------+-----------+
| 58  | bmcv_median_blur                 |NOT SUPPORT|   TPU     |
+-----+----------------------------------+-----------+-----------+
| 59  | bmcv_assigned_area_blur          |NOT SUPPORT|   TPU     |
+-----+----------------------------------+-----------+-----------+
| 60  | bmcv_image_crop                  |    TPU    |  TPU+VPP  |
+-----+----------------------------------+-----------+-----------+
| 61  | bmcv_image_split                 |    TPU    |   TPU     |
+-----+----------------------------------+-----------+-----------+
| 62  | bmcv_hamming_distance            |NOT SUPPORT|   TPU     |
+-----+----------------------------------+-----------+-----------+
| 63  | bmcv_image_yuv_resize            |    TPU    |  TPU+VPP  |
+-----+----------------------------------+-----------+-----------+
| 64  | bmcv_width_align                 |    TPU    |   TPU     |
+-----+----------------------------------+-----------+-----------+
| 65  | bmcv_faiss_indexPQ_ADC           |NOT SUPPORT|   TPU     |
+-----+----------------------------------+-----------+-----------+
| 66  | bmcv_faiss_indexPQ_SDC           |NOT SUPPORT|   TPU     |
+-----+----------------------------------+-----------+-----------+
| 67  | bmcv_faiss_indexPQ_encode        |NOT SUPPORT|   TPU     |
+-----+----------------------------------+-----------+-----------+
| 68  | bmcv_image_axpy                  |NOT SUPPORT|   TPU     |
+-----+----------------------------------+-----------+-----------+
| 69  | bmcv_image_fusion                |    TPU    |NOT SUPPORT|
+-----+----------------------------------+-----------+-----------+
| 70  | bmcv_calc_hist_with_weight       |    TPU    |   TPU     |
+-----+----------------------------------+-----------+-----------+
| 71  | bmcv_hist_balance                |    TPU    |   TPU     |
+-----+----------------------------------+-----------+-----------+
| 72  | bmcv_lap_matrix                  |NOT SUPPORT|   TPU     |
+-----+----------------------------------+-----------+-----------+
| 73  | bmcv_stft                        |NOT SUPPORT|   TPU     |
+-----+----------------------------------+-----------+-----------+
| 74  | bmcv_istft                       |NOT SUPPORT|   TPU     |
+-----+----------------------------------+-----------+-----------+
| 75  | bmcv_image_rotate                |NOT SUPPORT|   TPU     |
+-----+----------------------------------+-----------+-----------+
| 76  | bmcv_cos_similarity              |NOT SUPPORT|   TPU     |
+-----+----------------------------------+-----------+-----------+
| 77  | bmcv_matrix_prune                |NOT SUPPORT|   TPU     |
+-----+----------------------------------+-----------+-----------+
| 78  | bmcv_image_overlay               |NOT SUPPORT|   TPU     |
+-----+----------------------------------+-----------+-----------+
| 79  | bmcv_get_structuring_element     |    CPU    |   CPU     |
+-----+----------------------------------+-----------+-----------+
| 80  | bmcv_image_erode                 |    TPU    |NOT SUPPORT|
+-----+----------------------------------+-----------+-----------+
| 81  | bmcv_image_dilate                |    TPU    |NOT SUPPORT|
+-----+----------------------------------+-----------+-----------+
| 82  | bmcv_image_lkpyramid_create_plan |    CPU    |   CPU     |
+-----+----------------------------------+-----------+-----------+
| 83  | bmcv_image_lkpyramid_execute     |    CPU    |NOT SUPPORT|
+-----+----------------------------------+-----------+-----------+
| 84  | bmcv_dct_coeff                   |NOT SUPPORT|   VPP     |
+-----+----------------------------------+-----------+-----------+
| 85  | bmcv_image_mosaic                |NOT SUPPORT|   VPP     |
+-----+----------------------------------+-----------+-----------+
| 86  | bmcv_gen_text_watermark          |    CPU    |   CPU     |
+-----+----------------------------------+-----------+-----------+
| 87  | bmcv_image_csc_convert_to        |    TPU    |   VPP+TPU |
+-----+----------------------------------+-----------+-----------+
| 88  | bmcv_image_vpp_basic_v2          |NOT SUPPORT|   VPP+TPU |
+-----+----------------------------------+-----------+-----------+
| 89  | bmcv_image_draw_point            |NOT SUPPORT|   VPP     |
+-----+----------------------------------+-----------+-----------+
| 90  | bmcv_knn                         |NOT SUPPORT|   TPU     |
+-----+----------------------------------+-----------+-----------+

**Note:**

For BM1684 and BM1684X, the implementation of the following two operators requires a combination of BMCPU and Tensor Computing Processor

+-----+----------------------------------+
| num |         API                      |
+=====+==================================+
| 1   | bmcv_image_lkpyramid             |
+-----+----------------------------------+
| 2   | bmcv_image_morph                 |
+-----+----------------------------------+










































































