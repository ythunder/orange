/*************************************************************************
	> File Name: client.cpp
	> Author: ma6174
	> Mail: ma6174@163.com 
	> Created Time: 2016年07月12日 星期二 10时22分20秒
 ************************************************************************/

#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <assert.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <errno.h>

#define BUFFER_SIZE 1024

int main(int argc, char *argv[])
{
	if(argc <= 2) {
		printf("usage:%s ip_address port_number\n", basename(argv[0]));
		return 1;
	}
	const char* ip = argv[1];
	int port = atoi(argv[2]);

	struct sockaddr_in address;
	bzero(&address, sizeof(struct sockaddr_in));
	address.sin_family = AF_INET;
	inet_pton(AF_INET, ip, &address.sin_addr);
	address.sin_port =  htons(port);
	
	int sock = socket(PF_INET, SOCK_STREAM, 0);
	assert(socket >= 0);

	int recvbuf = atoi(argv[3]);
	int len = sizeof(recvbuf);

	setsockopt(sock, SOL_SOCKET, SO_RCVBUF, &recvbuf, sizeof(recvbuf));
	getsockopt(sock, SOL_SOCKET, SO_RCVBUF, &recvbuf, (socklen_t* )&len);
	printf("the tcp receive buffer size after setting is %d\n", recvbuf);


	int ret = bind(sock, (struct sockaddr* )&address, sizeof(address) );
	assert(ret != -1);

	ret = listen(sock, 5);
	assert(ret != -1);

	struct sockaddr_in client;
	socklen_t client_addrlength = sizeof(client);
	int connfd = accept(sock, (struct sockaddr* )&client, &client_addrlength); //client用于获取被接受连接的远端socket地址
	if(connfd < 0) {
		printf("errno is :%d\n", errno);
	} else {
		char buffer[BUFFER_SIZE];
		memset(buffer, 'a', BUFFER_SIZE);
		while(recv(connfd, buffer, BUFFER_SIZE-1, 0) > 0) {}
		close(connfd);
	}
	close(sock);

		return 0;
}
