#include "TextPost.h"

TextPost::TextPost(int _ID, int _author, int _groupID, Timer _creationTime, std::vector<int> _likes, std::vector<Comment> _comments, std::string text):
	Post(_ID,_author, _groupID, _creationTime, _likes, _comments), textContent(text){}

void TextPost::DisplayPost(sf::RenderWindow& window, sf::Font& font)
{ 
	Post::DisplayPost(window, font);
	GUIHelper::DrawText(window, font, textContent,
		sf::Color::Black, 40, { 130, 200 });
}

std::string TextPost::getContent()
{
	return textContent;
}
