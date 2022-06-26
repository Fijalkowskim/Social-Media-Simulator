#include "ImagePost.h"

ImagePost::ImagePost(int _ID, int _author, int _groupID, Timer _creationTime, std::vector<int> _likes, std::vector<Comment> _comments, std::string _desription, std::string _imagePath) :
	Post(_ID, _author, _groupID, _creationTime, _likes, _comments), description(_desription), imagePath(_imagePath){}


void ImagePost::DisplayPost(sf::RenderWindow& window, sf::Font& font)
{
	Post::DisplayPost(window, font);

	GUIHelper::DrawImage(window, imagePath, { 150, 280 });
	GUIHelper::DrawText(window, font, description,
		sf::Color::Black, 40, { 130, 200 });


}

std::string ImagePost::getDescription()
{
	return description;
}

std::string ImagePost::getImagePath()
{
	return imagePath;
}


