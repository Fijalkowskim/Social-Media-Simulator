#pragma once
#include <SFML/Graphics.hpp>
#include <iostream>
#include <vector>
#include <functional>
#include "GUIHelper.h"


class Button
{
public:
	enum State
	{
		Idle, HoverOver, Inactive
	};
private:
	void SetButtonPos(sf::Vector2f pos);
	void SetButtonCenterPos(sf::Vector2f pos);
	void ChangeButtonColor(sf::Color color);

	bool isMouseOver(sf::RenderWindow& window);
	void DrawButton(sf::RenderWindow& window);
protected:
	sf::RectangleShape shape;
	sf::Color basicColor, hoverColor;
	sf::Text text;

	std::function<void()> onClick;

	State state;

public:

	Button(std::function<void()> _onClick, sf::Vector2f size, sf::Vector2f position, bool alignByCenter,
		sf::Color basicColor, sf::Color hoverColor,const std::string _text, sf::Color textColor, 
		unsigned int textSize, sf::Font& font);
	Button();
	~Button();

	virtual void VisualUpdate(sf::RenderWindow& window);
	virtual void MouseOverCheck(sf::RenderWindow& window);

	void MouseClick(sf::RenderWindow& window);

	void ShowButton();
	void HideButton();
};

