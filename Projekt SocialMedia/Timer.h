#pragma once
#include <iostream>
#include <string>
#include <time.h>
#include <sstream>
#include <iomanip>

class Timer
{
private:
	std::tm creationTime;
	int year, month, day, hour, min, sec;
public:
	Timer();
	Timer(int year, int month, int day, int hour, int min, int sec);
	~Timer();

	std::string getTimeString();
	std::string getTimeFileFormat();
};

