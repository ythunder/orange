/*************************************************************************
	> File Name: pthread_sum.c
	> Author: 
	> Mail: 
	> Created Time: 2016年08月20日 星期六 23时41分22秒
 ************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

#define MAX 1000000000
#define N 100     //创建N个子线程求和
#define AVE (MAX/N)   //每个子线程处理的整数个数

double get_time()
{
    struct timeval t;
    gettimeofday( &t, NULL);
    return t.tv_sec + t.tv_usec/1000000.0;
}


int main()
{
    douvle t1,t2;
    pthread_t *pthread_id = NULL;   //保存子线程ID
    int i;
    long long result = 0;   //总和

    pthread_id = (pthread_t *)malloc(N * sizeof(pthread_t));
    sum = (long long)malloc(N*sizeof(long long));

    t1 = get_time（）；

    for( i=0; i<N; i++)
    {
        pthread_create(pthread_id+i, NULL, sum_work, i);
    }
    for(i=0; i<N; i++)
    {
        :wq

    }
}

