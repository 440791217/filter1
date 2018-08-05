#pragma once
#include<Windows.h>
#ifndef _FILTER_SYS_H
#define _FILTER_SYS_H


class FilterSys
{
public:
	FilterSys();
	~FilterSys();
	INT16 FRAME_HEIGHT=256, FRAME_WIDTH=256;
	INT16 WINDOW_TYPE;
	INT16 FILTER_TYPE;


private:

};

FilterSys::FilterSys()
{

}

FilterSys::~FilterSys()
{
}



#endif // !FILTER_SYS

