#include <sys/types.h>  
#include <regex.h>  
#include <stdio.h>  
#include <stdlib.h>  
  
  
int main(void)  
{  
    regex_t     reg;  
    //char        regex[] = "[0-9]{1,3}\\.[0-9]{1,3}\\.[0-9]{1,3}\\.[0-9]{1,3}";
    const char regex[] = "[\\u4e00-\\u9fa5]";
    //char        str[] = "\n\n\t111.21.2\n\t172.168.13.1";  
    size_t      nmatch = 1;
    regmatch_t  pmatch[1];  
    char        errbuf[100];  
    int     i, re;  
    FILE *fp;
    int file_size;
    char* html;

    fp = fopen("/home/yunting/post2.txt", "r");
    fseek( fp, 0, SEEK_END );         //将文件指针移动到文件尾
    file_size = ftell( fp );          //计算偏移位置来得到文件大小
    fseek( fp, 0, SEEK_SET );         //将文件指针移动到文件开头
    html = (char *)malloc( file_size*sizeof( char ) );
    fread( html, file_size, sizeof(char), fp );   //从文件读出内容到html
    fclose(fp);


    //Compile regex to reg    
    re  = regcomp(&reg, regex, REG_EXTENDED);  
    if ( 0 != re ) {  
        regerror(re, &reg, errbuf, 100);  
        printf("%s\n", errbuf);  
        exit(1);  
    }  
  
    //Use reg which compiled from pattern to match substring in whole string str  
    re  = regexec(&reg, html, nmatch, pmatch, 0);  
    if (REG_NOMATCH == re) {  
        printf("No match\n");  
        exit(1);
    } else if (0 == re) {  
        printf("matched:\n");  
        for (i = pmatch[0].rm_so; i < pmatch[0].rm_eo; ++i) {  
            putchar(html[i]);  
        }  
        printf("\n");  
        printf("%d, %d\n", pmatch[0].rm_so, pmatch[0].rm_eo);  
    }
  
    //Free &reg used in recomp  
    regfree(&reg);  
    return 0;  
}
