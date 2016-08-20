/*************************************************************************
	> File Name: post.cpp
	> Author: 
	> Mail: 
	> Created Time: 2016年08月16日 星期二 13时08分50秒
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

bool postUrl(const char *filename, std::string id, std::string passwd )
{
    CURL *curl;    
    CURLcode res;
    FILE *fp;
    std::string mess = "__VIEWSTATE=dDwxMTE4MjQwNDc1Ozs%2BYofaNxf5dpXqcC3ZAqYdKfPCdbw%3D&TextBox1=" 
        + id + "&TextBox2=" + passwd + "&RadioButtonList1=%D1%A7%C9%FA&Button1=+%B5%C7+%C2%BC+";
    

    if( (fp = fopen(filename, "w")) == NULL )
    {
        return false;
    }
    struct curl_slist *headers = NULL;
    headers = curl_slist_append(headers, "Accept:text/html,application/xhtml+xml,application/xml;q=0.9,image/webp,*/*;q=0.8" ); 
    headers = curl_slist_append(headers, "Accept-Encoding:gzip, deflate");
    headers = curl_slist_append(headers, "Accept-Language:zh-CN,zh;q=0.8");
    headers = curl_slist_append(headers, "ache-Control:no-cache");
    headers = curl_slist_append(headers, "Connection:keep-alive" ); 
    headers = curl_slist_append(headers, "Content-Type:application/x-www-form-urlencoded"  );
    headers = curl_slist_append(headers, "Host:222.24.62.120");
    headers = curl_slist_append(headers, "Origin:http://222.24.62.120");
    headers = curl_slist_append(headers, "Pragma:no-cache");
    headers = curl_slist_append(headers, "Referer:http://222.24.62.120/default4.aspx");
    headers = curl_slist_append(headers, "Upgrade-Insecure-Requests:1");
    headers = curl_slist_append(headers, "User-Agent:Mozilla/5.0 (X11; Linux x86_64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/52.0.2743.82 Safari/537.36");

    curl = curl_easy_init();

    if( curl )
    {
        curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);// 改协议头
        curl_easy_setopt(curl, CURLOPT_COOKIEJAR, "/home/yunting/cookie.txt");
        curl_easy_setopt(curl,CURLOPT_POSTFIELDS, mess.c_str());
        curl_easy_setopt(curl, CURLOPT_URL, "http://222.24.62.120/default4.aspx");  
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, fp);  
        res = curl_easy_perform(curl);
        curl_easy_cleanup(curl);
    }
    fclose(fp);
    return true;
}
