bmcv_faiss_indexflatL2
======================

This interface is used to calculate squared L2 distance between query vectors and database vectors, output the top K (sort_cnt) L2sqr-values and the corresponding indices, return BM_SUCCESS if succeed.


**Processor model support:**

This interface only supports BM1684X.


**Interface form:**

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
                    int vec_dims,
                    int query_vecs_num,
                    int database_vecs_num,
                    int sort_cnt,
                    int is_transpose,
                    int input_dtype,
                    int output_dtype);


**Input parameter description:**

* bm_handle_t handle

  Input parameter. The handle of bm_handle.

* bm_device_mem_t input_data_global_addr

  Input parameter. Device addr information of the query matrix.

* bm_device_mem_t db_data_global_addr

  Input parameter. Device addr information of the database matrix.

* bm_device_mem_t query_L2norm_global_addr

  Input parameter. Device addr information of the query norm_L2sqr vector.

* bm_device_mem_t db_L2norm_global_addr

  Input parameter. Device addr information of the database norm_L2sqr vector.

* bm_device_mem_t buffer_global_addr

  Input parameter. Squared L2 values stored in the buffer.

* bm_device_mem_t output_sorted_similarity_global_addr

  Output parameter. The L2sqr-values matrix.

* bm_device_mem_t output_sorted_index_global_addr

  Output parameter. The result indices matrix.

* int vec_dims

  Input parameter. Vector dimension.

* int query_vecs_num

  Input parameter. The num of query vectors.

* int database_vecs_num

  Input parameter. The num of database vectors.

* int sort_cnt

  Input parameter. Get top sort_cnt values.

* int is_transpose

  Input parameter. db_matrix 0: NO_TRNAS; 1: TRANS.

* int input_dtype

  Input parameter. Only support float, 5 means float.

* int output_dtype

  Output parameter. Only support float, 5 means float.


**Return value description:**

* BM_SUCCESS: success

* Other: failed


**Note:**

1. The input data type (query vectors) and data in the database (database vectors) are float.

2. The data type of the output sorted similarity result is float, and that of the corresponding indices is int.

3. The assumption is that the norm_L2sqr values of the input data and the database data have been computed ahead of time and stored on the processor.

4. Usually, the data in the database is arranged in the memory as database_vecs_num * vec_dims. Therefore, the is_transpose needs to be set to 1.

5. The smaller the squared L2 values of the query vector and the database vector, the higher the similarity of the two vectors. Therefore, the squared L2 values are sorted in ascending order in the process of TopK.

6. The interface is used for Faiss::IndexFlatL2.search() and implemented on BM1684X. According to the continuous memory of Tensor Computing Processor on BM1684X, we can query about 512 inputs of 256 dimensions at a time on a single processor if the database is about 100W.

7. the value of database_vecs_num and sort_cnt needs to meet the condition: database_vecs_num > sort_cnt.

8. The Faiss series operators have multiple input parameters, each of which has a usage range limit. If the input parameter exceeds the range, the corresponding TPU output will fail. We selected three main parameters for testing, fixed two of the dimensions, and tested the maximum value of the third dimension. The test results are shown in the following table:

+-----------+--------------+-------------------+
| query_num | vec_dims     | max_database_num  |
+===========+==============+===================+
| 1         | 128          | 8million          |
+-----------+--------------+-------------------+
| 1         | 256          | 4million          |
+-----------+--------------+-------------------+
| 1         | 512          | 2million          |
+-----------+--------------+-------------------+
| 64        | 128          | 5million          |
+-----------+--------------+-------------------+
| 64        | 256          | 3million          |
+-----------+--------------+-------------------+
| 64        | 512          | 180million        |
+-----------+--------------+-------------------+
| 128       | 128          | 4million          |
+-----------+--------------+-------------------+
| 128       | 256          | 2million          |
+-----------+--------------+-------------------+
| 128       | 512          | 1.5million        |
+-----------+--------------+-------------------+
| 256       | 128          | 2million          |
+-----------+--------------+-------------------+
| 256       | 256          | 1.5million        |
+-----------+--------------+-------------------+
| 256       | 512          | 1million          |
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
| 10k          | 128          | 128            |
+--------------+--------------+----------------+
| 10k          | 256          | 128            |
+--------------+--------------+----------------+
| 10k          | 512          | 128            |
+--------------+--------------+----------------+
| 100k         | 128          | 50             |
+--------------+--------------+----------------+
| 100k         | 256          | 50             |
+--------------+--------------+----------------+

+--------------+-----------------+--------------+
| database_num | query_num       | max_vec_dims |
+==============+=================+==============+
| 10k          | 1               | 8192         |
+--------------+-----------------+--------------+
| 10k          | 64              | 4096         |
+--------------+-----------------+--------------+
| 10k          | 128             | 4096         |
+--------------+-----------------+--------------+
| 10k          | 256             | 4096         |
+--------------+-----------------+--------------+
| 100k         | 1               | 4096         |
+--------------+-----------------+--------------+
| 100k         | 64              | 4096         |
+--------------+-----------------+--------------+


**Sample code**

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