/*************************************************************************
	> File Name: accept.cpp
	> Author: ma6174
	> Mail: ma6174@163.com 
	> Created Time: 2016年06月27日 星期一 07时24分00秒
 ************************************************************************/

#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <assert.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>

int main()
{
	if(argc <= 2) {
		pritnf("usage:%s ip_address port_number"， basename(argv[0]));
		return 1;
	}
}

