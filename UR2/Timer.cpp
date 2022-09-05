#include "Timer.h"

Timer::Timer()
{
	gameBegin = steady_clock::now();
}

float Timer::Subtract(timepoint a, timepoint b)
{
	const duration<float> frameTime = b - a;
	return frameTime.count();
}

float Timer::GameTime()
{
	return Subtract(gameBegin, thisFrame);
}

float Timer::DeltaTime()
{
	return deltatime;
}

void Timer::Tick()
{
	lastFrame = thisFrame;
	thisFrame = steady_clock::now();
	deltatime = Subtract(lastFrame, thisFrame);
}

