/*************************************************************************
> File Name: test.cpp
	> Author: 
	> Mail: 
	> Created Time: 2016年08月21日 星期日 19时50分44秒
 ************************************************************************/

#include<iostream>
using namespace std;

//值传递
void change1( int n )
{
    cout << "值传递--函数操作地址" << &n << endl;
    n++;
}
       
//引用传递
void change2( int &n )
{
    cout << "引用传递--函数操作地址" << &n << endl;
    n++;
}

//指针传递
void change3(int *n)
{
    cout << "指针传递--函数操作地址" << n << endl;
    *n = *n + 1;
}

int main()
{
    int n=10;
    cout << "实参的地址"<< &n << endl;

    change1(n);
    cout << "值传递后 n= " << n << endl;

    change2(n);
    cout << "引用传递后 n= " << n << endl;

    change3(&n);
    cout << "指针传递后 n= " << n << endl;

    return true;
}

