/*************************************************************************
	> File Name: pthread_TSD.c
	> Author: 
	> Mail: 
	> Created Time: 2016年08月20日 星期六 16时08分41秒
 ************************************************************************/

#include <stdio.h>
#include <string.h>
#include <pthread.h>

pthread_key_t key;

void *thread2(void *arg)
{
    int tsd = 5;
    printf("thread %d is running\n", pthread_self());
    pthread_setspecific(key, (void *)tsd);
    printf("thread %d return %d\n", pthread_self(), pthread_getspecific(key));
}


void *thread1( void *arg )
{
    int tsd = 0;
    pthread_t thid2;
    printf("thread %d is running\n", pthread_self());
    pthread_setspecific( key, (void *)tsd );
    pthread_create(&thid2, NULL, thread2, NULL);
    sleep(2);
    printf("thread %d return %d\n", pthread_self(), pthread_getspecific(key));
}

int main( int argc, char *argv )
{
    pthread_t thid1;
    printf("main thread begigs running\n");
    pthread_key_create( &key, NULL );  
    pthread_create( &thid1, NULL, thread1, NULL );
    sleep(3);
    pthread_key_delete( key );
    printf("main thread exit\n");
    return 0;
}
