#pragma once 
#include <iostream>
#include "speechManager.h"
using namespace std;
int main()
{
	//随机数种子
	srand((unsigned int)time(NULL));
	//创建管理类对象
	SpeechManager sm;

	int choice = 0;

	while (true)
	{
		sm.show_Menu();
		cout << "请输入你的选择：" << "\n";
		cin >> choice;
		switch (choice) 
		{
		case 1:            //开始比赛
			sm.startSpeech();
			break;
		case 2:            //查看往届记录；
			sm.showRecord();
			break;
		case 3:            //清空比赛记录
			sm.clearRecord();
			break;

		case 0:            //退出系统
			sm.exitSystem();
			break;
		default:
			cout << "输入错误，请重新输入！" << endl;
			break;
		}
			
	}
	return 0;
}