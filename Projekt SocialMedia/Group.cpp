#include "User.h"
#include "Post.h"
#include "Group.h"
#include "Event.h"

Group::Group(): Group(DataManager::getNewGroupID(), "Empty group", ""){}
Group::Group(int _ID, std::string _name, std::string _description) : Group(_ID, _name, _description, std::vector<int>()) {}
Group::Group(int _ID, std::string _name, std::string _description, std::vector<int> _users):
	name(_name), description(_description), ID(_ID)
{
	for (size_t i = 0; i < _users.size(); i++)
	{
		AddUser(_users[i]);
	}
}
Group::~Group()
{
}
bool Group::AddUser(int userID)
{
	if (std::count(users.begin(), users.end(),userID)) return false;
	users.push_back(userID);
	DataManager::getUser(userID).getGroups().push_back(ID);
	return true;
}
bool Group::RemoveUser(int userID)
{
	for (size_t i = 0; i < users.size(); i++)
	{
		if (users[i] == userID)
		{
			users.erase(users.begin() + i);

			std::vector<int>& groupsOfUser = DataManager::getUser(userID).getGroups();

			for (size_t j = 0; j < groupsOfUser.size(); j++)
			{
				if (groupsOfUser[j] == ID)
				{
					groupsOfUser.erase(groupsOfUser.begin() + j);
					return true;
				}
			}

		}
	}
	return false;
}
void Group::AddPost(std::shared_ptr<Post>& post)
{
	for (size_t i = 0; i < users.size(); i++)
	{
		if (users[i] == post->getAuthorID())
		{
			posts.emplace_back(post);
			return;
		}
	}
}
void Group::RemovePost(int postID)
{
	for (size_t i = 0; i < posts.size(); i++)
	{
		if (posts[i]->getID() == postID)
		{
			posts.erase(posts.begin() + i);
			return;
		}
	}
}

#pragma region Getters
int Group::getID()
{
	return ID;
}
const std::string& Group::getName()
{
	return name;
}
std::vector<int>& Group::getUsers()
{
	return users;
}
std::vector<std::shared_ptr<Post>>& Group::getPosts()
{
	return posts;
}
#pragma endregion

bool operator==(const Group& gL, const Group& gR)
{
	return gL.ID == gR.ID;
}

void Group::DisplayGroup(sf::RenderWindow& window, sf::Font& font)
{
	GUIHelper::DrawText(window, font, name, sf::Color(56,149,195), 100, { 130, 50 });
	GUIHelper::DrawText(window, font, description, sf::Color::Black, 30, { 130, 200 });
	GUIHelper::DrawText(window, font, std::to_string(users.size()) + " czlonkow.", sf::Color::Black, 50, {130, 250});
}
