/*************************************************************************
	> File Name: epoll.h
	> Author: 
	> Mail: 
	> Created Time: 2016年08月15日 星期一 22时47分05秒
 ************************************************************************/

#ifndef _EPOLL_H
#define _EPOLL_H

class myEpoll
{
public:
    myEpoll( bool bEt=true );
    ~myEpoll();

    /*创建事件表*/
    void create( int max_connections );

    /*往事件表添加用户关心的事件*/
    void add( int fd, long long data, uint32_t event );

    /*修改*/
    
    /*创建事件表*/
    void create();
private:
    int epollfd;   /*事件文件描述符*/
    int max_connections;   /*最大连接数*/
    struct epoll_event *events   /*就绪事件*/
    bool et;   /*是否为ET模式*/
};
#endif
