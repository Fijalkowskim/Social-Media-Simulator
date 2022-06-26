#pragma once
#include "Button.h"
class ImageButton :
    public Button
{
private:
    std::string imagePath;
    sf::Vector2f position;
    sf::Vector2f scale;
    sf::Vector2f hoverScale;
    sf::Sprite sprite;
public:
    ImageButton(std::function<void()> _onClick, sf::Vector2f position, sf::Vector2f scale,
        sf::Vector2f hoverScale, const std::string _imagePath);
    ImageButton();
    ~ImageButton();

    void VisualUpdate(sf::RenderWindow& window) override;
    void MouseOverCheck(sf::RenderWindow& window) override;

    void ChangeImagePath(std::string newPath);
};

