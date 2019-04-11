#include "ResetTimer.h"



ResetTimer::ResetTimer()
{
	time_t time_now = time(0);

#ifdef __unix__
	localtime_r(&time_now, &lastTime);
#elif defined(_WIN32) || defined(WIN32)
	localtime_s(&lastTime, &time_now);
#endif

}

bool ResetTimer::resetIsDue()
{
	time_t time_now = time(0);
	tm tm_now;

#ifdef __unix__
	localtime_r(&time_now, &tm_now);
#elif defined(_WIN32) || defined(WIN32)
	localtime_s(&tm_now, &time_now);
#endif

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
