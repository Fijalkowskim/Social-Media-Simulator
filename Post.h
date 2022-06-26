#pragma once
#include <vector>
#include <iostream>
#include "Comment.h"
#include "Timer.h"
#include "GUIHelper.h"
class User;

class Post
{
protected:
	int authorID, groupID, ID;
	std::vector<int> usersLiked;
	std::vector<Comment> comments;
	Timer creationTime;
public:
	void Like_Unlike(int userID);
	void AddComment(int userID, std::string content);
	void RemoveComment(int commentID);

	std::string getCreationTimeFileFormat();
	std::string getCreationTimeString();
	std::vector<Comment>& getComments();
	std::vector<int>& getLikes();
	int getID();
	int getAuthorID();
	int getGroupID();

	Post(int ID, int _authorID, int groupID, 
		Timer _creationTime, std::vector<int> _likes, std::vector<Comment> _comments);
	Post();
	virtual ~Post();

	virtual void DisplayPost(sf::RenderWindow& window, sf::Font& font);

	friend bool operator== (const Post& pL, const Post& pR);
};

