#pragma once
#include<debuglog.h>
#include<string.h>
#include<const.h>
#include <windows.h>
#include <time.h>  

#ifndef _READER_H_
#define _READER_H_
const int READER_RC_OK = 0;
const int READER_RC_ERR_MD_TEST = -1;
const int READER_MODE_TEST = 1;
const int READER_MODE_NORMAL = 2;

const int READER_TEST_IMAGE_256_256 = 1;
const int READER_TEST_IMAGE_128_128 = 2;
const int READER_TEST_IMAGE_64_64 = 3;
const int READER_TEST_IMAGE_16_16 = 4;
/******************************图片数据*********************************/
struct struct_pic_info
{
	INT16 width, height; /* 尺寸 */
	int bit_depth;  /* 位深 */
	int flag;   /* 一个标志，表示是否有alpha通道 */
	int mode;
	int test_image_size;
	INT16 *data;
	INT16** data2d;
	INT16 value_range;
};

class Reader
{
public:
	Reader();
	~Reader();
	int read_data(char *filepath, struct_pic_info *out);
	int destory(struct_pic_info *out);
	//int read_png(char *filepath, pic_data *out);
};
#endif // !_READER_H_



