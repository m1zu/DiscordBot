#pragma once
#include <ctime>

class ResetTimer
{
public:
	ResetTimer();
	~ResetTimer();

public:
	bool resetIsDue();
	
private:
	tm lastTime;
};

