/*************************************************************************
	> File Name: pthread_pool.cpp
	> Author: 
	> Mail: 
	> Created Time: 2016年08月22日 星期一 09时17分19秒
 ************************************************************************/

#include <iostream>
#include <list>
#include <thread>
#include <functional>
#include <memory>
#include <atomic>
#include "pthread_queue.cpp"

using namespace std;

const int MaxTaskCount = 100;

class ThreadPool
{
public:
    //规定任务类型为void,funciton可为任意函数，即可处理任意任务
    using Task = std::function<void()>;
    
    //hardware_concurrency检测硬件性能，给出默认线程数
    ThreadPool( int numThreads = std::thread::hardware_concurrency()): m_queue(MaxTaskCount)
    {
        Start(numThreads);   
    }

    ~ThreadPool(void)
    {
        //如果没有停止则主动停止线程池
        Stop();
    }

    //call_once保证StopThreadGroup只被调用一次
    void Stop()
    {
        std::call_once(m_flag, [this]{StopThreadGroup(); });
    }

    //普通形式和右值引用的添加任务
    //添加很多任务
    void AddTask(Task &&task)
    {
        m_queue.Put(std::forward<Task>(task));
    }
    //添加一个任务
    void AddTaskk(const Task &task)
    {
        m_queue.Put(task);
    }


    
    void Start( int numThreads )
    {
        m_running = true;

        //创建线程组
        for( int i=0; i<numThreads; ++i )
        {   
            cout << " Init create thread pool " << endl;
            m_threadgroup.push_back(std::make_shared<std::thread>(&ThreadPool::RunInThread, this));
        }
    }

private:
    void RunInThread()
    {
        while( m_running )
        {
            std::list<Task> list;
            cout << "take\n";
            m_queue.Take(list);

            for(auto &task : list)
            {
                if(!m_running)
                    return ;
                task();
            }
        }
    }

    void StopThreadGroup()
    {
        //让同步队列中的线程停止
        m_queue.Stop();
        m_running = false;

        for(auto thread : m_threadgroup)
        {
            if(thread)
            {
                thread->join();
            }
        }
        m_threadgroup.clear();
    }

private:
    std::list<std::shared_ptr<std::thread>> m_threadgroup;  //处理任务的线程组
    SyncQueue<Task> m_queue;    //同步队列
    atomic_bool m_running;      //是否停止的标志
    std::once_flag m_flag;  
};

int main( int argc, char *argv[] )
{
    ThreadPool pool(2);

    std::thread thd1([&pool]{
        for( int i=0; i<10; i++ )
        {
            auto thdId = std::this_thread::get_id();
            pool.AddTask([thdId]{
                std::cout << thdId << "thread execute task"<< std::endl;
            });
        }
    });
    std::this_thread::sleep_for(std::chrono::seconds(2));
    pool.Stop();
    thd1.join();

    return EXIT_SUCCESS;
}
