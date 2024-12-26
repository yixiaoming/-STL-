#pragma once 
#include <iostream>
#include "speechManager.h"
using namespace std;
int main()
{
	//���������
	srand((unsigned int)time(NULL));
	//�������������
	SpeechManager sm;

	int choice = 0;

	while (true)
	{
		sm.show_Menu();
		cout << "���������ѡ��" << "\n";
		cin >> choice;
		switch (choice) 
		{
		case 1:            //��ʼ����
			sm.startSpeech();
			break;
		case 2:            //�鿴�����¼��
			sm.showRecord();
			break;
		case 3:            //��ձ�����¼
			sm.clearRecord();
			break;

		case 0:            //�˳�ϵͳ
			sm.exitSystem();
			break;
		default:
			cout << "����������������룡" << endl;
			break;
		}
			
	}
	return 0;
}