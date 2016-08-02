/*************************************************************************
	> File Name: time_stack.cpp
	> Author: 
	> Mail: 
	> Created Time: 2016年08月01日 星期一 17时39分53秒
 ************************************************************************/

#ifndef MIN_HEAP
#define MIN_HEAP

#include <iostream>
#include <netinet/in.h>
#include <time.h>

using std::exception;

#define BUFFER_SIZE 64

class heap_timer;   /*前向声明*/

/*客户信息*/
struct client_data
{
    sockaddr_in address;
    int sockfd;
    char buf[ BUFFER_SIZE ];
    heap_timer* timer;
};


class heap_timer
{
public:
    heap_timer( int delay )
    {
        expire = time(NULL) + delay;
    }
public:
    time_t expire;  /*定时生效的绝对事件*/
    void ( *cb_func )( client_data* );   /*定时器的回调函数*/
    client_data* user_data;   /*用户数据*/
};


/*时间堆类*/
class time_heap
{
public:
    /*构造函数1，初始化一个大小为cap的空堆*/
    time_heap( int cap ) throw ( std::excption ) : capacity( cap ), cur_size( 0 );

    /*构造函数2，用已有数组来初始化堆*/
    time_heap( heap_timer** init_array, int size, int capacity ) throw ( std::exception ):cur_size( size ),capacity( capacity );

    /*销毁时间堆*/
    ~time_heap();

public:
    /*添加目标定时器*/
    void add_timer( heap_timer* timer ) throw (std::exception);

    /*删除目标定时器*/
    void del_timer( heap_timer* timer );

    /*获得堆顶部的定时器*/
    heap_timer* top() const ;

    /*删除堆顶定时器*/
    void pop_timer();

    /*心博函数*/
    void tick();

    /*判断时间堆是否为空*/
    bool empty const { return cur_size == 0; }

private:
    /*最小堆的下滤操作*/
    void percolate_down(int hole);

    /*将数组容量扩大一倍*/
    void resize() throw ( std::exception );

private:
    heap_timer** array;   //堆数组
    int capacity;    //堆数组的数组容量
    int cur_size;    //堆数组当前包含容量的个数
};


/*构造函数1，初始化一个大小为cap的空堆*/
time_heap:: time_heap( int cap ) throw ( std::excption ) : capacity( cap ), cur_size( 0 )
{
    array = new heap_timer* [ capacity ];    //分配空间
    if( array )
    {
        throw std :: exception();
    }
    for( int i=0; i<capacity; i++ )
    {
        array[i] = NULL;
    }
}


/*构造函数2，用已有数组来初始化堆*/
time_heap::time_heap( heap_timer** init_array, int size, int capacity ) throw ( std::exception ):cur_size( size ),capacity( capacity )
{
    if( capacity < size )
    {
        throw std::exception;
    }
    array = new heap_timer* [ capacity ];
    if( !array )
    {
        throw std::exception;
    }
    for( int i=0; i<capacity; i++ )
    {
        array[i] =  NULL;
    }
    if( size != 0 )
    {
        for( int i=0; i<size; i++ )
        {
            array[ i ] = init_array[ i ];     //用已有数组初始化堆
        }
        for( int i=(cur_size-1)/2; i>=0; --i )
        {
            percolate_down( i );        //对数组中的第[(cur_size-1)/2]~0个元素执行下滤操作
        }
    }
}


/*销毁时间堆*/
time_heap :: ~time_heap()
{
    for( int i=0; i<capacity; i++ )
    {
        delete array[ i ];
    }
    delete [] array;
}



/*添加目标定时器*/
time_heap:: add_timer( heap_timer* timer ) throw (std::exception)
{
    if( !timer )
    {
        return;
    }
    if( cur_size >= capacity )
    {
        resize();
    }
    int hole = cur_size++;      //新插入一个元素。当前堆大小加1，hole是新建空穴的位置
    int parent = 0;

    for( ; hole>0; hole=parent )
    {
        parent = ( hole-1 )/2;
        if( timer -> expire >= array[parent] -> expire) 
        {
            break;
        }
        array[ hole ] = array[parent];
    }
    array[ hole ] = timer;
}


/*删除目标定时器timer*/
time_heap:: del_timer( heap_timer* timer )
{
    if( !timer )
    {
        return ;
    }
    /*仅将定时器回调函数置空。即所谓的延迟销毁。这样节省开销，但容易使堆组膨胀*/
    timer -> cb_func = NULL;
}


/*获得堆顶部的定时器*/
heap_timer* time_heap::top() const
{
    if( empty() )
    {
        return NULL;
    }
    return array[0];
}


/*删除堆顶部的定时器*/
time_heap::pop_timer()
{
    if( empty() )
    {
        return ;
    }
    delete array[0];
    array[0] = array[cur_size];
    cur -= 1;
    percolate_down( 0 );
}


/*心博函数*/
time_heap:: tick()
{
    heap_timer* temp = array[0];

    time_t cur = time( NULL );

    while( !empty() )
    {
        if( temp -> expire > cur )
        {
            break;
        }
        if( array[0] -> cb_func)
        {
            array[0] -> cb_func( array[0] -> user_data );
        }
        pop_timer();
        temp = array[0];
    }
}


/*最小堆执行下滤操作*/
time_heap::percolate_down( int hole )
{
    heap_timer* temp = array[ hole ];
    int child = 0;
    
    for( ; (hole*2+1) <= (cur_size-1); hole=child )
    {
        child = hole*2 + 1;
        if( child < (cur_size-1) && (array[child+1] -> expire < array[child] -> expire))
        {
            ++child;        
        }
        if( arrry[child] -> expire < temp -> expire )
        {
            array[hole] = array[child];
        }
        else
        {
            break;
        }
    }
    array[hole]  = temp;
}


/*将堆组容量扩大1倍*/
time_heap::resize() throw ( std::exception )
{
    heap_timer** temp = new heap_timer* [2*capacity];

    for( int i=0; i<2*capacity; ++i )
    {
        temp[i] = NULL;
    }
    if( !temp )
    {
        throw std::exception;
    }
    capacity = 2 * capacity;
    for( int i=0; i<cur_size; ++i )
    {
        temp[i] = array[i];
    }
    delete [] array;
    array = temp;
}
#endif
