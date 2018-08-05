#pragma once
#include <Windows.h>
#include <const.h>
#include "cuda_runtime.h"
#include "device_launch_parameters.h"
#include <debuglog.h>
#include "curand_kernel.h" 
#include <time.h>  
#ifndef _GPU_FILTER_H_
#define _GPU_FILTER_H_

const char GPU_FILTER_MODE_MEAN = 1;
const char GPU_FILTER_MODE_MID = 2;
const char GPU_FILTER_MODE_MAX = 3;
const char GPU_FILTER_MODE_MIN = 4;
const char GPU_FILTER_MODE_GAUSS = 5;
const char GPU_FILTER_TMODE_8_8 = 1;
const char GPU_FILTER_TMODE_16_16 = 2;
const char GPU_FILTER_TMODE_32_32 = 3;
const char GPU_FILTER_WMODE_ACROSS = 1;
const char GPU_FILTER_WMODE_3_3 = 2;
const char GPU_FILTER_WMODE_5_5 = 3;
const char GPU_FILTER_WMODE_7_7 = 4;
const char GPU_FILTER_INJECT_NONE = 0;
const char GPU_FILTER_INJECT_KN = 1;
const char GPU_FILTER_INJECT_EX_K1 = 1;
const char GPU_FILTER_INJECT_EX_K2 = 2;
const char GPU_FILTER_INJECT_EX_K3 = 3;

struct struct_gpu_filter_info {
	INT16 *src=NULL;
	INT16 *dst=NULL;
	INT16 **src2d = NULL;
	INT16 **dst2d = NULL;
	INT16 height;
	INT16 width;
	INT16 value_range;
	INT16 window_height;
	INT16 window_width;
	INT16 tail_height;
	INT16 tail_width;
	char rundancy = 1;
	char tail_mode = GPU_FILTER_TMODE_32_32;
	char window_mode = GPU_FILTER_WMODE_ACROSS;
	char filter_mode=GPU_FILTER_MODE_MID;
	char inject_mode= GPU_FILTER_INJECT_NONE;
	char inject_mode_extension;
};
struct struct_filter_rand_mode {
	INT32 x;
	INT32 y;
	INT32 z;
	char inject_mode_extension;
	INT16 maxlength;
	INT16 value_range;
	INT16 *data;
	INT16 **data2d;
};

class GpuFilter
{
public:
	GpuFilter();
	~GpuFilter();
	int run_gpu(struct_gpu_filter_info *filter_info);
};
#endif // !_GPU_FILTER_H_



