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
	signal(SIGTERM,handle_term);  

	if(argc <= 3) {
		printf ("usage:%s ip_address port_number ", basename(argv[0]));
		return 0;
	}
	const char *ip = argv[1];
	int port = atoi( argv[2] );
	int backlog = atoi( argv[3] );

	//创建套接字
	int sock = socket( PF_INET, SOCK_STREAM, 0 );
	assert( socket >= 0);

	//创建一个IPV4 socket地址
	struct sockaddr_in address;
	bzero ( &address, sizeof(address) );
	address.sin_family = AF_INET;
	inet_pton(AF_INET, ip, &address.sin_addr);
	address.sin_port = htons( port );

	//套接字绑定地址族
	int ret = bind(sock, (struct sockaddr*)&address, sizeof(address));
	assert(ret != -1);

	//创建监听队列
	ret = listen(sock, backlog);
	assert(ret != -1);

	//循环等待连接，直到SIGTERM信号中断它
	while( !stop) {
		sleep(1);
	}

	//关闭套接字
	close(sock);

	return 0;

}

