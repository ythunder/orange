/*************************************************************************
	> File Name: method.cpp
	> Author: 
	> Mail: 
	> Created Time: 2016年07月30日 星期六 07时39分31秒
 ************************************************************************/

#include <stdio.h>
#include <iostream>

using namespace std;

template< typename T> 
void swap( T& t1, T& t2 )
{
    T temp;
    temp = t1;
    t1 = t2;
    t2 = temp;
}

int main()
{
    int num =1, num2 = 3;
    swap(num, num2);
    std::cout << num << " " << num2;
    return 0;
}
