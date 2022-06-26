#include "GUIHelper.h"

void GUIHelper::DrawImage(sf::RenderWindow& window, const std::string pngPath, sf::Vector2f position)
{
    sf::Texture tex;
    tex.loadFromFile(pngPath);
    sf::Sprite sprite(tex);
    sprite.setPosition(position);
    window.draw(sprite);
}
void GUIHelper::DrawImage(sf::RenderWindow& window, const std::string pngPath, sf::Vector2f position, sf::Vector2f scale)
{
    sf::Texture tex;
    tex.loadFromFile(pngPath);
    sf::Sprite sprite(tex);
    sprite.setPosition(position);
    sprite.setScale(scale);
    window.draw(sprite);
}

void GUIHelper::DrawImage(sf::RenderWindow& window, const std::string pngPath, sf::Vector2f position, sf::Vector2f scale, sf::Sprite& _sprite)
{
    sf::Texture tex;
    tex.loadFromFile(pngPath);
    sf::Sprite sprite(tex);
    sprite.setPosition(position);
    sprite.setScale(scale);
    _sprite = sprite;
    window.draw(_sprite);
}

void GUIHelper::DrawText(sf::RenderWindow& window, sf::Font& font, const std::string content, sf::Color color, unsigned int size, sf::Vector2f position)
{
    sf::Text text;
    text.setFont(font);
    text.setString(content);
    text.setFillColor(color);
    text.setCharacterSize(size);
    text.setPosition(position);

    window.draw(text);
}
void GUIHelper::DrawText(sf::RenderWindow& window, sf::Font& font, const std::string content, sf::Color color, unsigned int size, sf::Vector2f position, sf::Text::Style style)
{
    sf::Text text;
    text.setFont(font);
    text.setStyle(style);
    text.setString(content);
    text.setFillColor(color);
    text.setCharacterSize(size);
    text.setPosition(position);

    window.draw(text);
}

void GUIHelper::PlayAudio(const std::string audioPath)
{
    sf::SoundBuffer buffer;
    if (!buffer.loadFromFile(audioPath)) return;

    sf::Sound sound;
    sound.setBuffer(buffer);
    sound.play();
}

sf::Vector2f GUIHelper::MousePosition(sf::RenderWindow& window)
{
    return sf::Vector2f(sf::Mouse::getPosition(window));
}

bool GUIHelper::isMouseOverSprite(sf::RenderWindow& window, sf::Sprite& sprite)
{
    if (sprite.getGlobalBounds().contains(MousePosition(window)))
        return true;
    return false;
}
