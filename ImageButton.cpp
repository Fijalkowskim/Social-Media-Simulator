#include "ImageButton.h"



ImageButton::ImageButton(std::function<void()> _onClick, sf::Vector2f _position, sf::Vector2f _scale,
	sf::Vector2f _hoverScale, const std::string _imagePath): imagePath(_imagePath), position(_position), scale(_scale), hoverScale(_hoverScale)
{
    onClick = _onClick;
    state = Inactive;
}
ImageButton::ImageButton()
{
}

ImageButton::~ImageButton()
{
}
void ImageButton::VisualUpdate(sf::RenderWindow& window)
{
	switch (state)
	{
	case Button::Idle:
		GUIHelper::DrawImage(window, imagePath, position, 
			scale, sprite);
		break;
	case Button::HoverOver:
		GUIHelper::DrawImage(window, imagePath, position, 
			hoverScale, sprite);
		break;
	case Button::Inactive:
		return;
	}


}
void ImageButton::MouseOverCheck(sf::RenderWindow& window)
{
	if (state == Idle && GUIHelper::isMouseOverSprite(window, sprite))
	{
		state = HoverOver;
	}
	if (state == HoverOver && !GUIHelper::isMouseOverSprite(window, sprite))
	{
		state = Idle;
	}

}
void ImageButton::ChangeImagePath(std::string newPath)
{
	imagePath = newPath;
}

