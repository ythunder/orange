#include<stdio.h>
#include<stdlib.h>
#include<pthread.h>

#define MAX     1000000000    //整数范围 1 ～ MAX
#define N       100           //创建N 个子线程求和
#define AVE     (MAX/N)       //每个子线程处理的整数个数

long long     *sum = NULL;    //保存各个子线程计算的结果

//获取当前时间
double get_time()
{
    struct timeval t;
    gettimeofday(&t,NULL);
    return t.tv_sec + t.tv_usec/1000000.0;
}

//求和子线程
void* sum_work(void* arg)
{
    int            n = (int)arg;  //第n部分
    long long      start = n*AVE+1;
    long long      end = start + AVE -1;
    long long      i;
    sum[n] = 0;

    //计算start ~ end 范围的整数和
    for(i=start; i <= end;i++)
    {
        sum[n] = sum[n] + i;
    }
    pthread_exit(0);
}

int main()
{
    double         t1,t2;
    pthread_t      *pthread_id = NULL; //保存子线程id
    int            i;
    long long      result = 0;         //总和

    pthread_id = (pthread_t*)malloc(N * sizeof(pthread_t));
    sum  = (long long*)malloc(N * sizeof(long long));

    //开始计算
    t1 = get_time();

    //创建N个子线程
    for(i=0;i<N;i++)
    {
        pthread_create(pthread_id+i,NULL,sum_work,i);
    }

    //将各个子线程的求和结果综合到一起
    for(i=0;i<N;i++)
    {
        //等待子线程结束，如果该子线程已经结束，则立即返回
        pthread_join(pthread_id[i],NULL);
        result += sum[i];
    }

    //求和结束
    t2 = get_time();

    //输出求和结果和运行时间
    printf("sum of 1 ~ %lld is %lld runtime is %f\n",(long long)MAX,result,t2-t1);

    free(pthread_id);
    free(sum);
    return 0;
}
