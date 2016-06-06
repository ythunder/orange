/*************************************************************************
	> File Name: mydb.h
	> Author: yunting
	> Mail: 1368282581@qq.com 
	> Created Time: 2016年06月01日 星期三 00时19分17秒
 ************************************************************************/

#ifndef __MY_DB_H

#include <mysql/mysql.h>
#include <stdlib.h>
#include <stdio.h>
#include <iostream>
#include <string.h>

using namespace std;

class CMyDB
{
public:
	CMyDB(const std::string server_host, const std::string user, const std::string passwd, const std::string db_name); //构造函数用来初始化数据库和连接数据库
	~CMyDB();   //析构函数用来做数据库的善后处理
	bool executeSQL(const std::string sql_str);  //对数据库进行SQL操作
	MYSQL_ROW getrow(void);   //获取行
	int getcolumn(void);  //获取列数
	void freeresult(void);  //释放保存在该连接的操作
	void print_result(void); //打印查询结果
	bool InsertData(const std::string &operation);  //插入数据
	bool ModifyData(const std::string &operation);  //修改数据
	bool DeleteData(const std::string &operation);  //删除数据

private:
	std::string server_ip;  //数据库地址
	std::string user_name;  //用户名
	std::string password;  //用户密码
	std::string database_name;  //数据库名
	MYSQL *connection;
	MYSQL_RES *res;
	MYSQL_ROW row;
};

#endif



