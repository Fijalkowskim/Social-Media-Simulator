#pragma once
#include <string> 
#include "GUIHelper.h"

class Comment
{
private:
	std::string content;
	int ID, authorID;
public:
	Comment(int _ID, int _authorID, std::string _content);
	int getID();
	int getAuthorID();
	const std::string& getContent();

	void DisplayComment(sf::RenderWindow& window, sf::Font& font);

	friend bool operator== (const Comment& cL, const Comment& cR);
};

