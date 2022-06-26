#pragma once
#include "Post.h"
#include "User.h"
#include <string>

class ImagePost :
    public Post
{
private:
    std::string description;
    std::string imagePath;
public:
    ImagePost(int _ID, int _author, int _groupID, Timer _creationTime,  std::vector<int> _likes, 
        std::vector<Comment> _comments, std::string _desription, std::string _imagePath);
    void DisplayPost(sf::RenderWindow& window, sf::Font& font);

    std::string getDescription();
    std::string getImagePath();
};

