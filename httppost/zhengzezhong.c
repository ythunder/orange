/*************************************************************************
	> File Name: zhengze.c
	> Author: 
	> Mail: 
	> Created Time: 2016年08月17日 星期三 16时30分35秒
 ************************************************************************/
#include <sys/types.h>
#include <stdio.h>
#include <stdlib.h>
#include <regex.h>
#include "gb2312-utf.cpp"

int main( int argc, char *argv[] )
{
    const char *patton = "......学院";
    char *html;   //存储网页信息的字符串
    FILE *fp;
    int file_size;
    regmatch_t pmatch[1];
    const size_t nmatch = 1;
    regex_t *comp;
    char res[100];
    int i=0, j=0;
    int status;
    char buf[100];
    
    iconv_t cd = iconv_open("utf-8","");

    comp = (regex_t *)malloc(sizeof(regex_t));
    fp = fopen("/home/yunting/post2.txt", "r");
    fseek( fp, 0, SEEK_END );         //将文件指针移动到文件尾
    file_size = ftell( fp );          //计算偏移位置来得到文件大小
    fseek( fp, 0, SEEK_SET );         //将文件指针移动到文件开头
    html = (char *)malloc( file_size*sizeof( char ) );
    fread( html, file_size, sizeof(char), fp );   //从文件读出内容到html
    fclose(fp);

    mainq(html);

    regcomp( comp, patton, REG_EXTENDED );   //编译正则表达式

    status = regexec( comp, html, nmatch, pmatch, 0);   //匹配正则表达式
    printf("status = %d\n",status);
    if (status != 0 )
    {
        //
        regerror(status,comp,buf,100);
        printf("%s\n",buf);
    }

    printf("%d %d\n", pmatch[0].rm_so, pmatch[0].rm_eo);
    for(i=pmatch[0].rm_so; i<pmatch[0].rm_eo; i++)
    {
        res[j++] = html[i];
    }
    printf("%s", res);
    regfree(comp);   //释放正则表达式

    return 0;
}

