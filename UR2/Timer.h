#pragma once
#include <chrono>
using namespace std::chrono;

class Timer
{
	using timepoint = std::chrono::steady_clock::time_point;
public:
	Timer();

private:
	std::chrono::steady_clock::time_point gameBegin;
	std::chrono::steady_clock::time_point lastFrame;
	std::chrono::steady_clock::time_point thisFrame;
	float Subtract(timepoint a, timepoint b);// b - a
	float deltatime;
public:
	float GameTime();
	float DeltaTime();
	void Tick();
};

