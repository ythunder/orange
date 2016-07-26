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

	int ret = bind(sock, (struct sockaddr* )&address, sizeof(address) );
	assert(ret != -1);

	ret = listen(sock, 5);
	assert(ret != -1);

	struct sockaddr_in client;
	socklen_t client_addrlength = sizeof(client);
	int connfd = accept(sock, (struct sockaddr* )&client, &client_addrlength); //client用于获取被接受连接的远端socket地址
	if(connfd < 0) {
		printf("errno is :%d\n", errno);
		close(sock);
	}
	

	char buf[1024];
	fd_set read_fds;
	fd_set exception_fds;
	FD_ZERO( &read_fds );    
	FD_ZERO( &exception );

	while(1) 
	{
		memset(buf, '\0', sizeof(buf));

		FD_SET(connfd, &read_fds);     //将connfd加入read_fds集合
		FD_SET(connfd, &exception_fds);

		ret = select(connfd+1, &read_fds, NULL, &exception_fd, NULL);
		assert(ret != -1);

		if(FD_ISSET(connfd, &read_fds)) 
		{
			ret = recv(connfd, buf, sizeof(buf)-1, 0);
			if(ret < 0) 
			{
				break;
			}
			printf("get %d bytes of normal data:%s\n",ret ,buf);
		}
		else if(FD_ISSET(connfd, &exception)) 
		{
			ret = recv(connfd, buf, sizeof(buf)-1, MSG_OOB);
			if(ret <= 0) 
			{
				break;
			}
			printf("get %d bytes of obb data: %s\n", ret, buf);
		}
	}
	close(connfd);
	close(sock);
	return 0;
}
