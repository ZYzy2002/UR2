#pragma once
#include <chrono>
using namespace std::chrono;

class Timer
{
public:
	Timer();
	float Mark();			//���ش˴�Mark���ϴ�Mark֮��� ʱ�������ü�ʱ��
	float Peek()const;		//���ش˴�Peek���ϴ�Mark֮��� ʱ���

private:
	std::chrono::steady_clock::time_point last;
};

