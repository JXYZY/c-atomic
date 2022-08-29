// atomic.cpp : �������̨Ӧ�ó������ڵ㡣
//

#include "stdafx.h"
#include <thread>
#include <iostream>
#include <mutex>
#include <atomic>
#include <vector>

//int value_test = 0;
std::atomic<int> value_test = 0;
std::mutex mtx;

//���������
void task_sisuo()
{
	for (int i = 0; i < 1000000; ++i)
	{
		mtx.lock();
		++value_test;
		--value_test;
		//�м����Σ�1��return
		//            2:throw�쳣��������ִ�н���
					 // 3:������������˳��ͬ  
		mtx.unlock();
	}
}
void task1()
{
	for (int i = 0; i < 1000000;++i)
	{
		std::lock_guard<std::mutex> lck(mtx);
		++value_test;
		--value_test;
	}
}

void task2()
{
	for (int i = 0; i < 1000000; ++i)
	{
		++value_test;
		--value_test;
	}
}

std::atomic<bool> ready(false);
std::atomic<bool> winner(false);
void count1m(int id)
{
	while(!ready){}
	for (int i = 0; i < 1000000; ++i) {}
	if (!winner.exchange(true))
	{
		std::cout << "thread #" << id << "won!\n";
	}
}

int main()
{
#if 0
	std::thread t1(task2);
	std::thread t2(task2);
	t1.join();
	t2.join();
	std::cout << "result value:" << value_test << std::endl;
#endif
#if 0
	//count1m���ڼ��˭�ȵ�100000,atomic��exchange��ı���βε�ֵ�����ҷ���֮ǰ��ֵ����atomic����ֵ�������̰߳�ȫ�ģ��ڲ��Զ����������ݾ���
	std::vector<std::thread> th;
	for (int i = 0; i < 10;++i)
	{
		th.push_back(std::thread(count1m,i));
	}
	ready = true;
	for (auto& t:th)
	{
		t.join();
	}
#endif
#if 0
	//���atomic�Ƿ���lock-free
	if (ready.is_lock_free())
	{
		//˵��ready������״̬���������������߳�
		std::cout << "ready is lock-free;" << std::endl;
	}
#endif
	std::cin.get();
    return 0;
}

