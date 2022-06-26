#include "AudioPost.h"

AudioPost::AudioPost(int _ID, int _author, int _groupID, Timer _creationTime, std::vector<int> _likes, std::vector<Comment> _comments, std::string _audioPath):
	Post(_ID, _author, _groupID, _creationTime, _likes, _comments), audioPath(_audioPath) {}

void AudioPost::DisplayPost(sf::RenderWindow& window, sf::Font& font)
{
	Post::DisplayPost(window, font);
	GUIHelper::DrawText(window, font, "Dodal/a post audio:",
		sf::Color::Black, 40, { 130, 200 });
	GUIHelper::DrawImage(window, "Resources/Pictures/Audio.png", { 150, 280 });
}

std::string AudioPost::getAudioPath()
{
	return audioPath;
}
