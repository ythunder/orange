/*************************************************************************
	> File Name: gb2312-utf.c
	> Author: 
	> Mail: 
	> Created Time: 2016年08月18日 星期四 00时17分28秒
 ************************************************************************/

#include<stdio.h>
#include <iconv.h>
#include <string.h>
#define MAXLINE 1024

int main()
{
    int ret;
    char *inbuf = "���ڰ�װ";
    char *inbuf2 = "卢晓丹";
    size_t inbyte, outbyte;
    char out[1000];
    char *out_buf_p = out;
    iconv_t cd = iconv_open("gb2312", "UTF-8");
    //iconv_t cd = iconv_open("UTF-8", "gb2312");
    
    inbyte = 7;

    ret = iconv(cd, &inbuf2, &inbyte, &out_buf_p, &outbyte);
    //ret = iconv(cd, &inbuf, &inbyte, &out_buf_p, &outbyte);
    

        if (errno == EILSEQ) {
            printf("EILSEQ\n");
        } else if (errno == E2BIG) {
          printf("E2BIG\n");            
        } else if (errno == EINVAL){
            printf("EINVAL\n");
        }       

    

    printf("%d\n", ret);
    iconv_close(cd);
    printf("%s\n",out_buf_p);
    printf("\n");
    return 0;
}
