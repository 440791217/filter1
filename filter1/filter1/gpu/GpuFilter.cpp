#include "GpuFilter.h"

__global__ void do_filter(struct_gpu_filter_info filter_info);
__device__ int sort(INT16* arr, int N);
__device__ int mean(INT16* arr, int N);
__device__ int gauss(INT16 * arr, int side, double sig);
__device__ int inject_err_kn(struct_filter_rand_mode* mode);

GpuFilter::GpuFilter()
{
}


GpuFilter::~GpuFilter()
{
}

int GpuFilter::run_gpu(struct_gpu_filter_info *info) {

	Msg_Info("run_gpu start\r\n");
	INT16 height = info->height;
	INT16 width = info->width;
	INT64 frame_data_size = height * width;
	struct_gpu_filter_info filter_info = *info;
	INT16 tail_width, tail_height;

	Msg_Info("filter info-height:%d,width:%d,frame_data_size:%d\r\n", height, width, frame_data_size);

	{
		INT16 **src2d = NULL, **dst2d = NULL;
		Msg_Info("i:%d,src:%u,dst:%u\r\n", 0, src2d, dst2d);
		cudaMalloc((void **)&(src2d), sizeof(INT16*)*height);
		cudaMalloc((void **)&(dst2d), sizeof(INT16*)*height);
		filter_info.src2d = src2d;
		filter_info.dst2d = dst2d;
		Msg_Info("i:%d,src:%u,dst:%u\r\n", 0, src2d, dst2d);
		for (int i = 0; i < height; i++) {
			cudaError_t rc=cudaSuccess;
			Msg_Info("rc:%d\r\n", rc);
			rc=cudaMalloc((void **)&src2d[i], sizeof(INT16)*width);
			if (rc != cudaSuccess) {
				Msg_Info("1 rc:%d\r\n", rc);
			}
			rc=cudaMalloc((void **)&dst2d[i], sizeof(INT16)*width);
			if (rc != cudaSuccess) {
				Msg_Info("2 rc:%d\r\n", rc);
			}
			rc=cudaMemcpy(filter_info.src2d[i], info->src2d[i], sizeof(INT16)*width, cudaMemcpyHostToDevice);
			if (rc != cudaSuccess) {
				Msg_Info("3 rc:%d\r\n", rc);
			}
		}

		// 将原始图像从主机端复制到设备端
		Msg_Info("5\r\n");
		Msg_Info("i:%d,local src:%u,local dst:%u\r\n", 0, filter_info.src2d, filter_info.dst2d);
		Msg_Info("4\r\n");
	}

	//for (int i = 0; i < info->height; i++){
	//for (int j = 0; j < info->width; j++) {
	//int pos = i * info->width + j;
	//int value = info->src[pos];
	//Msg_Info("%5d,", value);
	//}
	//Msg_Info("\r\n");
	//}
	if (info->tail_mode == GPU_FILTER_TMODE_16_16) {
		tail_width = 16;
		tail_height = 16;
	}
	else {
		tail_width = 32;
		tail_height = 32;
	}
	filter_info.tail_width = tail_width;
	filter_info.tail_height = tail_height;

	int heightBlocks = (height + tail_height - 1) / (tail_height);
	int widthBlocks = (width + tail_width - 1) / (tail_width);

	Msg_Info("tail_height:%d,tail_width:%d,heightBlocks:%d,widthBlocks:%d\r\n", tail_height, tail_width, heightBlocks, widthBlocks);

	dim3 blocks(heightBlocks, widthBlocks);
	dim3 threads(tail_width, tail_height);

	Msg_Info("start filter\r\n");
	do_filter << <blocks, threads >> > (filter_info);
	Msg_Info("complete filter\r\n");

	Msg_Info("11\r\n");
	for (int i = 0; i < height; i++) {
		cudaMemcpy(info->dst2d[i], filter_info.dst2d[i], sizeof(INT16)*width, cudaMemcpyDeviceToHost);
	}


	//for (int i = 0; i < info->height; i++) {
	//	for (int j = 0; j < info->width; j++) {
	//		int pos = i * info->width + j;
	//		int value = info->dst[pos];
	//		Msg_Info("%5d,", value);
	//	}
	//	Msg_Info("\r\n");
	//}

	Msg_Info("111\r\n");
	for (int i = 0; i < height; i++) {
		cudaFree(filter_info.src2d[i]);
		cudaFree(filter_info.dst2d[i]);
	}
	cudaFree(filter_info.src2d);
	cudaFree(filter_info.dst2d);


	return COMMON_RC_OK;
}

__global__ void do_filter(struct_gpu_filter_info filter_info) {
	//Msg_Info("123\r\n");
	struct_gpu_filter_info* info = &filter_info;
	//Msg_Info("1234\r\n");
	INT16 tail_height = info->tail_height;
	INT16 tail_width = info->tail_width;
	INT16 src_height = info->height;
	INT16 src_width = info->width;
	INT16 row = blockIdx.x*tail_height + threadIdx.x;
	INT16 col = blockIdx.y*tail_width + threadIdx.y;
	INT64 pos = row * src_width + col;
	INT16 arr[100];
	INT16 window_mode = info->window_mode;
	//Msg_Info("12345\r\n");
	INT16** src2d = info->src2d;
	INT16** dst2d = info->dst2d;
	INT16 N = 0;
	INT16 side_length = 0;
	INT16 value_range = info->value_range;
	INT16 filter_mode = info->filter_mode;
	char inject_mode = info->inject_mode;
	char inject_mode_ex = info->inject_mode_extension;
	char tail_r;
	//Msg_Info("1234567890\r\n");

	Msg_ENABLE_XYZ_0()
		Msg_Info("src_height:%d,src_width:%d,tail_height:%d,tail_width:%d\r\n", src_height, src_width, tail_height, tail_width);
	//assign value
	//*(dst + pos) = *(src + pos);
	dst2d[row][col] = src2d[row][col];
	//printf("bx:%d,by:%d.bz:%d,tx:%d,ty:%d,tz:%d,dst value:%d,src:%d\r\n",blockIdx.x,blockIdx.y,blockIdx.z,threadIdx.x,threadIdx.y,threadIdx.z, *(dst + pos), *(src + pos));
	//return;
	Msg_ENABLE_XYZ_0()
		printf("test2\r\n");
	if (window_mode == GPU_FILTER_WMODE_ACROSS) {
		side_length = 3;
	}
	else if (window_mode == GPU_FILTER_WMODE_3_3) {
		side_length = 3;
	}
	else if (window_mode == GPU_FILTER_WMODE_5_5) {
		side_length = 5;
	}
	else if (window_mode == GPU_FILTER_WMODE_7_7) {
		side_length = 7;
	}
	tail_r = side_length / 2;
	Msg_ENABLE_XYZ_0()
		printf("test3\r\n");
	INT16 border_L = tail_r - 1, border_R = src_width - tail_r, border_T = tail_r - 1, border_B = src_height - tail_r;

	if (col > border_L&&col<border_R&&row>border_T&&row < border_B) {

		if (window_mode == GPU_FILTER_WMODE_ACROSS) {
			arr[0] = src2d[row - 1][col];//*(src + pos - src_width);
			arr[1] = src2d[row][col-1];// *(src + pos - 1);
			arr[2] = src2d[row][col];// *(src + pos);
			arr[3] = src2d[row][col + 1];// *(src + pos + 1);
			arr[4] = src2d[row + 1][col];//*(src + pos + src_width);
			N = 5;
		}
		else {
			for (int i = 0; i < side_length; i++) {
				for (int j = 0; j < side_length; j++) {
					int index = i * side_length + j;
					int idy = row + (i - tail_r);
					int idx = col + (j - tail_r);
					arr[index] = src2d[idy][idx];//*(src + idy * src_width + idx);
				}
			}
			N = side_length * side_length;
		}

		if (inject_mode == GPU_FILTER_INJECT_KN) {
			struct_filter_rand_mode mode;
			mode.x = pos;
			mode.y = src2d[row][col];
			if (mode.y == 0) {
				mode.y += 1;
			}
			mode.z = mode.x%mode.y;
			mode.maxlength = N;
			mode.value_range = value_range;
			mode.data = arr;
			inject_err_kn(&mode);
		}

		if (filter_mode == GPU_FILTER_MODE_MID) {
			sort(arr, N);
			//assign value
			//Msg_Info("arr[N/2]:%d\r\n", arr[N / 2]);
			//*(dst + pos) = arr[N / 2];
			dst2d[row][col] = arr[N / 2];
			//	Msg_Info("arr[N/2]:%d,*dst:%d\r\n", arr[N / 2], *(dst + pos));
		}
		else if (filter_mode == GPU_FILTER_MODE_MEAN) {
			mean(arr, N);
			dst2d[row][col] = arr[N / 2];
		}
		else if (filter_mode == GPU_FILTER_MODE_GAUSS) {

			if (window_mode == GPU_FILTER_WMODE_ACROSS) {
				gauss(arr, -1, 1);
			}
			else {
				gauss(arr, side_length, 1);
			}
			dst2d[row][col] = arr[N / 2];
		}


	}
}

__device__ int sort(INT16* arr, int N) {
	INT16 t;
	for (int i = 0; i < N; i++) {
		for (int j = 0; j < N - i - 1; j++) {

			if (arr[j] > arr[j + 1]) {
				t = arr[j];
				arr[j] = arr[j + 1];
				arr[j + 1] = t;
			}
		}
	}
	return COMMON_RC_OK;
}

__device__ int mean(INT16* arr, int N) {
	INT16 sum = 0;
	for (int i = 0; i < N; i++) {
		sum += arr[i];
	}

	arr[N / 2] = sum / N;

	return COMMON_RC_OK;
}

__device__ int gauss(INT16 * arr, int side, double sig) {
	double arr1[100];
	double sig2 = sig * sig;
	double h_sum = 0;
	double sum = 0;
	int N;
	if (side == -1) {
		N = 5;
		double h1 = (1 / (2 * 3.14*sig2));
		double h2 = (1 / (2 * 3.14*sig2)*exp(-(1) / (2 * sig2)));
		h_sum = 4 * h2 + h1;
		for (int i = 0; i < N; i++) {
			double h;
			if (i == (N / 2 + 1)) {
				h = h1;
			}
			else {
				h = h2;
			}
			h = h / h_sum;
			sum += arr[i] * h;
		}
	}
	else {
		int n = side / 2;
		for (int i = 0; i < side; i++) {
			for (int j = 0; j < side; j++) {
				int index = i * side + j;
				int x = i - n;
				int y = j - n;
				double x2 = x * x;
				double y2 = y * y;
				double h = (1 / (2 * 3.14*sig2)*exp(-(x2 + y2) / (2 * sig2)));
				h_sum += h;
				arr1[index] = h;
			}
		}

		for (int i = 0; i < side; i++) {
			for (int j = 0; j < side; j++) {
				int index = i * side + j;
				double h = arr1[index];
				sum += arr[index] = (h / h_sum)*arr[index];

			}
		}
		N = side * side;
	}
	arr[N / 2] = sum;



	return COMMON_RC_OK;
}

__device__ int inject_err_kn(struct_filter_rand_mode* mode) {

	curandState state0, state1;
	curand_init(mode->x, mode->y, 0, &state0);
	int random0 = curand(&state0);
	curand_init(mode->y, mode->z, 0, &state1);
	int random1 = curand(&state1);
	int error = (abs(random0) % mode->value_range);
	int index = (abs(random1) % mode->maxlength);
	mode->data[index] = error;
	//Msg_Info("index:%d,error:%d\r\n", index, error);
	mode->data[3] = 128;
	return COMMON_RC_OK;
}
