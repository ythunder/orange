/*************************************************************************
	> File Name: liangshatian.c
	> Author: ma6174
	> Mail: ma6174@163.com 
	> Created Time: 2016年07月13日 星期三 14时08分04秒
 ************************************************************************/

#include<stdio.h>

int main()
{
	int i,sum = 0;
	for(i=1; i<=50; i++) {
		if(i%2 == 1) {
			sum += i;
		}
	}
	printf("sum = %d\n", sum);
}
