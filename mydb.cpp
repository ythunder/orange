/*************************************************************************
	> File Name: mydb.cpp
	> Author: yunting
	> Mail: 1368282581@qq.com 
	> Created Time: 2016年06月01日 星期三 00时25分39秒
 ************************************************************************/

#include "mydb.h"


/*关闭初始化连接数据库变量*/
CMyDB::~CMyDB() {
	if (connection != NULL) {
		mysql_close (connection);
	}
}



/*初始化数据库，数据库连接*/
CMyDB::CMyDB(const std::string server_host, const std::string user, const std::string passwd, const std::string db_name) {
	
	server_ip = server_host;
	user_name = user;
	password = passwd;
	database_name = db_name;
/*
	connection = mysql_real_connect(connection, server_ip.c_str(), user_name.c_str(), password.c_str(), database_name.c_str(), 0, NULL, 0);
	if (connection == NULL) {
		perror("mysql_real_connect\n");
		exit(1);
	}
	connection = mysql_init(NULL);
	if(connection == NULL) {
		perror("mysql_init\n");
		exit(1);
	}
	*/
}

/*打印查询结果*/
void CMyDB::print_result(void) {
	while( (row = mysql_fetch_row(res)) != NULL) {
		for(int i=0; i<mysql_num_fields(res); i++) {
			cout << row[i] << "  ";
		}
		cout << endl;
	}
}

MYSQL_ROW CMyDB::getrow(void) {
	return mysql_fetch_row(res);
}


int CMyDB::getcolumn(void) {
	return mysql_num_fields(res);
}




/*执行SQL语句*/
bool CMyDB::executeSQL(string sql_str) {
	//查询编码设定
	if (mysql_query(connection, "set names utf8")) {   //执行由第二个参数指向的SQL语句
		fprintf(stderr, "%d: %s\n", mysql_errno(connection), mysql_error(connection));
	}
	int t = mysql_query(connection, sql_str.c_str());
	if (t) {
		printf("Error making query: %s \n", mysql_error(connection));
		exit(1);
	}
	else {
		res = mysql_use_result(connection);   //初始化逐行的结果集检索
		if (res) {
			for (int i = 0; i < mysql_field_count(connection); i++) {    //返回上次执行语句的结果集的列数
				row = mysql_fetch_row(res);   //从结果集中获取下一行
				if(row <= 0) {
					break;
				}
				for (int r = 0; r < mysql_num_fields(res); r++) {   //返回结果集中的字段数
					cout << row[r] << "  ";
				}
					cout << endl;
			}
		}
		mysql_free_result(res);
	}
	return true;
}


int main(int argc, char **argv)
{
	CMyDB my("127.0.0.1", "root", "yunting13", "student");
	//my.executeSQL("DELETE FROM student_info where stu_score=85");
	//my.executeSQL("SELECT * FROM student_info");
	//my.print_result();
	return 0;
}

