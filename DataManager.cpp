#include "DataManager.h"
#include"TextPost.h"
#include"ImagePost.h"
#include"AudioPost.h"
#include"VideoPost.h"
#include"GUIManager.h"

User* DataManager::mainUser;

int DataManager::highestUserID = 0;
int DataManager::highestGroupID = 0;
int DataManager::highestPostID = 0;
int DataManager::highestCommentID = 0;

int DataManager::postsToLoad = 20;

std::unordered_map<int, User> DataManager::users;
std::unordered_map<int, std::unique_ptr<Group>> DataManager::groups;
std::vector<std::shared_ptr<Post>> DataManager::loadedPosts;
std::vector<int> DataManager::usersIDs;

User DataManager::defaultUser = User();
Group DataManager::defaultGroup = Group();
Event DataManager::defaultEvent = Event();


#pragma region DataLoading
void DataManager::LoadData()
{
	std::unordered_map<int, std::vector<int>> tmp_friends;
	std::unordered_map<int, std::vector<std::shared_ptr<Post>>> tmp_usersPosts, tmp_groupPosts;
	std::unordered_map<int, std::vector<Comment>> tmp_postsComments;

	LoadComments(tmp_postsComments);
	LoadPosts(tmp_usersPosts, tmp_groupPosts, tmp_postsComments);

	//--------------------------------------------USERS----------------------------------------------------------
	//Looks for all files in directory
	for (const std::filesystem::directory_entry& entry : std::filesystem::directory_iterator(FileManager::usersDataPath))
	{
		std::ifstream file;
		file.open(entry.path().string(), std::ios::in);

		if (!file.is_open()) continue;
		std::string line;

		std::getline(file, line);
		if (line == "#U")
		{
			CreateUser(file, tmp_friends, tmp_usersPosts);
		}
		file.close();
	}
	//--------------------------------------------GROUPS & EVENTS----------------------------------------------------------
	for (const std::filesystem::directory_entry& entry : std::filesystem::directory_iterator(FileManager::groupsDataPath))
	{
		std::ifstream file;
		file.open(entry.path().string(), std::ios::in);

		if (!file.is_open()) continue;
		std::string line;

		std::getline(file, line);
		if (line == "#G")
		{
			CreateGroup(file, tmp_groupPosts);

		}
		else if (line == "#E")
		{
			CreateEvent(file, tmp_groupPosts);
		}

		file.close();
	}

	//Ensure that every user has his firends added 
	for (std::pair<const int, User>& user : users)
	{
		std::vector<int>& friendsOfUser = tmp_friends[user.first];
		for (size_t j = 0; j < friendsOfUser.size(); j++)
		{
			user.second.AddFriend(friendsOfUser[j]);
		}
	}

	LoadMainUser();
	LoadPostsToView();
}

void DataManager::CreateUser(std::ifstream& file, 
	std::unordered_map<int, std::vector<int>>& tmp_friends,
	std::unordered_map<int, std::vector<std::shared_ptr<Post>>>& posts)
{
	std::string line;
	std::string name, surname, placeOfResidence, 
		profileDescription, profilePicturePath;
	int ID, age;
	User::Gender gender;

#pragma region Personal Data Setup
	(std::getline(file, line));
	ID = std::stoi(line);
	(std::getline(file, line));
	name = line;
	(std::getline(file, line));
	surname = line;
	(std::getline(file, line));
	placeOfResidence = line;
	(std::getline(file, line));
	profileDescription = line;
	(std::getline(file, line));
	age = std::stoi(line);

	(std::getline(file, line));
	if (line == "M") gender = User::Gender::M;
	else if(line == "F") gender = User::Gender::F;
	else gender = User::Gender::O;

	(std::getline(file, line));
	profilePicturePath = line;
#pragma endregion

	if (ID > highestUserID) highestUserID = ID;

	std::getline(file, line);
	//Friends
	tmp_friends[ID] = std::vector<int>();
	while (std::getline(file, line))
	{
		tmp_friends[ID].push_back(std::stoi(line));
	}

	users[ID] = User(ID, name, surname, age, gender, placeOfResidence, 
		profileDescription, profilePicturePath, posts[ID]);
	usersIDs.push_back(ID);
}
void DataManager::CreateGroup(std::ifstream& file, std::unordered_map<int, 
	std::vector<std::shared_ptr<Post>>>& tmp_groupPosts)
{
	std::string line;
	int ID;
	std::string name, description;
	std::vector<int> users = std::vector<int>();

#pragma region Data Setup
	(std::getline(file, line));
	ID = std::stoi(line);
	(std::getline(file, line));
	name = line;
	(std::getline(file, line));
	description = line;
#pragma endregion

	//Users
	std::getline(file, line); //#U
	while (std::getline(file, line))
	{
		int currentID = std::stoi(line);
		users.push_back(currentID);
	}
	if (ID > highestGroupID) highestGroupID = ID;

	groups[ID] = std::make_unique<Group>(Group(ID, name, description, users));

	//AddPosts
	for (size_t i = 0; i < tmp_groupPosts[ID].size(); i++)
	{
		groups[ID]->AddPost(tmp_groupPosts[ID][i]);
	}
}
void DataManager::CreateEvent(std::ifstream& file, 
	std::unordered_map<int, std::vector<std::shared_ptr<Post>>>& tmp_groupPosts)
{
	std::string line;
	int ID;
	std::string name, description;
	Timer eventDate;

	std::vector<int> users = std::vector<int>();

#pragma region Data Setup
	(std::getline(file, line));
	ID = std::stoi(line);
	(std::getline(file, line));
	name = line;
	(std::getline(file, line));
	description = line;

	eventDate = FileManager::LoadTimerFromFile(file, line);
#pragma endregion
	//Users
	std::getline(file, line); //#U
	while (std::getline(file, line))
	{
		int currentID = std::stoi(line);
		users.push_back(currentID);
	}

	if (ID > highestGroupID) highestGroupID = ID;

	groups[ID] = std::make_unique<Event>(Event(ID, name, description, eventDate, users));

	//AddPosts
	for (size_t i = 0; i < tmp_groupPosts[ID].size(); i++)
	{
		groups[ID]->AddPost(tmp_groupPosts[ID][i]);
	}
}

void DataManager::LoadMainUser()
{
	system("cls");
	std::ifstream inFile;
	inFile.open(FileManager::mainUserDataPath, std::ios::in);

	if (inFile.is_open())
	{
		std::string line;
		std::getline(inFile, line);
		mainUser = &getUser(std::stoi(line));
		inFile.close();
	}
	else //Create main user
	{
		inFile.close();
		std::string name, surname, por, description, ppp;
		User::Gender gender;
		int age = 30, ID;

		std::cout << "Wprowadz dane swojego profilu\n\n";
		GUIManager::ReadUserData(name, surname, por, description, ppp, gender, age);

		ID = getNewUserID();
		users[ID] = User(ID, name, surname, age, gender, por, description, ppp);
		mainUser = &users[ID];

		FileManager::SaveUserToFile(users[ID]);

		std::ofstream outFile;
		outFile.open(FileManager::mainUserDataPath);
		if (!outFile.is_open()) return;
		outFile << std::to_string(ID);
		outFile.close();
	}
}
void DataManager::LoadPostsToView()
{
	loadedPosts = std::vector<std::shared_ptr<Post>>();

	for (size_t i = 0; i < mainUser->getFriends().size(); i++)
	{
		for (std::shared_ptr<Post> post : getUser(mainUser->getFriends()[i]).getPosts())
		{
			if (loadedPosts.size() >= postsToLoad) return;
			loadedPosts.push_back(post);
		}
	}
	for (size_t i = 0; i < mainUser->getGroups().size(); i++)
	{
		for (std::shared_ptr<Post> post : getGroup(mainUser->getGroups()[i]).getPosts())
		{
			if (loadedPosts.size() >= postsToLoad) return;
			if (post->getAuthorID() == mainUser->getUserID() || 
				std::count(mainUser->getFriends().begin(), mainUser->getFriends().end(), post->getAuthorID())) continue;

			loadedPosts.push_back(post);
		}
	}
}


#pragma region Post Creation
void DataManager::LoadComments(std::unordered_map<int, std::vector<Comment>>& postComments)
{
	for (const std::filesystem::directory_entry& entry : std::filesystem::directory_iterator(FileManager::commentsDataPath))
	{
		std::ifstream file;
		file.open(entry.path().string(), std::ios::in);

		if (!file.is_open()) continue;
		std::string line;

		std::getline(file, line);
		if (line == "#C")
		{
			std::getline(file, line);
			int commentID = std::stoi(line);
			std::getline(file, line);
			int postID = std::stoi(line);
			std::getline(file, line);
			int authorID = std::stoi(line);
			std::getline(file, line);
			std::string content = line;

			postComments[postID].emplace_back(Comment(commentID, authorID, content));

			if (commentID > highestCommentID) highestCommentID = commentID;
		}
		file.close();
	}
}
void DataManager::LoadPosts(std::unordered_map<int, std::vector<std::shared_ptr<Post>>>& tmp_usersPosts,
	std::unordered_map<int, std::vector<std::shared_ptr<Post>>>& tmp_groupPosts,
	std::unordered_map<int, std::vector<Comment>>& commentsMap)
{
	int postID, authorID, groupID;
	std::vector<int> likes;
	Timer creationTime;

	for (const std::filesystem::directory_entry& entry : std::filesystem::directory_iterator(FileManager::postsDataPath))
	{
		std::ifstream file;
		file.open(entry.path().string(), std::ios::in);

		if (!file.is_open()) continue;
		std::string line;

		std::getline(file, line);
		if (line == "#P")
		{
			likes.clear();

			std::getline(file, line);
			postID = std::stoi(line);
			std::getline(file, line);
			authorID = std::stoi(line);
			std::getline(file, line);
			groupID = std::stoi(line);

			std::getline(file, line);

			if (line == "#TP")
			{
				std::getline(file, line);
				std::string textContent = line;

				FileManager::GetDefaultPostValues(file, line, creationTime, likes);

				std::shared_ptr<Post> postToAdd(new TextPost(postID, authorID, groupID,
					creationTime, likes, commentsMap[postID], textContent));

				tmp_usersPosts[authorID].emplace_back(postToAdd);

				if (groupID >= 0)
					tmp_groupPosts[groupID].emplace_back(postToAdd);

				if (postID > highestPostID) highestPostID = postID;
			}
			else if (line == "#IP")
			{
				std::getline(file, line);
				std::string description = line;
				std::getline(file, line);
				std::string imagePath = line;

				FileManager::GetDefaultPostValues(file, line, creationTime, likes);

				std::shared_ptr<Post> postToAdd(new ImagePost(postID, authorID, groupID, 
					creationTime, likes, commentsMap[postID],
					description, imagePath));

				tmp_usersPosts[authorID].emplace_back(postToAdd);

				if (groupID >= 0)
					tmp_groupPosts[groupID].emplace_back(postToAdd);

				if (postID > highestPostID) highestPostID = postID;
			}
			else if (line == "#AP")
			{
				std::getline(file, line);
				std::string audioPath = line;

				FileManager::GetDefaultPostValues(file, line, creationTime, likes);

				std::shared_ptr<Post> postToAdd(new AudioPost(postID, authorID, groupID, 
					creationTime, likes, commentsMap[postID], audioPath));

				tmp_usersPosts[authorID].emplace_back(postToAdd);

				if (groupID >= 0)
					tmp_groupPosts[groupID].emplace_back(postToAdd);

				if (postID > highestPostID) highestPostID = postID;
			}
			else if (line == "#VP")
			{
				std::string description, videoPath;
				int views;

				std::getline(file, line);
				description = line;
				std::getline(file, line);
				videoPath = line;

				FileManager::GetDefaultPostValues(file, line, creationTime, likes);

				std::shared_ptr<Post> postToAdd(new VideoPost(postID, authorID, groupID, 
					creationTime, likes, commentsMap[postID],
					description, videoPath));

				tmp_usersPosts[authorID].emplace_back(postToAdd);

				if (groupID >= 0)
					tmp_groupPosts[groupID].emplace_back(postToAdd);

				if (postID > highestPostID) highestPostID = postID;
			}
		}

		file.close();
	}
}

#pragma endregion

#pragma endregion

void DataManager::RemoveUser(int userID)
{
	User* userToRemove = nullptr;

	for (size_t i = 0; i < users.size(); i++)
	{
		if (users[i].getUserID() == userID)
		{
			userToRemove = &users[i];
			break;
		}
	}
	if (userToRemove == nullptr) return;

	std::vector<int>& firends = userToRemove->getFriends();
	std::vector<int>& groups = userToRemove->getGroups();
	std::vector<std::shared_ptr<Post>>& posts = userToRemove->getPosts();

	for (int friendID : firends)
	{
		userToRemove->RemoveFriend(friendID);
		FileManager::RemoveFriend(userID, friendID);
	}
	for (int groupID : groups)
	{
		getGroup(groupID).RemoveUser(userID);
		FileManager::RemoveGroupUser(groupID, userID);
	}
	for (std::shared_ptr<Post> post : posts)
	{
		userToRemove->RemovePost(post->getID());
	}

	//Remove posts files
	for (const std::filesystem::directory_entry& entry : std::filesystem::directory_iterator(FileManager::postsDataPath))
	{
		std::ifstream file;
		file.open(entry.path().string(), std::ios::in);

		if (!file.is_open()) continue;
		std::string line;

		std::getline(file, line);
		if (line != "#P") return;

		std::getline(file, line);
		std::getline(file, line);
		file.close();

		if (std::stoi(line) == userID)
		{
			std::filesystem::remove(entry.path().string());
		}
	}
	//Remove comments files
	for (const std::filesystem::directory_entry& entry : std::filesystem::directory_iterator(FileManager::commentsDataPath))
	{
		std::ifstream file;
		file.open(entry.path().string(), std::ios::in);

		if (!file.is_open()) continue;
		std::string line;

		std::getline(file, line);
		if (line != "#C") return;

		std::getline(file, line);
		std::getline(file, line);
		std::getline(file, line);
		file.close();

		if (std::stoi(line) == userID)
		{
			std::filesystem::remove(entry.path().string());
		}
	}
	//Remove user file
	for (const std::filesystem::directory_entry& entry : std::filesystem::directory_iterator(FileManager::usersDataPath))
	{
		std::ifstream file;
		file.open(entry.path().string(), std::ios::in);

		if (!file.is_open()) continue;
		std::string line;

		std::getline(file, line);
		if (line != "#U") return;

		std::getline(file, line);
		file.close();

		if (std::stoi(line) == userID)
		{
			std::filesystem::remove(entry.path().string());
		}
	}

	if (userID == mainUser->getUserID())
	{
		std::filesystem::remove(FileManager::mainUserDataPath);
		mainUser = &defaultUser;
	}
	//Remove user from users
	users.erase(userID);
}

#pragma region Getters
int DataManager::getNewUserID() { highestUserID++; return highestUserID; }
int DataManager::getNewGroupID() { highestGroupID++; return highestGroupID; }
int DataManager::getNewPostID() { highestPostID++;  return highestPostID; }
int DataManager::getNewCommentID(){ highestCommentID++; return highestCommentID; }




User& DataManager::getUser(int ID)
{
	if (users.count(ID) > 0) return users[ID];
	return defaultUser;
}
Group& DataManager::getGroup(int ID)
{
	if (groups.count(ID) > 0) return *groups[ID];
	return defaultGroup;
}
std::vector<std::shared_ptr<Post>>& DataManager::getLoadedPosts() { return loadedPosts; }
std::vector<int>& DataManager::getUsersIDs() { return usersIDs; }
User* DataManager::MainUser()
{
	if (mainUser == nullptr) mainUser = &defaultUser;
	return mainUser;
}
#pragma endregion