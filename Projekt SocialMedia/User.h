#pragma once
#include <string>
#include <chrono>
#include <vector>
#include <iostream>
#include "DataManager.h"
#include "GUIHelper.h"

class Post;
class Group;
class Event;

class User
{
public:
	enum Gender
	{
		M, //Male
		F, //Female
		O  //Other
	};
private:

#pragma region Personal user information
	int ID;

	std::string name, surname, placeOfResidence, profileDescription;
	int age;
	Gender gender;
	std::string profilePicturePath;
#pragma endregion


	std::vector<std::shared_ptr<Post>> posts;
	std::vector<int> friends;
	std::vector<int> groups;

public:
#pragma region Constructors/Destructor
	User();
	User(int _ID, std::string _name, std::string _surname, int _age, Gender _gender,
		std::string _placeOfResidence, std::string _description, std::string _profilePicturePath);

	User(int _ID, std::string _name, std::string _surname, int _age, Gender _gender,
		std::string _placeOfResidence, std::string _description, std::string _profilePicturePath,
		std::vector<std::shared_ptr<Post>> _posts);
	~User();
#pragma endregion

#pragma region Getters
	std::string getNameAndSurname();
	const std::string& getName();
	const std::string& getSurname();
	const std::string& getDescription();
	const std::string& getPlaceOfResidence();
	const std::string& getProfilePicPath();
	Gender getGender();
	int getAge();
	int getUserID();
	std::vector<int>& getFriends();
	std::vector<int>& getGroups();
	std::vector<std::shared_ptr<Post>>& getPosts();
#pragma endregion

	friend bool operator== (const User& uL, const User& uR);
	User& operator=(const User&);

	bool AddFriend(int userID);
	bool RemoveFriend(int userID);

	static std::string GetUserFileString(User& user);


	void DisplayProfile(sf::RenderWindow& window, sf::Font& font);


	void ChangePersonalData(std::string _name, std::string _surname, int _age, Gender _gender,
		std::string _placeOfResidence, std::string _description, std::string _profilePicturePath);

	void AddTextPost(int groupID, std::string textContent);
	void AddImagePost(int groupID, std::string description, std::string imagePath);
	void AddVideoPost(int groupID, std::string description, std::string videoPath);
	void AddAudioPost(int groupID, std::string audioPath);

	void RemovePost(int postID);
};

