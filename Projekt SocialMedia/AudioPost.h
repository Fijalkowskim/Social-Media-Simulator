#pragma once
#include "Post.h"
#include "User.h"
#include <string>

class AudioPost :
    public Post
{
private:
    std::string audioPath;
public:
    class AudioPost(int _ID, int _author, int _groupID, Timer _creationTime, std::vector<int> _likes,
        std::vector<Comment> _comments, std::string _audioPath);
    void DisplayPost(sf::RenderWindow& window, sf::Font& font);

    std::string getAudioPath();
};

