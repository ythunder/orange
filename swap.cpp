/*************************************************************************
	> File Name: swap.h
	> Author: 
	> Mail: 
	> Created Time: 2016年07月30日 星期六 07时37分43秒
 ************************************************************************/

#include<iostream>
using namespace std;
template<typename T>
T n_add(T a,T b)
{
 T c;
 c=a+b;
 return c;
} 
int main()
{
 cout<<n_add(5,3)<<endl;
 cout<<n_add(5.35,5.5)<<endl;
 return 0;
}
