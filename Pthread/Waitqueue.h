/*************************************************************************
	> File Name: Waitqueue.h
	> Author: 
	> Mail: 
	> Created Time: 2016年08月09日 星期二 21时37分59秒
 ************************************************************************/

#ifndef _WAITQUEUE_H
#define _WAITQUEUE_H

template<typename T>
class SynQueue
{
public:

private:
    std::mutex m_mutex;     /*互斥锁*/
    std::list<T> m_queue;   /*队列，存放任务*/
    std::condition_variable m_notEmpty;    /*队列不为空的条件变量*/
    std::condition_variable m_notFull;   /*队列不为满的条件变量*/
    int m_maxSize;      /*任务队列最大长度*/
    bool:wq


}

#endif
