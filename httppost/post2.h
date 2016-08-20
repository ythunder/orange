/*************************************************************************
	> File Name: post2.cpp
	> Author: 
	> Mail: 
	> Created Time: 2016年08月16日 星期二 18时08分14秒
 ************************************************************************/


#include <stdio.h>
#include <iostream>
#include <curl/curl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <fstream>

using namespace std;

bool postUrl_two(const char *filename,std::string id)
{
    CURL *curl;    
    CURLcode res;
    FILE *fp;
 //   std::string mess1 ="Referer: http://222.24.62.120/xs_main.aspx?xh=" + id;
    std::string mess2 = "http://222.24.62.120/lw_xsxx.aspx?xh=" + id;
    std::string mess1 = "Referer: "+mess2;
    if((fp = fopen(filename,"w")) == NULL) 
    {
        return false;
    }
    struct curl_slist *headers = NULL;
    headers = curl_slist_append(headers, "Accept:text/html,application/xhtml+xml,application/xml;q=0.9,image/webp,*/*;q=0.8" ); 
    headers = curl_slist_append(headers, "Accept-Encoding:gzip, deflate,sdch");
    headers = curl_slist_append(headers, "Accept-Language:zh-CN,zh;q=0.8");
    headers = curl_slist_append(headers, "Cache-Control:no-cache");
    headers = curl_slist_append(headers, "Connection:keep-alive" ); 
    
    headers = curl_slist_append(headers, "Host:222.24.62.120");
    headers = curl_slist_append(headers, "Pragma:no-cache");
    headers = curl_slist_append(headers, mess1.c_str());
    headers = curl_slist_append(headers, "Upgrade-Insecure-Requests:1");
    headers = curl_slist_append(headers, "User-Agent:Mozilla/5.0 (X11; Linux x86_64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/52.0.2743.82 Safari/537.36");

    curl = curl_easy_init();

    if( curl )
    {
        curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);// 改协议头
        curl_easy_setopt(curl, CURLOPT_COOKIEFILE, "/home/yunting/cookie.txt");
      curl_easy_setopt(curl, CURLOPT_URL, mess2.c_str());
      
     //   curl_easy_setopt(curl, CURLOPT_URL, "xh=04141114&xm=%C2%AC%CF%FE%B5%A4&gnmkdm=N121902");
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, fp);  
        res = curl_easy_perform(curl);
        curl_easy_cleanup(curl);
    }
    fclose(fp);
    return true;
}
