/*************************************************************************
	> File Name: SOCK.h
	> Author: 
	> Mail: 
	> Created Time: 2016年08月26日 星期五 20时57分58秒
 ************************************************************************/

#ifndef _SOCK_H
#define _SOCK_H

#include <arpa/inet.h>
#include <assert.h>
#include <stdio.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <fcntl.h>
#include <stdlib.h>
#include <sys/epoll.h>
#include <pthread.h>


class Socket
{

public:

    Socket(sa_family_t family=AF_INET, int listen_num=100);

    ~Socket();


    int setnonblocking(); /*文件设置非阻塞*/

    bool bindAddressPort(); /*绑定IP和端口*/

    bool listenSocket(int listen_num);

    bool setReuseAddr(); /*设置地址重用*/

    bool setReusePort(); /*设置端口重用*/

    bool setKeepAlive(); /*设置保活机制*/

    int getSocketfd();  //得到监听套接诶字

private:
    
    int sockfd;  /*网络套接字文件描述符*/
    int backlog; /*监听的客户端的最大文件描述符数*/
    struct sockaddr_in serveraddress;
    struct sockaddr_in clientaddr;
    
};
#endif
