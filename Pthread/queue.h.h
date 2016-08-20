//
// Created by yunting on 16-8-10.
//

#ifndef PTHREAD_QUEUE_H_H
#define PTHREAD_QUEUE_H_H

#include <list>
#include <mutex>
#include <thread>
#include <condition_variable>
#include <iostream>

using namespace std;

template<typename T>

class SyncQueue
{
public:

private:
    std::list<T> m_queue;   /*请求队列*/
    std::mutex m_mutex;     /*互斥量和条件变量结合起来使用*/
    std::condition_variable m_notEmpty;     /*不为空的条件变量*/
    std::condition_variable m_notFull;      /*没有满的条件变量*/

};

#endif //PTHREAD_QUEUE_H_H
