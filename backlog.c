/*************************************************************************
	> File Name: backlog.c
	> Author: ma6174
	> Mail: ma6174@163.com 
	> Created Time: 2016年06月26日 星期日 11时08分25秒
 ************************************************************************/

#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <signal.h>
#include <unistd.h>
#include <stdlib.h>
#include <assert.h>
#include <stdio.h>
#include <string.h>

static bool stop = false;

static void handle_term(int sig)
{
	stop = true;
}

int main(int argc, char *argv[])
{
	sinal(SIGTERM，handle_term);  

	if(argc <= 3) {
		printf ("usage:%s ip_address port_number ");
	}
}

