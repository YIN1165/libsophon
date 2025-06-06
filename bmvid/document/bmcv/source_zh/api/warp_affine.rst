bmcv_image_warp_affine
======================

该接口实现图像的仿射变换，可实现旋转、平移、缩放等操作。仿射变换是一种二维坐标 (:math:`x` , :math:`y`) 到二维坐标(:math:`x_0` , :math:`y_0`)的线性变换，该接口的实现是针对输出图像的每一个像素点找到在输入图像中对应的坐标，从而构成一幅新的图像，其数学表达式形式如下：

.. math::

    \left\{
    \begin{array}{c}
    x_0=a_1x+b_1y+c_1 \\
    y_0=a_2x+b_2y+c_2 \\
    \end{array}
    \right.

对应的齐次坐标矩阵表示形式为：

.. math::

     \left[\begin{matrix} x_0 \\ y_0 \\ 1 \end{matrix} \right]=\left[\begin{matrix} a_1&b_1&c_1 \\ a_2&b_2&c_2 \\ 0&0&1 \end{matrix} \right]\times \left[\begin{matrix} x \\ y \\ 1 \end{matrix} \right]



坐标变换矩阵是一个 6 点的矩阵，该矩阵是从输出图像坐标推导输入图像坐标的系数矩阵，可以通过输入输出图像上对应的 3 个点坐标来获取。在人脸检测中，通过获取人脸定位点来获取变换矩阵。

bmcv_affine_matrix 定义了一个坐标变换矩阵，其顺序为 float m[6] = {a1, b1, c1, a2, b2, c2}。
而 bmcv_affine_image_matrix 定义了一张图片里面有几个变换矩阵，通常来说一张图片有多个人脸时，会对应多个变换矩阵。

    .. code-block:: c

        typedef struct bmcv_affine_matrix_s{
                float m[6];
        } bmcv_warp_matrix;

        typedef struct bmcv_affine_image_matrix_s{
                bmcv_affine_matrix *matrix;
                int matrix_num;
        } bmcv_affine_image_matrix;


**处理器型号支持：**

该接口支持BM1684/BM1684X。


**接口形式一:**

    .. code-block:: c

        bm_status_t bmcv_image_warp_affine(
                    bm_handle_t handle,
                    int image_num,
                    bmcv_affine_image_matrix matrix[4],
                    bm_image* input,
                    bm_image* output,
                    int use_bilinear = 0);


**接口形式二:**

    .. code-block:: c

        bm_status_t bmcv_image_warp_affine_similar_to_opencv(
                    bm_handle_t handle,
                    int image_num,
                    bmcv_affine_image_matrix matrix[4],
                    bm_image* input,
                    bm_image* output,
                    int use_bilinear = 0);

本接口是对齐opencv仿射变换的接口， 该矩阵是从输入图像坐标推导输出图像坐标的系数矩阵。


**输入参数说明**

* bm_handle_t handle

  输入参数。输入的 bm_handle 句柄。

* int image_num

  输入参数。输入图片数，最多支持 4。

* bmcv_affine_image_matrix matrix[4]

  输入参数。每张图片对应的变换矩阵数据结构，最多支持 4 张图片。

* bm_image\* input

  输入参数。输入 bm_image，对于 1N 模式，最多 4 个 bm_image，对于 4N 模式，最多一个 bm_image。

* bm_image\* output

  输出参数。输出 bm_image，外部需要调用 bmcv_image_create 创建，建议用户调用 bmcv_image_attach 来分配 device memory。如果用户不调用 attach，则内部分配 device memory。对于输出 bm_image，其数据类型和输入一致，即输入是 4N 模式，则输出也是 4N 模式,输入 1N 模式，输出也是 1N 模式。所需要的 bm_image 大小是所有图片的变换矩阵之和。比如输入 1 个 4N 模式的 bm_image，4 张图片的变换矩阵数目为【3,0,13,5】，则共有变换矩阵 3+0+13+5=21，由于输出是 4N 模式，则需要(21+4-1)/4=6 个 bm_image 的输出。

* int use_bilinear

  输入参数。是否使用 bilinear 进行插值，若为 0 则使用 nearest 插值，若为 1 则使用 bilinear 插值，默认使用 nearest 插值。选择 nearest 插值的性能会优于 bilinear，因此建议首选 nearest 插值，除非对精度有要求时可选择使用 bilinear 插值。


**返回值说明:**

* BM_SUCCESS: 成功

* 其他: 失败


**注意事项**

1. 该接口所支持的 image_format 包括：

   +-----+------------------------+
   | num | image_format           |
   +=====+========================+
   |  1  | FORMAT_BGR_PLANAR      |
   +-----+------------------------+
   |  2  | FORMAT_RGB_PLANAR      |
   +-----+------------------------+
   |  3  | FORMAT_GRAY            |
   +-----+------------------------+

2. bm1684中该接口所支持的 data_type 包括：

   +-----+------------------------+
   | num | data_type              |
   +=====+========================+
   |  1  | DATA_TYPE_EXT_1N_BYTE  |
   +-----+------------------------+
   |  2  | DATA_TYPE_EXT_4N_BYTE  |
   +-----+------------------------+

3. bm1684X中该接口所支持的 data_type 包括：

   +-----+-----------------------+
   | num | data_type             |
   +=====+=======================+
   | 1   | DATA_TYPE_EXT_1N_BYTE |
   +-----+-----------------------+

4. 该接口的输入以及输出 bm_image 均支持带有 stride。

5. 要求该接口输入 bm_image 的 width、height、image_format 以及 data_type 必须保持一致。

6. 要求该接口输出 bm_image 的 width、height、image_format、data_type 以及 stride 必须保持一致。


**代码示例**

    .. code-block:: c

        #include "stdio.h"
        #include "stdlib.h"
        #include "string.h"
        #include <memory>
        #include <iostream>
        #include "bmcv_api_ext.h"

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
            bm_handle_t handle;
            int image_h = 1080;
            int image_w = 1920;
            int dst_h = 256;
            int dst_w = 256;
            int use_bilinear = 0;
            bmcv_affine_image_matrix matrix_image;
            bm_image src, dst;
            bmcv_affine_matrix* matrix_data = (bmcv_affine_matrix*)malloc(sizeof(bmcv_affine_matrix) * 1);
            unsigned char* src_data = new unsigned char[image_h * image_w * 3];
            unsigned char* res_data = new unsigned char[dst_h * dst_w * 3];
            const char *filename_src = "path/to/src";
            const char *filename_dst = "path/to/dst";

            readBin(filename_src, src_data, image_h * image_w * 3);
            matrix_image.matrix_num = 1;
            matrix_image.matrix = matrix_data;
            matrix_image.matrix->m[0] = 3.848430;
            matrix_image.matrix->m[1] = -0.02484;
            matrix_image.matrix->m[2] = 916.7;
            matrix_image.matrix->m[3] = 0.02;
            matrix_image.matrix->m[4] = 3.8484;
            matrix_image.matrix->m[5] = 56.4748;

            bm_dev_request(&handle, 0);
            bm_image_create(handle, image_h, image_w, FORMAT_BGR_PLANAR, DATA_TYPE_EXT_1N_BYTE, &src);
            bm_image_create(handle, dst_h, dst_w, FORMAT_BGR_PLANAR, DATA_TYPE_EXT_1N_BYTE, &dst);
            bm_image_copy_host_to_device(src, (void**)&src_data);
            bmcv_image_warp_affine(handle, 1, &matrix_image, &src, &dst, use_bilinear);
            bm_image_copy_device_to_host(dst, (void**)&res_data);
            writeBin(filename_dst, res_data, dst_h * dst_w * 3);

            bm_image_destroy(src);
            bm_image_destroy(dst);
            bm_dev_free(handle);
            delete[] src_data;
            delete[] res_data;
            free(matrix_data);
            return 0;
        }

bmcv_image_warp_affine_padding
==============================

**接口说明**

* 所有的使用方式均和上述的 bmcv_image_warp_affine 相同，仅仅改变了接口名字，具体的 padding zero 的接口名字如下：

**接口形式一:**

    .. code-block:: c

        bm_status_t bmcv_image_warp_affine_padding(
            bm_handle_t handle,
            int image_num,
            bmcv_affine_image_matrix matrix[4],
            bm_image *input,
            bm_image *output,
            int use_bilinear);

**接口形式一:**

    .. code-block:: c

        bm_status_t bmcv_image_warp_affine_similar_to_opencv_padding(
            bm_handle_t handle,
            int image_num,
            bmcv_affine_image_matrix matrix[4],
            bm_image *input,
            bm_image *output,
            int use_bilinear);

* 接口仅仅支持1684x

**代码示例说明**

* 同 bmcv_image_warp_affine 接口使用方式相同，只需要将接口名字换成 bmcv_image_warp_affine_padding 或 bmcv_image_warp_affine_similar_to_opencv_padding 即可。