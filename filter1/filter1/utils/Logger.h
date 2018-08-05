#pragma once
#include<const.h>
#include<debuglog.h>
#include<io.h>
#include<direct.h>
#include <fstream>
#include<iostream>
#include <string>
using namespace std;

#ifndef _LOGGER_H_
#define _LOGGER_H_
class Logger
{
public:
	Logger();
	~Logger();
	int init(string dirname, string filename);
	int destroy();
	int write(string msg);
	int br();
private:
	ofstream fcout;
};
#endif // !_LOGGER_H_




