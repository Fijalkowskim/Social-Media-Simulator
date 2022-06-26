#pragma once
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>

class GUIHelper
{
public:
	static void DrawImage(sf::RenderWindow& window, const std::string pngPath, sf::Vector2f position);
	static void DrawImage(sf::RenderWindow& window, const std::string pngPath, sf::Vector2f position,
		sf::Vector2f scale);
	static void DrawImage(sf::RenderWindow& window, const std::string pngPath, sf::Vector2f position, 
		sf::Vector2f scale, sf::Sprite& sprite);
	static void DrawText(sf::RenderWindow& window, sf::Font& font, const std::string content, 
		sf::Color color, unsigned int size, sf::Vector2f position);
	static void DrawText(sf::RenderWindow& window, sf::Font& font, const std::string content,
		sf::Color color, unsigned int size, sf::Vector2f position, sf::Text::Style style);
	static void PlayAudio(const std::string audioPath);


	static sf::Vector2f MousePosition(sf::RenderWindow& window);

	static bool isMouseOverSprite(sf::RenderWindow& window, sf::Sprite& sprite);

};

