#include "Framework.h"
#include "Time.h"

bool Time::isTimerStopped = true;
float Time::timeElapsed = 0.0f;

Time::Time(void) :
	ticksPerSecond(0), currentTime(0), lastTime(0), lastFPSUpdate(0), fpsUpdateInterval(0),
	frameCount(0), runningTime(0), framePerSecond(0)
{
	// 초당 카운트수(주파수)를 저장, fps 업데이트 간격은 주파수 절반.
	QueryPerformanceFrequency((LARGE_INTEGER *)&ticksPerSecond);
	fpsUpdateInterval = ticksPerSecond >> 1;
}

Time::~Time(void)
{

}

// lastTime과 currentTime의 카운트 차이를 이용하여 경과시간을 나타내고 프레임 카운트 증가
// 지난 시간이 fps 갱신 간격보다 크면 새로 fps 갱신해준다. 
void Time::Update()
{
	if (isTimerStopped) return;

	QueryPerformanceCounter((LARGE_INTEGER *)&currentTime);
	timeElapsed = (float)(currentTime - lastTime) / (float)ticksPerSecond;
	runningTime += timeElapsed;


	frameCount++;
	if (currentTime - lastFPSUpdate >= fpsUpdateInterval)
	{
		float tempCurrentTime = (float)currentTime / (float)ticksPerSecond;
		float tempLastTime = (float)lastFPSUpdate / (float)ticksPerSecond;
		framePerSecond = (float)frameCount / (tempCurrentTime - tempLastTime);

		lastFPSUpdate = (INT64)currentTime;
		frameCount = 0;
	}

	lastTime = currentTime;
}

// 타이머를 시작하고 그때의 카운트값을 lastTime에 넣어준다.
void Time::Start()
{
	if (!isTimerStopped)
		assert(false);

	QueryPerformanceCounter((LARGE_INTEGER *)&lastTime);
	isTimerStopped = false;
}

// 타이머 정지, 경과 시간을 구해준다.
void Time::Stop()
{
	if (isTimerStopped)
		assert(false);

	INT64 stopTime = 0;
	QueryPerformanceCounter((LARGE_INTEGER *)&stopTime);
	runningTime += (float)(stopTime - lastTime) / (float)ticksPerSecond;
	isTimerStopped = true;
}

bool Time::Timer(INT64 before, float& time, bool isStop, OUT float& elapsed)
{
	elapsed = (float)(currentTime - before) / (float)ticksPerSecond;

	if (elapsed > time)
		return true;
	else
	{
		if (isStop)
		{
			// time 값 변경
			time -= elapsed;
		}
		return false;
	}
		

}

INT64 Time::GetBefore()
{
	INT64 stopTime = 0;
	QueryPerformanceCounter((LARGE_INTEGER *)&stopTime);
	return stopTime;
}
