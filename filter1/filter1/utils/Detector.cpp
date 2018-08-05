#include<Detector.h>

Detector::Detector() {

}

Detector::~Detector() {

}

int Detector::dmr_detect(struct_detect_info* detect_info) {

	INT32 frame_datasize = detect_info->frame_datasize;
	int rundancy = detect_info->rundancy;
	INT16*  dst0 = detect_info->dst[0];
	INT16*  dst1 = detect_info->dst[1];
	INT16* check_arr = (INT16*)malloc(sizeof(INT16)*frame_datasize);
	memset(check_arr, 0, sizeof(INT16)*frame_datasize);
	for (INT32 pos = 0; pos < frame_datasize; pos++) {
		INT16 value = DETECT_EQUAL;
		if (*(dst0 + pos) == *(dst1 + pos)) {
			value = DETECT_EQUAL;
			detect_info->num_equal++;
		}
		else {
			value = DETECT_NOT_EQUAL;
			detect_info->num_not_equal++;
		}
		*(check_arr + pos) = value;
	}

	detect_info->pecentage_equal = ((double)detect_info->num_equal) / frame_datasize;
	detect_info->pecentage_not_equal = ((double)detect_info->num_not_equal) / frame_datasize;
	free(check_arr);
	check_arr = NULL;
	return COMMON_RC_OK;
}