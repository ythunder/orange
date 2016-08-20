/*************************************************************************
	> File Name: test.cpp
	> Author: 
	> Mail: 
	> Created Time: 2016年08月16日 星期二 15时57分03秒
 ************************************************************************/
#include <regex.h>
#include <iostream>
#include <sys/types.h>
#include <stdio.h>
#include <cstring>
#include <sys/time.h>

using namespace std;
const int times = 1000000;

int main(int argc,char** argv)
{
        char pattern[512]="finance\.sina\.cn|stock1\.sina\.cn|3g\.sina\.com\.cn.*(channel=finance|_finance$|ch=stock|/stock/)|dp.sina.cn/.*ch=9&";
        const size_t nmatch = 10;
        regmatch_t pm[10];
        int z ;
        regex_t reg;
        char lbuf[256]="set",rbuf[256];
        char buf[3][256] = {"finance.sina.cn/google.com/baidu.com.google.sina.cndddddddddddddddddddddda.sdfasdfeoasdfnahsfonadsdf",
                                               "3g.com.sina.cn.google.com.dddddddddddddddddddddddddddddddddddddddddddddddddddddbaidu.com.sina.egooooooooo",
                                               "http://3g.sina.com.cn/google.baiduchannel=financegogo.sjdfaposif;lasdjf.asdofjas;dfjaiel.sdfaosidfj"};
        printf("input strings:\n");
        timeval end,start;
        gettimeofday(&start,NULL);
        regcomp(&reg,pattern,REG_EXTENDED|REG_NOSUB);
        for(int i = 0 ; i < times; ++i)
    {
                for(int j = 0 ; j < 3; ++j)
        {
                        z = regexec(&reg,buf[j],nmatch,pm,REG_NOTBOL);
            /*          if(z==REG_NOMATCH)
             *                          printf("no match\n");
             *                                      else
             *                                                      printf("ok\n");
             *                                                                      */
                    
        }
            
    }
        gettimeofday(&end,NULL);
        uint time = (end.tv_sec-start.tv_sec)*1000000 + end.tv_usec - start.tv_usec;
        cout<<time/1000000<<" s and "<<time%1000000<<" us."<<endl;
        return 0 ;
}
