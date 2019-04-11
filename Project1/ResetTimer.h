#pragma once
#include<ctime>

class ResetTimer
{
public:
	ResetTimer();

public:
	bool resetIsDue();
	
private:
	tm lastTime;
};

