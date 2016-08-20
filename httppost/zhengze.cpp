/*************************************************************************
	> File Name: zhengze.cpp
	> Author: 
	> Mail: 
	> Created Time: 2016年08月17日 星期三 14时38分56秒
 ************************************************************************/

#include<iostream>
using namespace std;

class MyPreg
{
public:
    MyPreg() = default ;
    MyPreg( std::string preg )
    {
        this->regex = pre;
    }
    ~MyPreg();
    
    /*将正则表达式编译为标准格式*/
    int regcomp(regex_t *preg, const char *regex, int cflags);

    /*匹配string中的preg串,存入pmatch_t*/
    int regexec(const regex_t *preg, const char *string, size_t nmatch, regmatch_t pmatch[], int eflags);

    /**/
private:
    std::string regex;    //原正则字符串



