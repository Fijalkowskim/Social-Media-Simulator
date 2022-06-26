#include"Button.h"

Button::~Button(){}
Button::Button(std::function<void()> _onClick, sf::Vector2f size, sf::Vector2f position, bool alignByCenter,
	sf::Color _basicColor, sf::Color _hoverColor,const std::string _text, sf::Color textColor, 
	unsigned int textSize, sf::Font& font):
	onClick(_onClick), state(Inactive), basicColor(_basicColor), hoverColor(_hoverColor)
{
	shape.setSize(size);
	shape.setFillColor(basicColor);

	text.setFont(font);
	text.setString(_text);
	text.setFillColor(textColor);
	text.setCharacterSize(textSize);

	if (alignByCenter)
		SetButtonCenterPos(position);
	else
		SetButtonPos(position);
}

Button::Button()
{
}

void Button::VisualUpdate(sf::RenderWindow& window)
{
	switch (state)
	{
	case Button::Idle:
		ChangeButtonColor(basicColor);
		break;
	case Button::HoverOver:
		ChangeButtonColor(hoverColor);
		break;
	case Button::Inactive:
		return;
	}
	DrawButton(window);
}

void Button::MouseOverCheck(sf::RenderWindow& window)
{
	if (state == Idle && isMouseOver(window))
	{
		state = HoverOver;
	}
	if (state == HoverOver && !isMouseOver(window))
	{
		state = Idle;
	}
}

void Button::MouseClick(sf::RenderWindow& window)
{
	if (state == HoverOver)
	{
		onClick();
	}
}

void Button::ShowButton()
{
	state = Idle;
}

void Button::HideButton()
{
	state = Inactive;
}


void Button::SetButtonPos(sf::Vector2f pos)
{
	shape.setPosition(pos);
	float textXPos = (pos.x + shape.getGlobalBounds().width / 2.0) - (text.getGlobalBounds().width / 2.0);
	float textYPos = (pos.y + shape.getGlobalBounds().height / 2.0) - (text.getGlobalBounds().height / 2.0);
	text.setPosition({ textXPos, textYPos });
}
void Button::SetButtonCenterPos(sf::Vector2f pos)
{
	float shapeXPos = pos.x - shape.getGlobalBounds().width / 2.0;
	float shapeYPos = pos.y - shape.getGlobalBounds().height / 2.0;
	shape.setPosition({ shapeXPos , shapeYPos });

	float textXPos = shapeXPos + shape.getGlobalBounds().width / 2.0 - (text.getGlobalBounds().width / 2.0);
	float textYPos = shapeYPos + shape.getGlobalBounds().height / 2.0 - (text.getGlobalBounds().height / 2.0);
	text.setPosition({ textXPos, textYPos });
}
void Button::ChangeButtonColor(sf::Color color)
{
	shape.setFillColor(color);
}

void Button::DrawButton(sf::RenderWindow& window)
{
	window.draw(shape);
	window.draw(text);
}
bool Button::isMouseOver(sf::RenderWindow& window)
{
	if (shape.getGlobalBounds().contains(GUIHelper::MousePosition(window)))
		return true;
	return false;
}





