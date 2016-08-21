/*************************************************************************
	> File Name: time_te.cpp
	> Author: 
	> Mail: 
	> Created Time: 2016年08月20日 星期六 22时34分25秒
 ************************************************************************/

#include <iostream>
#include <time.h>

using namespace std;

int main(void)
{
    time_t nowtime;
    nowtime = time(NULL);
    cout << nowtime  << endl;
}
