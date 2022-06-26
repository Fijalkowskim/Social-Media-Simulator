#include "Event.h"
#include "User.h"
#include "Post.h"

Event::Event() : Event(DataManager::getNewGroupID(), "Empty Event", "", Timer()) {};
Event::Event(int _ID, std::string _name, std::string _description, Timer _eventDate) : Group(_ID, _name, _description), eventDate(_eventDate) {}
Event::Event(int _ID, std::string _name, std::string _description, Timer _eventDate, std::vector<int> _users):
	Group(_ID, _name, _description), eventDate(_eventDate)
{
	for (size_t i = 0; i < _users.size(); i++)
	{
		AddUser(_users[i]);
	}
}

Event::~Event()
{
}


void Event::DisplayGroup(sf::RenderWindow& window, sf::Font& font)
{
	Group::DisplayGroup(window, font);
	GUIHelper::DrawText(window, font, "Data wydarzenia: " + eventDate.getTimeString(),
		sf::Color::Black, 50, {130, 330});
}
