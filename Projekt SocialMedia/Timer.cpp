#include "Timer.h"

Timer::Timer(): creationTime{}
{
	time_t now = std::time(0);
	localtime_s(&creationTime, &now);
	year = creationTime.tm_year + 1900;
	month = creationTime.tm_mon + 1;
	day = creationTime.tm_mday;
	hour = creationTime.tm_hour;
	min = creationTime.tm_min;
	sec = creationTime.tm_sec;
}

Timer::Timer(int _year, int _month, int _day, int _hour, int _min, int _sec):
	year(_year),month(_month),day(_day),hour(_hour),min(_min),sec(_sec)
{
    creationTime.tm_year = year - 1900;

	if (month >= 1 && month <= 12)
		creationTime.tm_mon = month - 1;
	else
		creationTime.tm_mon = 0;

	if (month == 1 || month == 3 || month == 5 || month == 7 || month == 8 || month == 10 || month == 12)
	{
		if (day >= 1 && day <= 31)
			creationTime.tm_mday = day;
		else
			creationTime.tm_mday = 1;
	}
	else if (month != 2)//Feb
	{
		if (day >= 1 && day <= 30)
			creationTime.tm_mday = day;
		else
			creationTime.tm_mday = 1;
	}
	else if (day >= 1 && day <= 28)
		creationTime.tm_mday = day;
	else
		creationTime.tm_mday = 1;

	if (hour >= 0 && hour <= 23)
		creationTime.tm_hour = hour;
	else
		creationTime.tm_hour = 0;

	if (min >= 0 && min <= 59)
		creationTime.tm_min = min;
	else
		creationTime.tm_min = 0;

	if (sec >= 0 && sec <= 60)
		creationTime.tm_sec = sec;
	else
		creationTime.tm_sec = 0;

}

Timer::~Timer()
{
}

std::string Timer::getTimeString()
{
	std::stringstream ss;
	ss << std::put_time(&creationTime, "%F %T");
	return ss.str();
}

std::string Timer::getTimeFileFormat()
{
	return std::to_string(year) + "\n" + std::to_string(month) + "\n" +
		std::to_string(day) + "\n" + std::to_string(hour) + "\n" + std::to_string(min) + "\n" + std::to_string(sec) + "\n";
}
