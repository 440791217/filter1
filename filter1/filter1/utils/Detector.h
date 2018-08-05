#pragma once
#include<Windows.h>
#include<const.h>
#ifndef _DETECT_H_
#define _DETECT_H_

const int DETECT_EQUAL = 0;
const int DETECT_NOT_EQUAL = 1;

struct struct_detect_info {
	INT16* dst[10];
	int rundancy = 0;
	//INT16 *check_arr;
	INT32 frame_datasize;
	UINT32 num_equal = 0;
	UINT32 num_not_equal = 0;
	double pecentage_equal = 0;
	double pecentage_not_equal = 0;
};

class Detector
{
public:
	Detector();
	~Detector();
	int dmr_detect(struct_detect_info* detect_info);
private:

};

#endif // !_DETECT_H_
