/*************************************************************************
	> File Name: pthread_once.c
	> Author: 
	> Mail: 
	> Created Time: 2016年08月20日 星期六 13时38分19秒
 ************************************************************************/

#include<stdio.h>
#include <pthread.h>

pthread_once_t once = PTHREAD_ONCE_INIT;

void run(void)
{
    printf("Function run is running in thread %u\n", pthread_self());
}

void *thread1(void *arg)
{
    printf("Current thread's id is %u\n", pthread_self());

    pthread_once(&once, run);

    printf("thread1 ends\n");
}

void *thread2(void *arg)
{
    printf("Current thread's id is %u\n", pthread_self());

    pthread_once(&once, run);

    printf("thread2 end\n");
}


int main()
{
    pthread_t thid1, thid2;

    pthread_create(&thid1, NULL, thread1, NULL);
    
    pthread_create(&thid2, NULL, thread2, NULL);

    sleep(3);

    printf("main thread exit!\n");

    exit(0);

}
