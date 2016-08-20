#include <iconv.h>
#include <iostream>
#include <cstring>
#include <memory>

#define OUTLEN 255

using namespace std;

// 代码转换操作类
class CodeConverter {
private:
iconv_t cd;
public:
// 构造
CodeConverter(const char *from_charset,const char *to_charset) {
cd = iconv_open(to_charset,from_charset);
}

// 析构
~CodeConverter() {
iconv_close(cd);
}

// 转换输出
int convert(char *inbuf,int inlen,char *outbuf,int outlen) {
char **pin = &inbuf;
char **pout = &outbuf;

memset(outbuf,0,outlen);
return iconv(cd,pin,(size_t *)&inlen,pout,(size_t *)&outlen);
}

int mainq(char *html);
};

int main()
{
char *in_utf8 = "正在安装";

char out[OUTLEN];
char out2[OUTLEN];

//utf-8-->gb2312
CodeConverter cc = CodeConverter("utf-8","gb2312");
cc.convert(in_utf8,strlen(in_utf8), out ,OUTLEN);
cout << "utf-8-->gb2312 in=" << in_utf8 << ",out=" << out << endl;
/*
// gb2312-->utf-8
CodeConverter cc2 = CodeConverter("gb2312","utf-8");
cc2.convert(out ,strlen(html),out2,OUTLEN);
cout << out << endl;
*/
return 0;
}
