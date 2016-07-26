/*************************************************************************
	> File Name: EUID.cpp
	> Author: ma6174
	> Mail: ma6174@163.com 
	> Created Time: 2016年07月17日 星期日 09时00分06秒
 ************************************************************************/

#include <unistd.h>
#include <stdio.h>

int main()
{
	uid_t uid = getuid();
	uid_t euid = geteuid();
	printf("userid is %d, effective userid is:%d\n", uid, euid);
	return 0;
}
