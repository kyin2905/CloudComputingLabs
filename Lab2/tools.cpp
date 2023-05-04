#include <stdio.h>
#include <errno.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>
#include <string.h>
#include <arpa/inet.h>
#include <fcntl.h>
#include <dirent.h>
#include <sys/stat.h>
#include <ctype.h>
#include <sys/socket.h>
#include "tools.h"


static char errbuf[ERROR_SIZE] = {0};

//合法ip的正则
//static const char *ip_format = "(25[0-4]|2[0-4][0-9]|1[0-9][0-9]|[1-9][0-9]|[1-9])[.](25[0-5]|2[0-4][0-9]|1[0-9][0-9]|[1-9][0-9]|[0-9])[.](25[0-5]|2[0-4][0-9]|1[0-9][0-9]|[1-9][0-9]|[0-9])[.](25[0-4]|2[0-4][0-9]|1[0-9][0-9]|[1-9][0-9]|[1-9])";
static const char *ip_format = "^([0-9]|[1-9][0-9]|1[0-9]{1,2}|2[0-4][0-9]|25[0-4]).([0-9]|[1-9][0-9]|1[0-9]{1,2}|2[0-4][0-9]|25[0-5]).([0-9]|[1-9][0-9]|1[0-9]{1,2}|2[0-4][0-9]|25[0-5]).([1-9]|[1-9][0-9]|1[0-9]{1,2}|2[0-4][0-9]|25[0-5])$";

bool get_IP_legal(regex_t *ipreg, const char *ip){
	regmatch_t pmatch[1];
	const size_t nmatch = 1;
	int status = regexec(ipreg, ip, nmatch, pmatch, 0);
	if(status == 0) {
		//printf("Match Successful!\n");
        return true;
	}
	else if(status == REG_NOMATCH) {
		regerror(status, ipreg, errbuf, ERROR_SIZE);	
		//printf("%s\n", errbuf);
        return false;
		memset(errbuf, 0, ERROR_SIZE);
	}
}
// ip转换成int 进行非法判断
unsigned int ip_int(char *ip)
{   
    //编译正则
    regex_t ipreg1;
	int reg = regcomp(&ipreg1, ip_format, REG_EXTENDED);
	if(reg != 0) {
		regerror(reg, &ipreg1, errbuf, ERROR_SIZE);	
		printf("%s\n", errbuf);
		memset(errbuf, 0, ERROR_SIZE);
		return 0;
	}
    if(!get_IP_legal(&ipreg1 ,ip)){
        printf("Illegal IP!! Please Check!\n");
        return __INT32_MAX__;
    }
    unsigned int re = 0;
    unsigned char tmp = 0;
    //printf("%s\n", ip);
    //printf("%d\n", strlen(ip));
    while (1) {
        if (*ip != '\0' && *ip != '.') {
            tmp = tmp * 10 + *ip - '0';
        } else {
            re = (re << 8) + tmp;
            if (*ip == '\0')
                break;
            tmp = 0;
        }
        ip++;
    }
    return re;
}


