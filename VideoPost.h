#pragma once
#include "Post.h"
#include "User.h"
#include <string>
class VideoPost :
    public Post
{
private:
    std::string description;
    std::string videoPath;
public:
    VideoPost(int _ID, int _author, int _groupID, Timer _creationTime, std::vector<int> _likes,
        std::vector<Comment> _comments, std::string _desription, std::string _videoPath);
    void DisplayPost(sf::RenderWindow& window, sf::Font& font);

    std::string getDescription();
    std::string getVideoPath();
};

