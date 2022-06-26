#pragma once
#include <filesystem>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>
#include <unordered_map>
#include "User.h"
#include "Event.h"
#include "Post.h"
#include "FileManager.h"

class TextPost; class ImagePost; class VideoPost; class AudioPost;

class DataManager
{
private:
	static User* mainUser;
	static std::unordered_map<int, User> users;
	static std::unordered_map<int, std::unique_ptr<Group>> groups;
	static std::vector<int> usersIDs;

	static int postsToLoad;
	static std::vector<std::shared_ptr<Post>> loadedPosts;

	static int highestUserID;
	static int highestGroupID;
	static int highestPostID;
	static int highestCommentID;

	static User defaultUser;
	static Group defaultGroup;
	static Event defaultEvent;

#pragma region Data loading
	static void CreateUser(std::ifstream& file, std::unordered_map<int, std::vector<int>>& tmp_friends, 
		std::unordered_map<int, std::vector<std::shared_ptr<Post>>>& tmp_posts);
	static void CreateGroup(std::ifstream& file, std::unordered_map<int, 
		std::vector<std::shared_ptr<Post>>>& tmp_groupPosts);
	static void CreateEvent(std::ifstream& file, std::unordered_map<int,
		std::vector<std::shared_ptr<Post>>>& tmp_groupPosts);


	static void LoadMainUser();

	static void LoadComments(std::unordered_map<int, std::vector<Comment>>& postComments);
	static void LoadPosts(std::unordered_map<int, std::vector<std::shared_ptr<Post>>>& tmp_usersPosts,
		std::unordered_map<int, std::vector<std::shared_ptr<Post>>>& tmp_groupPosts,
		std::unordered_map<int, std::vector<Comment>>& commentsMap);
#pragma endregion
	
public:
	static void LoadPostsToView();

	static void LoadData();

	static User* MainUser();
	static User& getUser(int ID);
	static Group& getGroup(int ID);
	static std::vector<std::shared_ptr<Post>>& getLoadedPosts();
	static std::vector<int>& getUsersIDs();

	static int getNewUserID();
	static int getNewGroupID();
	static int getNewPostID();
	static int getNewCommentID();

	static void RemoveUser(int ID);
};

