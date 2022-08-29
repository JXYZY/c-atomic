// atomic.cpp : 定义控制台应用程序的入口点。
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

//死锁的情况
void task_sisuo()
{
	for (int i = 0; i < 1000000; ++i)
	{
		mtx.lock();
		++value_test;
		--value_test;
		//中间程序段：1：return
		//            2:throw异常，来不及执行解锁
					 // 3:两个函数加锁顺序不同  
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
	//count1m用于检测谁先到100000,atomic的exchange会改变成形参的值，并且返回之前的值，且atomic的数值对象是线程安全的，内部自动避免了数据竞争
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
	//检测atomic是否是lock-free
	if (ready.is_lock_free())
	{
		//说明ready是无锁状态，不会阻塞其他线程
		std::cout << "ready is lock-free;" << std::endl;
	}
#endif
	std::cin.get();
    return 0;
}

