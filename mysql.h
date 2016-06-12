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

class YUNSQL
{
public:
	YUNSQL(const std::string server_host, const std::string user, const std::string passwd, const std::string db_name); //构造函数用来初始化数据库和连接数据库
	~YUNSQL();   //析构函数用来做数据库的善后处理
	bool executeSQL(const std::string sql_str);  //对数据库进行SQL查询操作
	bool reviseSQL(const std::string sql_str);  //对数据库进行SQL修改操作
private:
	MYSQL *connection;
	MYSQL_RES *res;
	MYSQL_ROW row;
};

#endif



