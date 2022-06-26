#include "VideoPost.h"

VideoPost::VideoPost(int _ID, int _author, int _groupID, Timer _creationTime,
	std::vector<int> _likes, std::vector<Comment> _comments, 
	std::string _desription, std::string _videoPath):
	Post(_ID, _author, _groupID, _creationTime, _likes, _comments),
	description(_desription), videoPath(_videoPath) {}

void VideoPost::DisplayPost(sf::RenderWindow& window, sf::Font& font)
{
	Post::DisplayPost(window, font);

	GUIHelper::DrawText(window, font, "Dodal/a post wideo:",
		sf::Color::Black, 30, { 130, 160 });

	GUIHelper::DrawText(window, font, description,
		sf::Color::Black, 40, { 130, 200 });

	GUIHelper::DrawImage(window, videoPath, { 150, 280 });
}

std::string VideoPost::getDescription()
{
	return description;
}

std::string VideoPost::getVideoPath()
{
	return videoPath;
}
