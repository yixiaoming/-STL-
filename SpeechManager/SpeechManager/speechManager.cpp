#include "speechManager.h"
#include <windows.h>

//构造函数
SpeechManager::SpeechManager()
{
	//初始化容器和属性
	this->initSpeech();

	//创建12名选手
	this->createSpeaker();

	//加载往届记录
	//this->loadRecord();
}


//析构函数
SpeechManager::~SpeechManager()
{

}


void SpeechManager::show_Menu()
{
	cout << "********************************************" << endl;
	cout << "*************  欢迎参加演讲比赛 ************" << endl;
	cout << "*************  1.开始演讲比赛  *************" << endl;
	cout << "*************  2.查看往届记录  *************" << endl;
	cout << "*************  3.清空比赛记录  *************" << endl;
	cout << "*************  0.退出比赛程序  *************" << endl;
	cout << "********************************************" << endl;
	cout << endl;
}

//初始化
void SpeechManager::initSpeech()
{
	//容器都置空
	this->v1.clear();
	this->v2.clear();
	this->vVictory.clear();
	this->m_Speaker.clear();

	//初始化比赛轮数
	this->m_Index = 1;

	//将记录的容器 也清空
	this->m_Record.clear();
}

//创建12名比赛选手
void SpeechManager::createSpeaker()
{
	string nameSeed = "ABCDEFGHIJKL";
	for (int i = 0; i < nameSeed.size(); i++)
	{
		string name = "选手";
		name = name + nameSeed[i];    //选手A 选手B 选手C .....

		//创建具体选手
		Speaker sp;
		sp.m_Name = name;

		for (int j = 0; j < 2; j++)
		{
			sp.m_Score[j] = 0;
		}

		//创建选手的编号，并且放在v1容器内；
		this->v1.push_back(i + 10001);

		//选手编号以及对应选手 放入到map容器中
		this->m_Speaker.insert(make_pair(i + 10001, sp));
	}	
}


//开始比赛
void SpeechManager::startSpeech()   
{
	//第一轮比赛
	//1.抽签
	this->speechDraw();
	
	//2.开始比赛
	this->speechContest();

	//3、显示晋级结果
	this->showScore();


	//第二轮比赛开始
	this->m_Index++;
	//1、抽签
	this->speechDraw();
	//2、比赛
	speechContest();
	//3、显示最终结果
	this->showScore();
	//4、保存分数到文件中
	this->saveRecord();
	

	//重置比赛 ，获取记录
	//初始化容器和属性
	this->initSpeech();

	//创建12名选手
	this->createSpeaker();

	//加载往届记录
	this->loadRecord();

	cout << "本届比赛完毕！" << endl;
	system("pause");
	system("cls");
}


//抽签
void SpeechManager::speechDraw()
{
	cout << "第" << this->m_Index << "轮选手正在抽签" << "\n";
	cout << "抽签后演讲顺序如下： " << "\n";
	if (this->m_Index == 1)
	{
		//this->m_Index == 1 表示第一轮比赛
		random_shuffle(v1.begin(), v1.end());
		for (vector<int>::iterator it = v1.begin(); it != v1.end(); it++)
		{
			cout << *it << " ";         //*it 是 解引用操作符，它的作用是访问迭代器 it 所指向的元素。
			                            //迭代器本质上是指针，指向容器中的元素。因此，* it 就是获取迭代器 it 指向的具体元素。
		}
		cout << endl;
	}
	else
	{
		//第二轮比赛
		random_shuffle(v2.begin(), v2.end());
		for (vector<int>::iterator it = v2.begin(); it != v2.end(); it++)
		{
			cout << *it << " ";
		}
		cout << endl;
	}
	cout << "-----------------------" << endl;
	cout << endl;
}

//开始比赛
void SpeechManager::speechContest()
{
	cout << "----------- 第" << this->m_Index << " 轮比赛正式开始 --------------" << endl;
	Sleep(2000);

	//准备临时容器 存放小组成绩
	multimap<double, int, greater<double>> groupScore;

	int num = 0;  //记录人员个数 6人一组

	vector<int>v_Src;   //比赛选手容器

	if (this->m_Index == 1)  //第一轮比赛；
	{
		v_Src = v1;
	}
	else
	{
		v_Src = v2;
	}

	//遍历所有选手进行比赛
	for (vector<int>::iterator it = v_Src.begin(); it != v_Src.end(); it++)
	{
		num++;
		//评委打分
		deque<double>d;   //队列     存放评委打的分数
		for (int i = 0; i < 10; i++)                 //10个评委打分
		{
			double score = (rand() % 401 + 600) / 10.f;   // 60 ~ 100
			cout << score << " ";
			d.push_back(score);
		}
		cout << endl;

		sort(d.begin(), d.end(), greater<double>());   //排序
		d.pop_front(); //去除最高分
		d.pop_back(); //去除最低分

		double sum = accumulate(d.begin(), d.end(), 0.0f);   //总分
		double avg = sum / (double)d.size();   //平均分


		//打印平均分
		cout << "编号： " << *it << " 姓名： " << this->m_Speaker[*it].m_Name << " 获取平均分： " << avg << endl;

		//将平均分放入到 map容器里
		this -> m_Speaker[*it].m_Score[this->m_Index - 1] = avg;

		//将打分数据，放到临时小组容器中
		groupScore.insert(make_pair(avg, *it));  //key是得分，value是具体选手编号；

		//取出前6名 每6人取出前3名，因为之前将12人分成了两组
		if (num % 6 == 0)
		{
			cout << "第" << num / 6 << "小组比赛名次： " << endl;
			for (multimap<double, int, greater<double>>::iterator it = groupScore.begin(); it != groupScore.end(); it++)
			{
				cout << "编号：" << it->second << "姓名：" << this->m_Speaker[it->second].m_Name << "成绩："
					<< this->m_Speaker[it->second].m_Score[this->m_Index - 1] << endl;
			}

			//取出前3名
			int count = 0;
			for (multimap<double, int, greater<double>>::iterator it = groupScore.begin(); it != groupScore.end() && count < 3; it++, count++)
			{
				if (this->m_Index == 1)
				{
					v2.push_back((*it).second);    //second是value，也就是分数；first是键(key),也就是姓名编号；
				}
				else
				{
					vVictory.push_back((*it).second);
				}
			}

			groupScore.clear(); //小组容器清空
			cout << endl;
		}
	}
	cout << "------------------ 第" << this->m_Index << "轮比赛完毕 --------------" << endl;
}


//显示分数
void SpeechManager::showScore()
{	
	Sleep(2000);
	cout << "------------------ 第" << this->m_Index << "轮晋级选手信息如下： -------------------" << endl;

	vector<int>v;
	if (this->m_Index == 1)
	{
		v = v2;
	}
	else
	{
		v = vVictory;
	}

	for (vector<int>::iterator it = v.begin(); it != v.end(); it++)
	{	
		Sleep(2000);
		cout << "选手编号：" << *it << " 姓名：" << this->m_Speaker[*it].m_Name << " 得分：" <<
			this->m_Speaker[*it].m_Score[this->m_Index - 1] << endl;
	}
	cout << endl;
	system("pause");
	system("cls");
}


//保存记录
void SpeechManager::saveRecord()
{
	ofstream ofs;   //在C++中用于输出文件的流类，用于向文件中写入数据。 ofs是ofstream类型的对象，它将用于操作文件。
	ofs.open("E:\C++\SpeechManager\SpeechManager\speech.csv", ios::out | ios::app);   //用追加的方式写文件；

	//将每个选手数据 写入到文件中
	for (vector<int>::iterator it = vVictory.begin(); it != vVictory.end(); it++)
	{
		ofs << *it << "," << this->m_Speaker[*it].m_Score[1] << ",";
	}
	ofs << endl;

	//关闭；
	ofs.close();
	cout << "记录已保存！" << endl;

	//更改文件不为空状态   通常在打开文件并写入内容后，会将 fileIsEmpty 设置为 false，表示文件已经有内容。
	this->fileIsEmpty = false;
}


//读取记录
void SpeechManager::loadRecord()
{
	ifstream ifs("E:\C++\SpeechManager\SpeechManager\speech.csv", ios::in);  //读取文件
	
	if (!ifs.is_open())
	{
		this->fileIsEmpty = true;
		cout << "文件不存在" << endl;
		ifs.close();
		return;
	}

	//文件清空情况
	char ch;
	ifs >> ch;
	if (ifs.eof())
	{
		cout << "文件为空" << endl;
		this->fileIsEmpty = true;
		ifs.close();
		return;
	}

	//文件不为空
	this->fileIsEmpty = false;
	ifs.putback(ch); // 将上面读取的单个字符 放回来

	string data;
	int index = 0;

	while (ifs >> data)
	{	
		cout << data << endl;
		vector<string>v; //存放6个string字符串

		int pos = -1;//查到","位置的变量
		int start = 0;

		while (true)
		{
			pos = data.find(",", start);
			if (pos == -1)
			{
				//没有找到情况
				break;
			}
			string temp = data.substr(start, pos - start);
			cout << temp << endl;
			v.push_back(temp);
			start = pos + 1;
		}

		this->m_Record.insert(make_pair(index, v));
		index++;
	}
	ifs.close();

	for (map<int, vector<string>>::iterator it = m_Record.begin(); it != m_Record.end(); it++)
	{
		cout << it->first+1 << "冠军编号： " << it->second[0] << " 分数： " << it->second[1] << endl;
	}
}


//查看往届记录
void SpeechManager::showRecord()   
{
	if (this->fileIsEmpty)
	{
		cout << "文件为空或者文件不存在！" << endl;
	}
	else
	{
		for (int i = 0; i < this->m_Record.size(); i++)
		{
			cout << "第" << i + 1 << "届" << endl;
			cout << "冠军编号：" << this->m_Record[i][0] << " 得分： " << this->m_Record[i][1] << " "
				 << "亚军编号：" << this->m_Record[i][2] << " 得分： " << this->m_Record[i][3] << " "
				 << "季军编号：" << this->m_Record[i][4] << " 得分： " << this->m_Record[i][5] << endl;
		}
	}
	system("pause");
	system("cls");

}


//清空比赛记录
void SpeechManager::clearRecord()  
{
	cout << "是否确定清空文件？" << endl;
	cout << "1、是" << endl;
	cout << "2、否" << endl;

	int select = 0;
	cin >> select;

	if (select == 1)
	{
		//确认清空
		ofstream ofs("speech.csv", ios::trunc);
		ofs.close();

		// 清空比赛记录容器
		this->m_Record.clear();

		//初始化容器和属性
		this->initSpeech();

		//创建12名选手
		this->createSpeaker();

		//加载往届记录
		this->loadRecord();

		cout << "清空成功！" << endl;
	}

	system("pause");
	system("cls");
}

//退出系统
void SpeechManager::exitSystem()
{
	cout << "欢迎下次使用" << endl;
	system("pause");
	exit(0);
}