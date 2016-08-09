/*************************************************************************
	> File Name: tick.h
	> Author: 
	> Mail: 
	> Created Time: 2016年08月03日 星期三 09时53分25秒
 ************************************************************************/

#ifndef _TICK_H
#define _TICK_H

class tw_timer
{
public:
    tw_timer(int rot, int ts);

public:
    int rotation; /*记录定时器再时间轮中转多少圈后生效*/
    int time_slot;  /*记录定时器属于时间轮上哪个槽*/
    void (*cb_func)( client_data* ); /*定时器回调函数*/
    client_data* user_data;   /*客户数据*/
    tw_timer* next;   /*指向下一个定时器*/
    tw_timer* prev;   /*指向上一个定时器*/
}

class time_wheel
{
public:
    time_wheel();
    ~time_wheel();
    
    /*根据定时值timeout创建一个定时器并插入合适槽中*/
    tw_timerr* add_timer( int timeout );

    /*删除目标定时器*/
    void del_timer( tw_timer* timer )；

    /*SI时间到后，调用该哈数，时间轮像前滚动一个槽*/
    void tick();:
}

#endif
