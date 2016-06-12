/*************************************************************************
	> File Name: student.cpp
	> Author: yunting
	> Mail: 1368282581@qq.com 
	> Created Time: 2016年06月06日 星期一 19时21分22秒
 ************************************************************************/

#include "mysql.h"
#include <iostream>
using namespace std;

void menu()
{
	cout << "--------------------------------------------------" << endl;
	cout << "1.Add a student's information to databases" << endl;
	cout << "2.Delete a student's information to databases" << endl;
	cout << "3.Change a student's information to databases" << endl;
	cout << "4.Inquiry a student's information" << endl;
	cout << "-------------------------------------------------" << endl;
}


int main()
{
	int choice;
	for(;;) {
		menu();
		cout << "Select the function that you need:" ;
		cin >> choice;
		switch(choice) {
			case 1:	Add_stuinfo(); break;
			case 2: Del_stuinfo(); break;
			case 3: Cha_stuinfo(); break;
			case 4: Inq_stuinfo(); break;
			default: exit(1);
		}
	}
}
