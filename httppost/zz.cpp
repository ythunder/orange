/*************************************************************************
	> File Name: zhengze.h
	> Author: 
	> Mail: 
	> Created Time: 2016年08月17日 星期三 14时55分54秒
 ************************************************************************/

#ifndef _ZHENGZE_H
#define _ZHENGZE_H

#include <regex>
#include <iostream>
#include <stdio.h>
#include <string>
#include <fstream>
#include <streambuf>

using namespace std;

class Student
{
    std::string id;
    std::string name;
    std::string college;
    std::string profess;
    std::string clas;
};

 
int main( int argc, char *argv[] )
{
    char xueha[9];

    std::ifstream t("/home/yunting/post2.txt");
    std::string html( (std::istreambuf_iterator<char>(t)) , std::istreambuf_iterator<char>());
    std::smatch results;

   // std::regex xuehao("0[0-9]1[2-6]\\d{4}");    //改正正则式的格式
   const std::regex xuehao("(\\d{1,3}):(\\d{1,3}):(\\d{1,3}):(\\d{1,3})");
/*
    regex_search(html, results, xuehao);   //匹配

    cout << results.str() << endl;
*/
    return 0;
}


#endif
