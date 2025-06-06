bmcv_faiss_indexflatL2
======================

计算查询向量与数据库向量 L2 距离的平方, 输出前 K （sort_cnt）个最匹配的 L2 距离的平方值及其对应的索引。


**处理器型号支持：**

该接口仅支持BM1684X。


**接口形式：**

    .. code-block:: c++

        bm_status_t bmcv_faiss_indexflatL2(
                    bm_handle_t handle,
                    bm_device_mem_t input_data_global_addr,
                    bm_device_mem_t db_data_global_addr,
                    bm_device_mem_t query_L2norm_global_addr,
                    bm_device_mem_t db_L2norm_global_addr,
                    bm_device_mem_t buffer_global_addr,
                    bm_device_mem_t output_sorted_similarity_global_addr,
                    bm_device_mem_t output_sorted_index_global_addr,
                    int  vec_dims,
                    int query_vecs_num,
                    int database_vecs_num,
                    int sort_cnt,
                    int is_transpose,
                    int input_dtype,
                    int output_dtype);


**输入参数说明：**

* bm_handle_t handle

  输入参数。bm_handle 句柄。

* bm_device_mem_t input_data_global_addr

  输入参数。存放查询向量组成的矩阵的 device 空间。

* bm_device_mem_t db_data_global_addr

  输入参数。存放底库向量组成的矩阵的 device 空间。

* bm_device_mem_t query_L2norm_global_addr

  输入参数。存放计算出的内积值的缓存空间。

* bm_device_mem_t db_L2norm_global_addr

  输入参数。数据库向量L2范数的 device 空间。

* bm_device_mem_t buffer_global_addr

  输入参数。缓存 device 空间。

* bm_device_mem_t output_sorted_similarity_global_addr

  输出参数。存放排序后的最匹配的内积值的 device 空间。

* bm_device_mem_t output_sorted_index_global_addr

  输出参数。存储输出内积值对应索引的 device 空间。

* int vec_dims

  输入参数。向量维数。

* int query_vecs_num

  输入参数。查询向量的个数。

* int database_vecs_num

  输入参数。底库向量的个数。

* int sort_cnt

  输入参数。输出的前 sort_cnt 个最匹配的 L2 距离的平方值。

* int is_transpose

  输入参数。0 表示底库矩阵不转置; 1 表示底库矩阵转置。

* int input_dtype

  输入参数。输入数据类型，仅支持 float, 5 表示float。

* int output_dtype

  输出参数。输出数据类型，仅支持 float, 5 表示float。


**返回值说明:**

* BM_SUCCESS: 成功

* 其他: 失败


**注意事项：**

1、输入数据(查询向量)和底库数据(底库向量)的数据类型为 float。

2、输出的排序后的相似度结果的数据类型为 float, 相对应的索引的数据类型为 int。

3、假设输入数据和底库数据的 L2 范数的平方值已提前计算完成, 并存储在处理器上。

3、底库数据通常以 database_vecs_num * vec_dims 的形式排布在内存中。此时, 参数 is_transpose 需要设置为 1。

5、查询向量和数据库向量 L2 距离的平方值越小, 表示两者的相似度越高。因此, 在 TopK 过程中对 L2 距离的平方值按升序排序。

6、该接口用于 Faiss::IndexFlatL2.search(), 在 BM1684X 上实现。考虑 BM1684X 上 Tensor Computing Processor 的连续内存, 针对 100W 底库, 可以在单处理器上一次查询最多约 512 个 256 维的输入。

7、database_vecs_num与sort_cnt的取值需要满足条件：database_vecs_num > sort_cnt。

8、faiss系列算子有多个输入参数，每个参数都有一个使用范围限制，超过该范围的入参对应tpu输出会出错，我们选择了三个主要参数做了测试，固定其中两个维度，测试了第三个维度的最大值，测试结果如下表格所示：

+-----------+--------------+-------------------+
| query_num | vec_dims     | max_database_num  |
+===========+==============+===================+
| 1         | 128          | 800万             |
+-----------+--------------+-------------------+
| 1         | 256          | 400万             |
+-----------+--------------+-------------------+
| 1         | 512          | 200万             |
+-----------+--------------+-------------------+
| 64        | 128          | 500万             |
+-----------+--------------+-------------------+
| 64        | 256          | 300万             |
+-----------+--------------+-------------------+
| 64        | 512          | 180万             |
+-----------+--------------+-------------------+
| 128       | 128          | 400万             |
+-----------+--------------+-------------------+
| 128       | 256          | 200万             |
+-----------+--------------+-------------------+
| 128       | 512          | 150万             |
+-----------+--------------+-------------------+
| 256       | 128          | 200万             |
+-----------+--------------+-------------------+
| 256       | 256          | 150万             |
+-----------+--------------+-------------------+
| 256       | 512          | 100万             |
+-----------+--------------+-------------------+

+--------------+--------------+----------------+
| database_num | vec_dims     | max_query_num  |
+==============+==============+================+
| 1000         | 128          | 128            |
+--------------+--------------+----------------+
| 1000         | 256          | 128            |
+--------------+--------------+----------------+
| 1000         | 512          | 128            |
+--------------+--------------+----------------+
| 1万          | 128          | 128            |
+--------------+--------------+----------------+
| 1万          | 256          | 128            |
+--------------+--------------+----------------+
| 1万          | 512          | 128            |
+--------------+--------------+----------------+
| 10万         | 128          | 50             |
+--------------+--------------+----------------+
| 10万         | 256          | 50             |
+--------------+--------------+----------------+

+--------------+-----------------+--------------+
| database_num | query_num       | max_vec_dims |
+==============+=================+==============+
| 1万          | 1               | 8192         |
+--------------+-----------------+--------------+
| 1万          | 64              | 4096         |
+--------------+-----------------+--------------+
| 1万          | 128             | 4096         |
+--------------+-----------------+--------------+
| 1万          | 256             | 4096         |
+--------------+-----------------+--------------+
| 10万         | 1               | 4096         |
+--------------+-----------------+--------------+
| 10万         | 64              | 4096         |
+--------------+-----------------+--------------+


**示例代码**

    .. code-block:: c++

        #include <math.h>
        #include <stdio.h>
        #include <stdlib.h>
        #include <stdint.h>
        #include "bmcv_api_ext_c.h"
        #include "test_misc.h"
        #include <string.h>

        void matrix_gen_data(float* data, u32 len)
        {
            for (u32 i = 0; i < len; i++) {
                data[i] = ((float)rand() / (float)RAND_MAX) * 3.3;
            }
        }

        void fvec_norm_L2sqr_ref(float* vec, float* matrix, int row_num, int col_num)
        {
            for (int i = 0; i < row_num; i++) {
                for (int j = 0; j < col_num; j++) {
                    vec[i] += matrix[i * col_num + j] * matrix[i * col_num + j];
                }
            }
        }

        int main()
        {
            int sort_cnt = 100;
            int vec_dims = 256;
            int query_vecs_num = 1;
            int database_vecs_num = 20000;
            int is_transpose = 1;
            int input_dtype = 5; // 5: float
            int output_dtype = 5;
            float* input_data = new float[query_vecs_num * vec_dims];
            float* db_data = new float[database_vecs_num * vec_dims];
            float* vec_query = new float[1 * query_vecs_num];
            float* vec_db = new float[1 * database_vecs_num];
            bm_handle_t handle;
            bm_device_mem_t query_data_dev_mem;
            bm_device_mem_t db_data_dev_mem;
            bm_device_mem_t query_L2norm_dev_mem;
            bm_device_mem_t db_L2norm_dev_mem;
            float* output_dis = new float[query_vecs_num * sort_cnt];
            int* output_inx = new int[query_vecs_num * sort_cnt];
            bm_device_mem_t buffer_dev_mem;
            bm_device_mem_t sorted_similarity_dev_mem;
            bm_device_mem_t sorted_index_dev_mem;

            matrix_gen_data(input_data, query_vecs_num * vec_dims);
            matrix_gen_data(db_data, vec_dims * database_vecs_num);
            fvec_norm_L2sqr_ref(vec_query, input_data, query_vecs_num, vec_dims);
            fvec_norm_L2sqr_ref(vec_db, db_data, database_vecs_num, vec_dims);

            bm_dev_request(&handle, 0);
            bm_malloc_device_byte(handle, &query_data_dev_mem, query_vecs_num * vec_dims * sizeof(float));
            bm_malloc_device_byte(handle, &db_data_dev_mem, database_vecs_num * vec_dims * sizeof(float));
            bm_malloc_device_byte(handle, &query_L2norm_dev_mem, 1 * query_vecs_num * sizeof(float));
            bm_malloc_device_byte(handle, &db_L2norm_dev_mem, 1 * database_vecs_num * sizeof(float));

            bm_memcpy_s2d(handle, query_data_dev_mem, input_data);
            bm_memcpy_s2d(handle, db_data_dev_mem, db_data);
            bm_memcpy_s2d(handle, query_L2norm_dev_mem, vec_query);
            bm_memcpy_s2d(handle, db_L2norm_dev_mem, vec_db);


            bm_malloc_device_byte(handle, &buffer_dev_mem, query_vecs_num * database_vecs_num * sizeof(float));
            bm_malloc_device_byte(handle, &sorted_similarity_dev_mem, query_vecs_num * sort_cnt * sizeof(float));
            bm_malloc_device_byte(handle, &sorted_index_dev_mem, query_vecs_num * sort_cnt * sizeof(int));

            bmcv_faiss_indexflatL2(handle, query_data_dev_mem, db_data_dev_mem, query_L2norm_dev_mem,
                                db_L2norm_dev_mem, buffer_dev_mem, sorted_similarity_dev_mem,
                                sorted_index_dev_mem, vec_dims, query_vecs_num, database_vecs_num,
                                sort_cnt, is_transpose, input_dtype, output_dtype);

            bm_memcpy_d2s(handle, output_dis, sorted_similarity_dev_mem);
            bm_memcpy_d2s(handle, output_inx, sorted_index_dev_mem);

            delete[] input_data;
            delete[] db_data;
            delete[] vec_query;
            delete[] vec_db;
            delete[] output_dis;
            delete[] output_inx;
            bm_free_device(handle, query_data_dev_mem);
            bm_free_device(handle, db_data_dev_mem);
            bm_free_device(handle, query_L2norm_dev_mem);
            bm_free_device(handle, db_L2norm_dev_mem);
            bm_free_device(handle, buffer_dev_mem);
            bm_free_device(handle, sorted_similarity_dev_mem);
            bm_free_device(handle, sorted_index_dev_mem);
            bm_dev_free(handle);
            return 0;
        }