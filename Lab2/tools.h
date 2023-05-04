#ifndef TOOLS_H
#define TOOLS_H
#include <stdbool.h>
#include <regex.h>
#define ERROR_SIZE 256


//非法ip判断
bool get_IP_legal(regex_t *ipreg, const char *ip);

// ip转换成int 进行非法判断
unsigned int ip_int(char *ip);

#endif