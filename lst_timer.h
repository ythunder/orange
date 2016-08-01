/*************************************************************************
	> File Name: lst_timer.h
	> Author: 
	> Mail: 
	> Created Time: 2016年07月30日 星期六 16时22分22秒
 ************************************************************************/

#ifndef _LST_TIMER
#define _LST_TIMER

#include <time.h>
#define BUFFER_SIZE 64
class util_timer;  

/*用户数据结构：客户端socket地址、socket文件描述符、读缓冲区和定时器*/
struct client_data
{
    sockaddr_in address;
    int sockfd;
    char buffer[ BUFFER_SIZE ];
    util_timer* timer;
};

/*定时器类*/
struct util_timer
{
    public:
        util_timer() : prev( NULL ), next( NULL ) {  }
    public:
        time_t expire;   /*任务的超时时间*/
        void ( *cd_func )(client_data*);   /*任务回调函数*/
        client_data* user_data;
        util_timer* prev;
        util_timer* next;
};

/*定时器链表。它是一个升序、双向链表，且带有头结点和尾节点*/
class sort_timer_lst
{
    public:
        /*构造函数*/
        sort_timer_lst() : head( NULL ), tail( NULL ) {}

        /*析构函数，清空定时器链表*/
        ~sort_timer_lst();

        /*将目标定时器timer添加到链表中*/
        void add_timer( util_timer* timer );

        /*当某个定时任务发生变化时，调整对应的定时器在链表的位置，这个函数只考虑被调整的定时器事件延长情况，即只需向尾部移*/
        void adjust_timer( util_timer* timer );

        /*将目标定时器timer从链表重删除*/
        void del_timer( util_timer* timer );

        /*SIGALRM信号每次被触发就在其信号处理函数中执行一次tick函数，以处理链表上到期的任务*/
        void tick();

    private:
        /*一个重载的辅助函数，被函数add和adjust调用，该函数表示将time添加到head之后的位置*/
        void add_timer( util_timer* timer, util_timer* lst_head );

    private:
        util_timer* head;
        util_timer* tail;
};

/*析构函数*/
sort_timer_lst::~sort_timer_lst()
{
    util_timer* temp = head;
    while( temp )
    {
        head = temp -> next;
        delete temp;
        temp = head;
    }
}


/*将目标定时器timer添加到链表中*/
sort_timer_lst:: add_timer( util_timer* timer )
{
    if( !timer )
    {
        return ;
    }
    if( !head ) 
    {
        head = tail = timer;
        return ;
    }
    if( timer -> expire < head -> expire )  //timer时间小于链首
    {
        timer -> next = head;
        head -> prev = timer;
        head = timer;
        return ;
    }
    
    add_timer( time, head );
}


/*当某个定时任务发生变化时，调整对应的定时器在链表的位置，这个函数只考虑被调整的定时器事件延长情况，即只需向尾部移*/
sort_timer_lst :: adjust_timer( util_timer* timer )
{
    if( !timer )
    {
        return ;
    }
    util_timer* temp = head -> next
    if( !temp || timer -> expire < temp -> expire )
    {
        return ;
    }
    if( timer == head )
    {
        head = head -> next;
        head -> prev = NULL;
        head -> next = NULL;
        add_timer( timer, head );
    }
    else
    {
        timer -> prev -> next = timer -> next;
        timer -> next -> prev = timer -> prev;
        add_timer( timer, head );
    }
}


/*将目标定时器timer从链表重删除*/
sort_timer_lst :: del_timer( util_timer* timer )
{
    if( !timer )
    {
        return ;
    }
    if( ( timer == head ) && (timer == tail))
    {
        delete timer;
        head = NULL;
        tail = NULL;
        return ;
    }
    if( timer == head )
    {
        head = head -> next;
        head -> prev = NULL;
        delete timer;
        return ;
    }
    if( timer == tail )
    {
        tail  = tail -> next;
        tail -> next = NULL;
        delete timer;
        return ;
    }
    timer -> prev -> next = timer -> next;
    timer -> next -> prev = timer -> prev;
    delete timer;
}


/*SIGALRM信号每次被触发就在其信号处理函数中执行一次tick函数，以处理链表上到期的任务*/
sort_timer_lst :: tick()
{
    if( !head )
    {
        return ;
    }
    printf( "timer tick!\n" );
    time_t cur = time( NULL );   /*获取系统当前时间*/
    util_timer* temp = head;
    while( temp )
    {
        if( temp -> expire > cur )
        {
            break;
        }
        temp -> cb_func( temp -> user_data );
        head = temp -> next;
        if( head )
        {
            head -> prev = NULL;
        }
        delete temp;
        temp = head;
    }
}


/*一个重载的辅助函数，被函数add和adjust调用，该函数表示将time添加到head之后的位置*/
sort_timer_lst :: add_timer( util_timer* timer, util_timer* lst_head )
{
    util_timer* prev = lst_head;
    util_timer* temp = prev -> next;

    while( temp )
    {
        if( temp -> expire >  timer -> expire )
        {
            prev -> next = timer;
            timer -> prev = prev;
            timer -> next = temp;
            temp -> prev = timer;
        }
        prev = temp;
        temp = temp -> next;
    }
    if( !tmp )
    {
        prev -> next = timer;
        timer -> prev = prev;
        timer -> next = NULL;
        tail = timer;
    }
}
#endif
