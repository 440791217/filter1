#include "Logger.h"



Logger::Logger()
{
}


Logger::~Logger()
{
}

int Logger::init(string dirname, string filename) {

	string filepath;
	if (access(dirname.c_str(), 0) == 0) {
		printf("目录存在");
	}
	else {
		printf("目录不存在");
		mkdir(dirname.c_str());
	}

	filepath = dirname + "//" + filename;
	fcout.open(filepath, ios::in|ios::out|ios::trunc);

	return LOGGER_RC_OK;
}

int Logger::write(string msg) {
	fcout << msg;
	return LOGGER_RC_OK;
}

int Logger::br() {
	fcout << endl;
	return LOGGER_RC_OK;
}



int Logger::destroy() {
	fcout.close();
	return LOGGER_RC_OK;
}