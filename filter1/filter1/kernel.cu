#include "cuda_runtime.h"
#include "device_launch_parameters.h"
#include <test.h>
#include <filter_sys.h>
#include <Reader.h>
#include <debuglog.h>
#include <Logger.h>
#include <GpuFilter.h>
#include <Detector.h>
using namespace std;

__global__ void test(FilterSys filterSys);
bool debug = true;
bool print2local = false;

Logger logger;
int main()
{
	char tail_mode = GPU_FILTER_TMODE_16_16, window_mode = GPU_FILTER_WMODE_5_5;
	char inject_mode = GPU_FILTER_INJECT_KN, inject_mode_extension = GPU_FILTER_INJECT_EX_K1;
	char rundancy = 2, inejct_channels = 1,filter_mode=GPU_FILTER_MODE_GAUSS;
	char pic_mode = READER_MODE_TEST;
	int pic_test_image = READER_TEST_IMAGE_256_256;
	INT16 *dst_arr[10];
	INT16 **dst_arr2d[10];
	INT16 *check_arr;
	INT32 frame_datasize;
	struct_gpu_filter_info gpu_filter_info_arr[10];
	struct_detect_info detect_info;

	if (window_mode == GPU_FILTER_WMODE_3_3) {

	}
	else if (window_mode == GPU_FILTER_WMODE_5_5) {

	}
	else if (window_mode == GPU_FILTER_WMODE_7_7) {

	}
	else if (window_mode == GPU_FILTER_WMODE_ACROSS) {

	}
	else {
		Msg_Info("invalid window mode!\r\n");
		return COMMON_RC_ERR;
	}

	if (tail_mode == GPU_FILTER_TMODE_8_8) {

	}
	else if (tail_mode == GPU_FILTER_TMODE_16_16) {

	}
	else if (tail_mode == GPU_FILTER_TMODE_32_32) {

	}
	else {
		Msg_Info("invalid tail mode!\r\n");
		return COMMON_RC_ERR;
	}

	if (inject_mode == GPU_FILTER_INJECT_NONE) {

	}
	else if (inject_mode == GPU_FILTER_INJECT_KN) {

	}
	else {
		Msg_Info("invalid inject mode!\r\n");
		return COMMON_RC_ERR;
	}
	if (inejct_channels >= rundancy) {
		Msg_Info("invalid inject channels!\r\n");
		return COMMON_RC_ERR;
	}
	logger.init("simulation", "log");
	Reader reader;
	//init test matrix
	struct_pic_info pic_info;
	pic_info.height = 256;
	pic_info.width = 256;
	pic_info.value_range = 256;
	if (pic_mode == READER_MODE_TEST) {
		pic_info.mode = READER_MODE_TEST;
		pic_info.test_image_size = READER_TEST_IMAGE_256_256;
		reader.read_data(NULL, &pic_info);
	}

	if (print2local) {
		logger.write("random matrix");
		logger.br();
		for (int i = 0; i < pic_info.height; i++) {
			for (int j = 0; j < pic_info.width; j++) {
				INT16 value = pic_info.data2d[i][j];
				char buffer[10];
				sprintf(buffer, "%3d", value);
				logger.write(buffer);
				logger.write(",");
			}
			logger.br();
		}
	}
	//return 0;

	frame_datasize = pic_info.width*pic_info.height;
	for (int i = 0; i < rundancy; i++) {
		GpuFilter filter;
		struct_gpu_filter_info *gpu_filter_info = &gpu_filter_info_arr[i];
		gpu_filter_info->width = pic_info.width;
		gpu_filter_info->height = pic_info.height;
		gpu_filter_info->src2d = (INT16**)malloc(sizeof(INT16*)*pic_info.height);
		gpu_filter_info->dst2d = (INT16**)malloc(sizeof(INT16*)*pic_info.height);
		for (int j = 0; j < gpu_filter_info->height; j++) {
			gpu_filter_info->src2d[j] = (INT16*)malloc(sizeof(INT16)*gpu_filter_info->width);
			gpu_filter_info->dst2d[j] = (INT16*)malloc(sizeof(INT16)*gpu_filter_info->width);
			memcpy(gpu_filter_info->src2d[j], pic_info.data2d[j], gpu_filter_info->width * sizeof(INT16));
		}
		gpu_filter_info->filter_mode = GPU_FILTER_MODE_MID;
		gpu_filter_info->value_range = pic_info.value_range;
		gpu_filter_info->filter_mode = filter_mode;
		gpu_filter_info->window_mode = window_mode;
		gpu_filter_info->tail_mode = tail_mode;
		gpu_filter_info->inject_mode_extension = inject_mode_extension;
		gpu_filter_info->rundancy = rundancy;

		if (i < inejct_channels) {
			gpu_filter_info->inject_mode = inject_mode;
		}
		else {
			gpu_filter_info->inject_mode = GPU_FILTER_INJECT_NONE;
		}
		int rc = filter.run_gpu(gpu_filter_info);
		dst_arr2d[i] = gpu_filter_info->dst2d;
		Msg_Info("filter%d rc:%d\r\n", i, rc);
	}
	return 0;


	for (int i = 0; i < rundancy; i++) {
		struct_gpu_filter_info *gpu_filter_info = &gpu_filter_info_arr[i];
		detect_info.dst[i] = gpu_filter_info->dst;
		detect_info.frame_datasize = frame_datasize;
	}


	if (print2local) {
		logger.write("filtered matrix");
		logger.br();
		for (int i = 0; i < pic_info.height; i++) {
			for (int j = 0; j < pic_info.width; j++) {
				int pos = i * pic_info.width + j;
				INT16 value = *(dst_arr[0] + pos);
				char buffer[10];
				sprintf(buffer, "%3d", value);
				logger.write(buffer);
				logger.write(",");
			}
			logger.br();
		}

	}

	Detector detector;
	detector.dmr_detect(&detect_info);
	{
		string errmode="null";
		if (inject_mode == GPU_FILTER_INJECT_KN) {
			if (inject_mode_extension == GPU_FILTER_INJECT_EX_K1) {
				errmode = "K1";
			}
			else if (inject_mode_extension == GPU_FILTER_INJECT_EX_K2) {
				errmode = "K2";
			}
			else if (inject_mode_extension == GPU_FILTER_INJECT_EX_K3) {
				errmode = "K3";
			}

		}
		string window_info = "";
		if (window_mode == GPU_FILTER_WMODE_ACROSS) {
			window_info = "across_window,item_counts:5,";
		}
		else if (window_mode == GPU_FILTER_WMODE_3_3) {
			window_info = "3_3_window,item_counts:9,";
		}
		else if (window_mode == GPU_FILTER_WMODE_5_5) {
			window_info = "5_5_window,item_counts:25,";
		}
		logger.write("filter info");
		logger.br();
		string msg1 = "height:"+to_string(pic_info.height)+",width:"+to_string(pic_info.width)+",errormode:"+errmode+",window_info:"+window_info;
		logger.write(msg1);
		logger.br();
		Msg_Info("%s\r\n", msg1.c_str());
		logger.write("detect arr");
		logger.br();
		string msg2 ="total pixels:"+to_string(detect_info.frame_datasize)+",positive nums:"+to_string(detect_info.num_not_equal)+",negative:"+to_string(detect_info.num_equal)+ string(",pecentage equal:") + to_string(detect_info.pecentage_equal) + ",not equal:" + to_string(detect_info.pecentage_not_equal);
		logger.write(msg2);
		logger.br();
		Msg_Info("%s\r\n", msg2.c_str());
	}

	for (int i = 0; i < rundancy; i++) {
		struct_gpu_filter_info *gpu_filter_info = &gpu_filter_info_arr[i];
		free(gpu_filter_info->src);
		free(gpu_filter_info->dst);
	}

	reader.destory(&pic_info);

	return 0;
}

__global__ void test(FilterSys filterSys) {
	printf("%d\r\n", filterSys.FRAME_WIDTH);
}

