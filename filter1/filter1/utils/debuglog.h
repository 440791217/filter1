#pragma once

#ifndef _DEBUGLOG_H
#define _DEBUGLOG_H
#include<stdio.h>
#define ESC_START     "\033["
#define ESC_END       "\033[0m"
#define COLOR_FATAL   "31;40;5m"
#define COLOR_ALERT   "31;40;1m"
#define COLOR_CRIT    "31;40;1m"
#define COLOR_ERROR   "31;40;1m"
#define COLOR_WARN    "33;40;1m"
#define COLOR_NOTICE  "34;40;1m"
#define COLOR_INFO    "32;40;1m"
#define COLOR_DEBUG   "36;40;1m"
#define COLOR_TRACE   "37;40;1m"

//#define Msg_Info(format, ...) (printf( ESC_START COLOR_INFO "[INFO]-[%s]-[%s]-[%d]:" format ESC_END, __FILE__, __FUNCTION__ , __LINE__, __VA_ARGS__))
//#define Msg_Debug(format, ...) (printf( ESC_START COLOR_DEBUG "[DEBUG]-[%s]-[%s]-[%d]:" format ESC_END, __FILE__, __FUNCTION__ , __LINE__, ##...))
//#define Msg_Warn(format,...) (printf( ESC_START COLOR_WARN "[WARN]-[%s]-[%s]-[%d]:" format ESC_END, __FILE__, __FUNCTION__ , __LINE__, ##...))
//#define Msg_Error(format,...) (printf( ESC_START COLOR_ERROR "[ERROR]-[%s]-[%s]-[%d]:" format ESC_END, __FILE__, __FUNCTION__ , __LINE__, ##...))
const int LOGGER_RC_OK = 0;
#define Msg_Info(format, ...) (printf(format,__VA_ARGS__))
#define Msg_Debug(format, ...) (printf(format,__VA_ARGS__))
#define Msg_ENABLE_XYZ_0() if (blockIdx.x == 0 && blockIdx.y == 0&&threadIdx.x==0&&threadIdx.y==0&&blockIdx.z==0&&threadIdx.z==0)

#endif
