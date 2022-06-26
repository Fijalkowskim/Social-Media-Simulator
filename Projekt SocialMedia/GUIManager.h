#pragma once
#include <iostream>
#include <string>
#include <vector>
#include "Button.h"
#include "ImageButton.h"
#include "AudioPost.h"
#include "VideoPost.h"
#include "TextPost.h"
#include "User.h"
#include "GUIHelper.h"
#include "Group.h"
#include "DataManager.h"

#pragma region SFML LIBRARY

#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>
#include <SFML/Window.hpp>
#include <SFML/Audio.hpp>

#pragma endregion


class GUIManager
{
	enum Page
	{
		MainMenu, PostDisplay, Comments, ProfilePage, UserDisplay, Groups, Posting
	};

private:
#pragma region Content display
	Page currentPage;

	int currentPostIndex = 0;
	int currentCommentIndex = 0;
	int currentUserIndex = 0;
	int currentGroupIndex = -1;

	User* currentUser;                                            //Currently viewed users (from targetedUsers)
	User* currentlyWatchedUser;                                   //The user whose friends/posts/groups are currently being viewed
	Post* currentPost;
	Group* currentGroup;
	Comment* currentComment;
	std::vector<int>* targetedUsers;
	std::vector<std::shared_ptr<Post>>* targetetPosts;            //Posts targetet to be viewed in this moment
#pragma endregion

#pragma region Page Control
	void NextPost(); void PreviousPost();
	void NextComment(); void PreviousComment();
	void NextFriend(); void PreviousFriend();
	void NextGroup(); void PreviousGroup();
	void SwitchPage(Page newPage);
#pragma endregion

#pragma region Window Setup
	sf::Event event;
	sf::RenderWindow* window;
	int width, height;
	sf::Font defaultFont;
	const std::string defaultFontPath = "Fonts/arial.ttf";

#pragma endregion

#pragma region Buttons
	void ConditionalButtonsCheck();
	void InstantiateButtons();
	void ButtonsVisualUpdate();

	std::unordered_map<Page, std::vector<std::unique_ptr<Button>>> buttons;

	std::unordered_map<Page, std::vector<std::pair<std::unique_ptr<Button>, 
		std::function<bool()>>>> conditionalButtons;

#pragma region Buttons functionality
	void ToPostDisplay(); void ToUserDisplay(); void ToMainMenuButton(); void ToProfilePageButton(); void ToUsersPosts();
	void ToFriendsButton(); void ToGroupMembersButton(); void ToGroupsButton();
	void ToCommentsButton(); void ToGroupPosts(); void ToAllUsers();

	void SelectPostToAddButton(); void CreateTextPost(); void CreateImagePost(); void CreateVideoPost(); void CreateAudioPost();
	void RemovePostButton(); void RemoveCommentButton();
	void LikeCurrentPost(); void CommentCurrentPost(); 

	void AddFriend(); void RemoveFriend(); void JoinGroup(); void LeaveGroup();
	void EditProfile(); void RemoveProfile();
#pragma endregion

#pragma endregion

#pragma region Main Tasks

	void Render();
	void DisplayCurrentPage();

	void PollEvents();
	void KeyInputLogic();
	void MousePressed();
	void MouseReleased();
	void MousePosCheck();
	void OpenWindow(int width, int height);
#pragma endregion
public:
	GUIManager(int _width, int _height);
	~GUIManager();

	void RenderLoop();
	bool IsWindowOpen();

	static void ReadUserData(std::string& name, std::string& surname, 
		std::string& placeOfResidence, std::string& description,
		std::string& profilePicturePath, User::Gender& gender, int& age);
};

