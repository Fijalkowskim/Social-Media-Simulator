#include "Comment.h"
#include"DataManager.h"

Comment::Comment(int _ID, int _authorID, std::string _content): ID(_ID),authorID(_authorID), content(_content)
{
}

void Comment::DisplayComment(sf::RenderWindow& window, sf::Font& font)
{
	GUIHelper::DrawText(window, font, "KOMENTARZE", sf::Color::Blue, 80, { 600, 10 });
	GUIHelper::DrawImage(window, DataManager::getUser(authorID).getProfilePicPath(), { 610, 280 });
	GUIHelper::DrawText(window, font, DataManager::getUser(authorID).getNameAndSurname(), 
		sf::Color::Black, 50, {730, 280});
	GUIHelper::DrawText(window, font, content, sf::Color::Black, 60, { 730, 350 });
}

int Comment::getID()
{
	return ID;
}
int Comment::getAuthorID()
{
	return authorID;
}
const std::string& Comment::getContent()
{
	return content;
}
bool operator==(const Comment& cL, const Comment& cR)
{
	return cL.ID == cR.ID && cL.authorID == cR.authorID;
}
