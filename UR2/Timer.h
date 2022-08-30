#pragma once
#include <chrono>
using namespace std::chrono;

class Timer
{
public:
	Timer();
	float Mark();			//返回此次Mark和上次Mark之间的 时间差；即重置计时器
	float Peek()const;		//返回此次Peek和上次Mark之间的 时间差

private:
	std::chrono::steady_clock::time_point last;
};

