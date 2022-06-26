#pragma once
#include "Group.h"
#include "Timer.h"

class Event:
	public Group
{
private:
	Timer eventDate;
public:
	Event();
	Event(int _ID, std::string _name, std::string _description, Timer _eventDate);
	Event(int _ID, std::string _name, std::string _description, Timer _eventDate,
		std::vector<int> _users);
	~Event();

	void DisplayGroup(sf::RenderWindow& window, sf::Font& font) override;
};

