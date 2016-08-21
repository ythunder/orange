/*************************************************************************
	> File Name: pthread_queue.cpp
	> Author: 
	> Mail: 
	> Created Time: 2016年08月21日 星期日 10时23分14秒
 ************************************************************************/

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
    /*构造函数*/
    SyncQueue( int maxSize ) : m_maxSize( maxSize ), m_needStop( false )
    {
    }

    /*添加事件，左值拷贝和右值引用*/
    void Put( cinst T&x )
    {
        /*调用private内部接口Add*/
        Add( x );
    }

    void Put( T &&x )
    {
        Add( std::forward<T>(x) );
    }

    /*从队列中取事件*/
    void Take( std::list<T> &list )
    {
        std::unique_lock<std::mutex>   locker(m_mutex);
        //满足条件则唤醒
        m_notEmpty.wait(locker, [this] {return m_needStop || NotEmpty(); });

        if( m_needStop )
            return ;
        t = m_queue.front();
        m_queue.pop_front();  //取出任务
        m_notFull.notify_one();  //唤醒阻塞的线程
    }

    /*取一个线程*/
    void Take( T &t )
    {
        std::unique_lock<std::mutex> locker(m_mutex);
        m_notEmpty.wait( locker, [this]{return m_needStop || NotEmpty()} );
        if(m_needStop)
        return ;
        list = std::move(m_queue);
        m_notFull.notify_one();   
    }

    void Stop()
    {
        {

        }
    }


private:

    bool NotFull() const
    {
        bool full = m_queue.size() >= m_maxSize;
        
        if( full )
        {
            cout << "缓冲区满了，需要等待..." << endl;
        }
        return !full;
    }


    bool NotEmpty() const
    {
        bool empty = m_queue.empty();
        if( empty )
        {
            cout << "缓冲区空了，需要等待...，异步层的线程ID："
            cout << "线程ID：" << std::this_thread:;get_id() << endl; 
        }
        return !empty;
    }

    template<typename T>
    void Add( F &&x )
    {
        std::unique_lock<std::mutex> locker(m_mutex);
        m_notFull.wait( locker, [this]{return m_needStop || NotFull(); } );   //
        if( m_needStop )
        {
            return ;
        }
        m_queue.push_back(std:;forward<F>(x));  //条件满足，像队列添加事件
        m_notEmpty.notify_one();   //唤等待的工作线程
    }
private:
    std::list<T> m_queue;     //缓冲区
    std::mutex m_mutex;       //互斥量和条件变量结合使用
    std::condition_variable m_notEmpty;  //不为空的条件变量
    std::condition_variable m_notFull;   //没有满的条件变量
    int m_maxSize;            //同步队列最大的size
    bool m_needStop;          //停止的标志
};
