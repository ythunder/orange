/*************************************************************************
	> File Name: mydb.cpp
	> Author: yunting
	> Mail: 1368282581@qq.com 
	> Created Time: 2016年06月01日 星期三 00时25分39秒
 ************************************************************************/

#include "mydb.h"


/*关闭初始化连接数据库变量*/
YUNSQL::~YUNSQL() {
	if (connection != NULL) {
		mysql_close (connection);
	}
}



/*初始化数据库，数据库连接*/
YUNSQL::YUNSQL(const std::string server_host, const std::string user, const std::string passwd, const std::string db_name) {
	connection = mysql_init(NULL);
	if(connection == NULL) {
		perror("mysql_init\n");
		exit(1);
	}
	connection = mysql_real_connect(connection,server_host.c_str(), user.c_str(), passwd.c_str(), db_name.c_str(), 0, NULL, 0);

	if (connection == NULL) {
		perror("mysql_real_connect\n");
		exit(1);
	}
	
}


/*执行SQL查询类指令*/
bool YUNSQL::executeSQL(const std::string sql_str)
{
	mysql_query(connection, "set names utf8");
	mysql_query(connection, sql_str.c_str());   //执行命令sql_str
	res = mysql_use_result(connection);      //返回当前连接打开的执行结果，存入结果集res
	if(res) {
	   row = mysql_fetch_row(res);         //返回结果集的下一行信息
	   while (row != NULL) {        
			if(row <= 0) break;
			for(int r=0; r<mysql_num_fields(res); r++)    //返回一个结果集中列的数量 
				cout << row[r] << " ";
				cout << endl;
				row = mysql_fetch_row(res);
		}
	}
	mysql_free_result(res);  //用完一个结果集，释放由他使用的内存
}

/*执行SQL修改类命令*/
bool YUNSQL::reviseSQL(const std::string sql_str)
{
	mysql_query(connection, "set name utf8");
	mysql_query(connection, sql_str.c_str());
}



