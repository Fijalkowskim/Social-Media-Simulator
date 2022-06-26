#pragma once
#include <vector>
#include <string>
#include <algorithm>
#include "DataManager.h"
#include "GUIHelper.h"

class User;
class Post;

class Group
{
protected:

	int ID;
	std::string name;
	std::string description;
	std::vector<int> users;
	std::vector<std::shared_ptr<Post>> posts;
public:
	Group();
	Group(int _ID, std::string _name, std::string _description);
	Group(int _ID, std::string _name, std::string _description,
		std::vector<int> _users);
	virtual ~Group();

	bool AddUser(int userID);
	bool RemoveUser(int userID);
	void AddPost(std::shared_ptr<Post>& post);
	void RemovePost(int postID);

	int getID();
	const std::string& getName();
	std::vector<int>& getUsers();
	std::vector<std::shared_ptr<Post>>& getPosts();

	friend bool operator== (const Group& gL, const Group& gR);

	virtual void DisplayGroup(sf::RenderWindow& window, sf::Font& font);
};

