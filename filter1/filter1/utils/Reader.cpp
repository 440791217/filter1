#include "Reader.h"


Reader::Reader()
{
	//int rc = read_png("resource/lena.png", NULL);
	//printf("rc:%d\r\n", rc);
}


Reader::~Reader()
{
}

int Reader::read_data(char *filepath, struct_pic_info *out) {

	if (filepath==NULL) {
		srand((unsigned)time(NULL));/*播种子*/
		INT32 frame_size;
		INT16 *data;
		INT16 **data2d;
		int test_image_size = out->test_image_size;
		if (test_image_size == READER_TEST_IMAGE_256_256) {
			out->height = 256;
			out->width = 256;
		}
		else if (test_image_size == READER_TEST_IMAGE_128_128) {
			out->height = 128;
			out->width = 128;
		}
		else if (test_image_size == READER_TEST_IMAGE_64_64) {
			out->height = 64;
			out->width = 64;
		}
		else if ((test_image_size == READER_TEST_IMAGE_16_16)) {
			out->height = 16;
			out->width = 16;
		}
		else {
			Msg_Info("invalid test image size!\r\n");
			return COMMON_RC_ERR;
		}
		/*one dimension
		frame_size = out->width*out->height;
		data= (INT16*)malloc(sizeof(INT16)*frame_size);
		for (int i = 0; i < out->height; i++) {
		for (int j = 0; j < out->width; j++) {
		INT32 pos = i * out->width + j;
		INT16 value = rand() % out->value_range;
		data[pos] = value;
		}
		}
		out->data = data;
		*/
		data2d= (INT16**)malloc(sizeof(INT16*)*out->height);
		for (int i = 0; i < out->height; i++) {
			data2d[i]= (INT16*)malloc(sizeof(INT16)*out->width);
		}

		for (int i = 0; i < out->height; i++) {
			for (int j = 0; j < out->width; j++) {
				INT16 value = rand() % out->value_range;
				data2d[i][j] = value;
				//printf("%d,", data2d[i][j]);
			}
		}

		out->data2d = data2d;
	}
	else {

	}

	return READER_RC_OK;
}

int Reader::destory(struct_pic_info *out) {

	free(out->data);
	out->data = NULL;
	return READER_RC_OK;
}

/*
int Reader::read_png(char *filepath, pic_data *out) {
	int rc=RC_OK;
	FILE *fp;
	int err=fopen_s(&fp,filepath, "rb");
	const int PNG_BYTES_TO_CHECK = 8;
	char png_header[8];
	int iRetVal;
	if (err!=0)
	{
		return (RC_NOT_EXIST);
	}
	fread(png_header, 1, PNG_BYTES_TO_CHECK, fp);
	int is_png = !png_sig_cmp((png_const_bytep)png_header, 0, PNG_BYTES_TO_CHECK);
	if (!is_png)
	{
		rc=RC_NOT_PNG;
		goto end;
	}

	png_structp png_ptr = NULL;//libpng的结构体
	png_infop   info_ptr = NULL;//libpng的信息

	png_ptr = png_create_write_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
	if (!png_ptr) {
		rc = RC_PNG_STRUCT_NULL;
		goto end;
	}

	info_ptr = png_create_info_struct(png_ptr);
	if (!info_ptr)
	{
		rc = RC_PNG_INFO_NULL;
		png_destroy_write_struct(&png_ptr, NULL);
		goto end;
	}

	iRetVal = setjmp(png_jmpbuf(png_ptr));//安装错误处理跳转点
										  //当libpng内部出现错误的时候，libpng会调用longjmp直接跳转到这里运行。
	if (iRetVal)//setjmp的返回值就是libpng跳转后提供的错误代码（貌似总是1，但是还是请大家看libpng的官方文档）
	{
		rc = RC_PNG_INFO_NULL;
		fprintf(stderr, "PNG 内部 错误码：%d\n", iRetVal);
		goto end;
	}

	png_init_io(png_ptr, fp);
	png_read_info(png_ptr, info_ptr);
	end:
	return rc;
};
*/