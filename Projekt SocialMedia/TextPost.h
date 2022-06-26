#pragma once
#include "Post.h"
#include "User.h"
#include <string>

class TextPost :
    public Post
{
private:
    std::string textContent;
public:
    TextPost(int _ID, int _author, int _groupID, Timer _creationTime, std::vector<int> _likes,
        std::vector<Comment> _comments, std::string text);
    void DisplayPost(sf::RenderWindow& window, sf::Font& font) override;

    std::string getContent();
};

