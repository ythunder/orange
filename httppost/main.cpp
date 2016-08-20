/*************************************************************************
	> File Name: main.cpp
	> Author: 
	> Mail: 
	> Created Time: 2016年08月16日 星期二 13时48分27秒
 ************************************************************************/

#include <iostream>
#include "post.h"
#include "post2.h"
#include <string>

int main( int argc, char *argv[] )
{
    std::string id = "04141114";
    std::string passwd = "yuntingxxyd.13";
    const char *filename = "/home/yunting/post.txt";
    const char *filena = "/home/yunting/post2.txt";
    postUrl( filename , id, passwd );
    postUrl_two(filena, id);
    return 0;
}

