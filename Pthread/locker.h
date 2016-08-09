/*************************************************************************
	> File Name: locker.h
	> Author: 
	> Mail: 
	> Created Time: 2016年08月09日 星期二 08时58分32秒
 ************************************************************************/

#ifndef _LOCKER_H
#define _LOCKER_H

/*封装信号的类*/
class sem
{
public:
    /*构造函数初始化信号量*/
    sem()
    {
        if( sen_init(&m_sem, 0, 0) != 0)
        {
            throw std::exception();
        }
    }
    /*销毁信号量*/
    ~sem()
    {
        sem_destroy( &m_sem );
    }
    /*等待信号量*/
    bool wait()
    {
        return sem_wait( &m_sem ) == 0;
    }
    /*增加信号量*/
    bool post()
    {
        return sem_post( &m_sem ) == 0;
    }

private:
    sem_t m_sem;

};

/*封装互斥锁的类*/
class locker
{
    /*创建并初始化互斥锁*/
    locker()
    {
        if( pthread_mutex_init( &m_mutex, NULL ) != 0)
        {
            throw std::exception();
        }
    }
    /*销毁互斥锁*/
    ~locker()
    {
        pthread_mutex_destroy( &m_mutex );
    }
    /*获取互斥锁*/
    bool lock()
    {
        return pthread_mutex_lock( &m_mutex ) == 0;
    }
    /*释放互斥锁*/
    bool unlock()
    {
        return pthread_mutex_unlock( &m_mutex ) == 0;
    }

private:
    pthread_mutex_t m_mutex;
};


/*封装条件变量的类*/
class cond
{
    /*创建并初始化条件变量*/
    cond()
    {
        if( pthread_mutex_init( &m_mutex, NULL) != 0)    //先加锁
        {
            throw std::exception();
        }
        if( pthread_cond_init( &m_cond, NULL) != 0)
        {
            /*构造函数一旦失败,立即释放已成功分配了的资源*/
            pthread_mutex_destroy( &m_mutex );
            throw std::exception;
        }
    }
    /*销毁添加变量*/
    ~cond()
    {
        pthread_mutex_destory( &m_mutex );
        pthread_cond_destroy( &m_cond );
    }
    /*等待条件变量*/
    bool wait()
    {
        int ret = 0;
        pthread_mutex_lock( &m_mutex);
        ret = pthread_cond_wait( &m_cond. &m_mutex);
        pthread_mutex_unlock( &m_mutex);
        return ret == 0;
    }
    /*唤醒等待条件变量的线程*/
    bool signal()
    {
        return pthread_cond_signal( &m_cond ) == 0;
    }

private:
    pthread_mutex_t m_mutex;     //互斥锁
    pthread_cond_t m_cond;     //条件变量
};


#endif
