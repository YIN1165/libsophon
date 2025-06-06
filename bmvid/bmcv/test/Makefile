##############################################################################
#
#    Copyright (c) 2016-2022 by Bitmain Technologies Inc. All rights reserved.
#
#    The material in this file is confidential and contains trade secrets
#    of Bitmain Technologies Inc. This is proprietary information owned by
#    Bitmain Technologies Inc. No part of this work may be disclosed,
#    reproduced, copied, transmitted, or used in any way for any purpose,
#    without the express written permission of Bitmain Technologies Inc.
#
##############################################################################
LDFLAGS = -L$(BMVID_ROOT)/3rdparty/libbmrt/lib/$(PRODUCTFORM)/

TPU_PERFORMANCE_DIR = $(TEST_BMCV_DIR)/tpu_operators_performance_test_case

TEST_BMCV_DIR = $(BMCV_SRC_DIR)/test

VPP_DIR ?= $(OUT_DIR)/vpp

BMCV_DIR ?= $(OUT_DIR)/bmcv

INCLUDE_DIR += -I$(TEST_BMCV_DIR)

INCLUDE_DIR += -I$(BMVID_ROOT)/3rdparty/libbmrt/include

ifeq ($(PRODUCTFORM), soc)
TURBO_JPEG_PATH =  $(BMVID_ROOT)/3rdparty/libjpeg-turbo/binary/lib/soc/
else ifeq ($(PRODUCTFORM), pcie_arm64)
TURBO_JPEG_PATH =  $(BMVID_ROOT)/3rdparty/libjpeg-turbo/binary/lib/soc/
else ifeq ($(PRODUCTFORM), pcie_mips64)
TURBO_JPEG_PATH =  $(BMVID_ROOT)/3rdparty/libjpeg-turbo/binary/lib/mips/
else ifeq ($(PRODUCTFORM), pcie_loongarch64)
TURBO_JPEG_PATH =  $(BMVID_ROOT)/3rdparty/libjpeg-turbo/binary/lib/loongarch64/
else ifeq ($(PRODUCTFORM), pcie_sw64)
TURBO_JPEG_PATH =  $(BMVID_ROOT)/3rdparty/libjpeg-turbo/binary/lib/sw64/
else ifeq ($(PRODUCTFORM), pcie_riscv64)
TURBO_JPEG_PATH =  $(BMVID_ROOT)/3rdparty/libjpeg-turbo/binary/lib/riscv64/
else  # pcie_x86
TURBO_JPEG_PATH =  $(BMVID_ROOT)/3rdparty/libjpeg-turbo/binary/lib/pcie/
endif

#TEST_BMCV_SRCS_CXX = $(wildcard $(TEST_BMCV_DIR)/*.cpp wildcard $(TEST_BMCV_DIR)/bm1684x/*.cpp)
TEST_BMCV_SRCS_CXX = $(TEST_BMCV_DIR)/test_bmodel/test_gaussian_blur_fp16_1080p_3channel.cpp  \
					 $(TEST_BMCV_DIR)/test_cv_assigned_area_blur.cpp  \
                     $(TEST_BMCV_DIR)/test_cv_median_blur.cpp  \
					 $(TEST_BMCV_DIR)/test_faiss_indexPQ_encode.cpp \
					 $(TEST_BMCV_DIR)/test_cv_surf.cpp \
					 $(TEST_BMCV_DIR)/test_cv_matrix_prune.cpp  \
					 $(TEST_BMCV_DIR)/test_cv_cluster.cpp  \
					 $(TEST_BMCV_DIR)/test_cv_cos_similarity.cpp  \
					 $(TEST_BMCV_DIR)/test_cv_overlay.cpp  \
					 $(TEST_BMCV_DIR)/test_cv_rotate.cpp  \
					 $(TEST_BMCV_DIR)/test_convert_to.cpp  \
					 $(TEST_BMCV_DIR)/test_csc_convert_to.cpp  \
                     $(TEST_BMCV_DIR)/test_cv_absdiff.cpp  \
					 $(TEST_BMCV_DIR)/test_cv_as_strided.cpp  \
					 $(TEST_BMCV_DIR)/test_cv_bayer2rgb.cpp  \
					 $(TEST_BMCV_DIR)/test_cv_add_weighted.cpp \
					 $(TEST_BMCV_DIR)/test_cv_axpy.cpp   \
					 $(TEST_BMCV_DIR)/test_cv_bitwise.cpp  \
					 $(TEST_BMCV_DIR)/test_cv_base64.cpp  \
					 $(TEST_BMCV_DIR)/test_cv_batch_topk.cpp \
					 $(TEST_BMCV_DIR)/test_cv_bgrsplit.cpp  \
					 $(TEST_BMCV_DIR)/test_cv_calc_hist.cpp \
					 $(TEST_BMCV_DIR)/test_cv_hist_balance.cpp \
					 $(TEST_BMCV_DIR)/test_cv_canny.cpp  \
					 $(TEST_BMCV_DIR)/test_cv_cmulp.cpp  \
					 $(TEST_BMCV_DIR)/test_cv_copy_to.cpp  \
					 $(TEST_BMCV_DIR)/test_cv_copy_to_param.cpp  \
					 $(TEST_BMCV_DIR)/test_cv_crop.cpp  \
					 $(TEST_BMCV_DIR)/test_cv_dct.cpp  \
					 $(TEST_BMCV_DIR)/test_cv_distance.cpp \
  					 $(TEST_BMCV_DIR)/test_cv_draw_lines.cpp  \
  					 $(TEST_BMCV_DIR)/test_cv_draw_rectangle.cpp \
  					 $(TEST_BMCV_DIR)/test_cv_feature_match.cpp \
  					 $(TEST_BMCV_DIR)/test_cv_fft_1d.cpp \
  					 $(TEST_BMCV_DIR)/test_cv_fft_2d.cpp \
					 $(TEST_BMCV_DIR)/test_cv_stft.cpp \
					 $(TEST_BMCV_DIR)/test_cv_istft.cpp \
  					 $(TEST_BMCV_DIR)/test_cv_fill_rectangle.cpp \
					 $(TEST_BMCV_DIR)/test_cv_fusion.cpp   \
  					 $(TEST_BMCV_DIR)/test_cv_gaussian_blur.cpp  \
  					 $(TEST_BMCV_DIR)/test_cv_gemm.cpp  \
  					 $(TEST_BMCV_DIR)/test_cv_image_align.cpp  \
  					 $(TEST_BMCV_DIR)/test_cv_image_transpose.cpp   \
  					 $(TEST_BMCV_DIR)/test_cv_img_scale.cpp   \
  					 $(TEST_BMCV_DIR)/test_cv_jpeg.cpp   \
  					 $(TEST_BMCV_DIR)/test_cv_json.cpp   \
					 $(TEST_BMCV_DIR)/test_cv_knn.cpp\
  					 $(TEST_BMCV_DIR)/test_cv_laplacian.cpp   \
					 $(TEST_BMCV_DIR)/test_cv_lapmatrix.cpp   \
  					 $(TEST_BMCV_DIR)/test_cv_lkpyramid.cpp   \
					 $(TEST_BMCV_DIR)/test_cv_matmul_t_opt.cpp   \
  					 $(TEST_BMCV_DIR)/test_cv_matmul.cpp  \
  					 $(TEST_BMCV_DIR)/test_cv_min_max.cpp \
  					 $(TEST_BMCV_DIR)/test_cv_morph.cpp   \
  					 $(TEST_BMCV_DIR)/test_cv_multi_crop_resize.cpp  \
  					 $(TEST_BMCV_DIR)/test_cv_nms.cpp   \
  					 $(TEST_BMCV_DIR)/test_cv_put_text.cpp   \
  					 $(TEST_BMCV_DIR)/test_cv_pyramid.cpp   \
					 $(TEST_BMCV_DIR)/test_cv_qr_cpu.cpp\
					 $(TEST_BMCV_DIR)/test_cv_quantify.cpp  \
  					 $(TEST_BMCV_DIR)/test_cv_sobel.cpp   \
  					 $(TEST_BMCV_DIR)/test_cv_sort.cpp   \
  					 $(TEST_BMCV_DIR)/test_cv_split.cpp  \
  					 $(TEST_BMCV_DIR)/test_cv_storage_convert.cpp   \
  					 $(TEST_BMCV_DIR)/test_cv_threshold.cpp  \
  					 $(TEST_BMCV_DIR)/test_cv_transpose.cpp  \
  					 $(TEST_BMCV_DIR)/test_cv_vpp.cpp  \
  					 $(TEST_BMCV_DIR)/test_cv_vpp_border.cpp  \
  					 $(TEST_BMCV_DIR)/test_cv_vpp_loop.cpp   \
  					 $(TEST_BMCV_DIR)/test_cv_vpp_random.cpp  \
  					 $(TEST_BMCV_DIR)/test_cv_vpp_stitch.cpp  \
  					 $(TEST_BMCV_DIR)/test_cv_warp.cpp  \
					 $(TEST_BMCV_DIR)/test_cv_warp_affine.cpp  \
					 $(TEST_BMCV_DIR)/test_cv_warp_bilinear.cpp  \
					 $(TEST_BMCV_DIR)/test_cv_warp_perspective.cpp  \
					 $(TEST_BMCV_DIR)/test_cv_warp_perspective_bilinear.cpp  \
					 $(TEST_BMCV_DIR)/test_cv_width_align.cpp  \
					 $(TEST_BMCV_DIR)/test_cv_yuv2hsv.cpp \
					 $(TEST_BMCV_DIR)/test_cv_yuv2rgb.cpp  \
					 $(TEST_BMCV_DIR)/test_faiss_indexflatIP.cpp  \
					 $(TEST_BMCV_DIR)/test_faiss_indexflatL2.cpp  \
					 $(TEST_BMCV_DIR)/test_faiss_indexPQ.cpp  \
					 $(TEST_BMCV_DIR)/test_perf_bmcv.cpp  \
					 $(TEST_BMCV_DIR)/test_perf_vpp.cpp  \
					 $(TEST_BMCV_DIR)/test_resize.cpp  \
					 $(TEST_BMCV_DIR)/test_cv_hm_distance.cpp  \
					 $(TEST_BMCV_DIR)/test_yolov3_detect_out.cpp  \
					 $(TEST_BMCV_DIR)/test_matrix_log.cpp  \
					 $(TEST_BMCV_DIR)/bm1684x/test_bm1684x_fbd.cpp  \
					 $(TEST_BMCV_DIR)/bm1684x/test_bm1684x_fbd_param.cpp  \
					 $(TEST_BMCV_DIR)/bm1684x/test_bm1684x_slt.cpp   \
					 $(TEST_BMCV_DIR)/bm1684x/test_bm1684x_vpp.cpp   \
					 $(TEST_BMCV_DIR)/bm1684x/test_bm1684x_vpp_ax+b.cpp   \
					 $(TEST_BMCV_DIR)/bm1684x/test_bm1684x_vpp_basic.cpp  \
					 $(TEST_BMCV_DIR)/bm1684x/test_bm1684x_vpp_border.cpp     \
					 $(TEST_BMCV_DIR)/bm1684x/test_bm1684x_vpp_random.cpp   \
					 $(TEST_BMCV_DIR)/bm1684x/test_bm1684x_vpp_resize.cpp   \
					 $(TEST_BMCV_DIR)/bm1684x/test_bm1684x_vpp_stitch.cpp  \
					 $(TEST_BMCV_DIR)/bm1684x/test_bm1684x_vpp_stitch_2way.cpp  \
					 $(TEST_BMCV_DIR)/bm1684x/test_bm1684x_vpp_mosaic.cpp   \
					 $(TEST_BMCV_DIR)/bm1684x/test_bm1684x_vpp_convert.cpp    \
					 $(TEST_BMCV_DIR)/bm1684x/test_bm1684x_vpp_dst_position.cpp  \
					 $(TEST_BMCV_DIR)/bm1684x/test_bm1684x_vpp_comparison.cpp    \
					 $(TEST_BMCV_DIR)/bm1684x/test_bm1684x_vpp_border_comapre.cpp \
					 $(TEST_BMCV_DIR)/bm1684x/test_bm1684x_vpp_fill_rectangle.cpp  \
					 $(TEST_BMCV_DIR)/bm1684x/test_bm1684x_vpp_postpadding-compare.cpp  \
					 $(TEST_BMCV_DIR)/bm1684x/test_bm1684x_vpp_postpadding-haikang.cpp    \
					 $(TEST_BMCV_DIR)/bm1684x/test_bm1684x_vpp_watermark_superpose.cpp     \
					 $(TEST_BMCV_DIR)/bm1684x/test_bm1684x_vpp_convert_to_comparison.cpp   \
					 $(TEST_BMCV_DIR)/bm1684x/test_bm1684x_vpp_mul_crop_resize_haikang.cpp  \
			 		 $(TEST_BMCV_DIR)/bm1684x/test_bm1684x_vpp_point.cpp\
			 		 $(TEST_BMCV_DIR)/bm1684x/test_bm1684x_vpp_multi_thread.cpp\
			 		 $(TEST_BMCV_DIR)/bm1684x/test_bm1684x_gen_text_watermark.cpp\
					 $(TPU_PERFORMANCE_DIR)/rotate_performance.cpp\

TEST_BMCV_CXX_EXE1 = $(patsubst %.cpp,%,$(TEST_BMCV_SRCS_CXX))
TEST_BMCV_CXX_EXE2 = $(patsubst $(BMVID_ROOT)/%,%,$(TEST_BMCV_CXX_EXE1))
TEST_BMCV_CXX_EXE ?= $(addprefix $(OUT_DIR)/,$(TEST_BMCV_CXX_EXE2))
TEST_BMCV_CXX_EXE_TEMP = $(addprefix $(OUT_DIR)/,$(TEST_BMCV_CXX_EXE2))

TEST_BMCV_OBJ_DIR = $(sort $(dir $(TEST_BMCV_CXX_EXE)))
INSTALL_EXES += $(TEST_BMCV_CXX_EXE)
DAILY_TEST_EXES += $(TEST_BMCV_CXX_EXE)

BMLIB_DYN =  $(BMVID_ROOT)/3rdparty/libbmcv/lib/$(PRODUCTFORM)/libbmlib.so
VPP_DYN =  $(BMVID_ROOT)/3rdparty/libbmcv/lib/$(PRODUCTFORM)/libvpp_cmodel.so

$(TEST_BMCV_OBJ_DIR):
	@$(MKDIR) $@

INCLUDE_DIR += -I$(BMVID_ROOT)/bmcv/src/bm1684x/
INCLUDE_DIR += -I$(BMVID_ROOT)/bmcv/src/

TEST_BMCV_DYN_LDFLAGS =
ifeq ($(USING_OPENBLAS),1)
TEST_BMCV_DYN_LDFLAGS         += -L$(OPENBLAS_DIR) -lopenblas
ifeq ($(PRODUCTFORM), pcie)
TEST_BMCV_DYN_LDFLAGS         += -L$(BMVID_ROOT)/3rdparty/OpenBLAS/x86 -lopenblas
else ifeq ($(PRODUCTFORM),soc)
TEST_BMCV_DYN_LDFLAGS         += -L$(BMVID_ROOT)/3rdparty/OpenBLAS/arm -lopenblas
else
    $(error Unsupported PRODUCTFORM: $(PRODUCTFORM))
endif
endif

ifeq ($(USING_CMODEL),1)
INCLUDE_DIR += -I$(BMVID_ROOT)/bmcv/include

TEST_BMCV_LDFLAGS = $(LDFLAGS) -L$(BMCV_DIR)/lib/cmodel/ \
 -l:libbmcv.a -lbmlib \
 -L$(BMVID_ROOT)/3rdparty/libbmcv/lib/pcie/pcie_cmodel/ \
-ldl

$(TEST_BMCV_CXX_EXE): $(OUT_DIR)/%:$(BMVID_ROOT)/%.cpp $(BMLIB_RUNTIME_STATIC) | $(VPP_DYN) | $(TEST_BMCV_OBJ_DIR)
	@echo cc -o $@ $<
	@$(CXX) $(INCLUDE_DIR) $(CXXFLAGS) -Wextra -o $@ $< $(TEST_BMCV_DYN_LDFLAGS) $(TEST_BMCV_LDFLAGS) -std=c++0x -pthread

else
TEST_BMCV_STATIC = -std=c++0x -pthread

INCLUDE_DIR += -I$(BMVID_ROOT)/bmcv/include \
			   -I$(BMVID_ROOT)/vpp/driver/include/bm1684 \
			   -I$(BMVID_ROOT)/vpp/driver/include/ \
			   -I$(BMVID_ROOT)/jpeg/driver/bmjpuapi/inc \
			   -I$(BMVID_ROOT)/allocator/ion/inc

TEST_BMCV_LDFLAGS = $(LDFLAGS) -L$(JPU_DIR)/lib/ -L$(BMION_DIR)/lib/ \
					-L$(OUT_DIR)/lib/ -L$(BMCV_DIR)/lib/ -L$(VPP_DIR)/lib/ \
					-L$(BMVID_ROOT)/3rdparty/libbmcv/lib/$(PRODUCTFORM) \
					-L$(TURBO_JPEG_PATH) \
					-lbmjpuapi -lbmjpulite -lbmion -lbmcv -lbmlib -lvpp -lturbojpeg -lvpp_cmodel -ldl -lbmrt\

TEST_MISC_CXX = $(TEST_BMCV_DIR)/test_misc.cpp $(TEST_BMCV_DIR)/md5.cpp

$(TEST_BMCV_CXX_EXE): $(OUT_DIR)/%:$(BMVID_ROOT)/%.cpp $(BMLIB_DYN) | $(TEST_BMCV_OBJ_DIR)
	@echo cc -o $@ $<
	@$(CXX) $(INCLUDE_DIR)  $(CXXFLAGS) -o $@ $< $(TEST_MISC_CXX) $(TEST_BMCV_DYN_LDFLAGS) $(TEST_BMCV_LDFLAGS) $(TEST_BMCV_STATIC)
endif

test_bmcv: $(TEST_BMCV_CXX_EXE) $(BMCV_CPU_LIB)
	@echo $(BMCV_CPU_LIB) "this is bmcpu lib"
	@cp -rf $(BMCV_SRC_DIR)/test/image $(OUT_DIR)/bmcv/test/

clean_test:
	@echo rm -rf $(OUT_DIR)/bmcv/test/
	@rm -rf $(OUT_DIR)/bmcv/test/
	@rm -rf $(TEST_BMCV_OBJ_DIR)

BM_TEST_BMCV_DEP = $(patsubst %.cpp,%.d,$(TEST_BMCV_SRCS_CXX))
-include $(BM_TEST_BMCV_DEP)
