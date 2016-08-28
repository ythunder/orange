/*************************************************************************
	> File Name: SOCK.cpp
	> Author: 
	> Mail: 
	> Created Time: 2016年08月27日 星期六 17时54分31秒
 ************************************************************************/
#include "SOCK.h"
#include <iostream>
#include <fcntl.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <sys/epoll.h>

using namespace std;

/*构造函数*/
Socket::Socket(sa_family_t family, int listen_num)
{
    const char *ip = "127.0.0.1";
    this->sockfd = socket(family, SOCK_STREAM, 0);
    if(this->sockfd < 0)
    {
        cout << "socket创建失败" << endl;
    }
    setReuseAddr();
    setReusePort();
    setKeepAlive();
    bzero(&this->serveraddress, sizeof(struct sockaddr_in));
    this->serveraddress.sin_family = AF_INET;
    inet_pton(AF_INET, ip, &serveraddress.sin_addr);
    this->serveraddress.sin_port = htons(8888);

    if(bindAddressPort()) 
    {
        if(listenSocket(listen_num)) 
        {

        }
        return ;
    }

}

int 
Socket::getSocketfd()
{
    return this->sockfd;
}
/*析构函数*/
Socket::~Socket()
{
    close(this->sockfd);
}


/*设置文件非阻塞*/
int 
Socket::setnonblocking()
{
    int old_option = fcntl(this->sockfd, F_GETFL);
    int new_option = old_option | O_NONBLOCK;
    fcntl(this->sockfd, F_SETFL, new_option);
    return old_option;
}
 
/*绑定IP和端口*/
bool 
Socket::bindAddressPort()
{
    int ret = bind(this->sockfd, (struct sockaddr* )&serveraddress, sizeof(serveraddress));
    if(ret < 0)
    {
        cout << "绑定IP和端口" << endl;
        return false;
    }
    return true;
}

/*监听套接字*/
bool
Socket::listenSocket(int listen_num)
{
    int ret = listen(this->sockfd, listen_num);
    if(ret == -1)
    {
        cout << "监听错误" << endl;
        return false;
    }
    return true;
}

/*设置地址重用*/    
bool 
Socket::setReuseAddr()
{
    int on = 1;
    setsockopt(this->sockfd,SOL_SOCKET, SO_REUSEADDR, &on, sizeof(on));
    return true;
}

/*设置端口重用*/
bool 
Socket::setReusePort()
{
    int on = 1;
    setsockopt(this->sockfd, SOL_SOCKET, SO_REUSEPORT, &on, sizeof(on));
    return true;
}

/*设置保活机制*/
bool 
Socket::setKeepAlive()
{
    int on = 1;
    setsockopt(sockfd, SOL_SOCKET, SO_KEEPALIVE, &on, sizeof(on));
    return true;
}


