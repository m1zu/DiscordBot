#include "ResetTimer.h"



ResetTimer::ResetTimer()
{
	time_t time_now = time(0);
	localtime_s(&lastTime, &time_now);
}


ResetTimer::~ResetTimer()
{
}

bool ResetTimer::resetIsDue()
{
	time_t time_now = time(0);
	tm tm_now;
	localtime_s(&tm_now, &time_now);
	if (tm_now.tm_wday < lastTime.tm_wday)
	{
		lastTime = tm_now;
		return true;
	}
	else
	{
		lastTime = tm_now;
		return false;
	}
}
