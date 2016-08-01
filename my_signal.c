/*************************************************************************
	> File Name: my_signal.c
	> Author: 
	> Mail: 
	> Created Time: 2016年07月29日 星期五 16时30分15秒
 ************************************************************************/

#include<stdio.h>
#include <signal.h>

void handler_sigint( int signal )
{
    printf( "recv SIGINT\n" );
}
int main(int argc, char *argv[])
{
    signal(SIGINT, handler_sigint);
    while( 1 )
    ;
    return 0;
}
