#pragma once
#include <filesystem>
#include <fstream>
#include <iostream>
#include <string>
#include "Comment.h"
#include "User.h"
#include "Timer.h"
class TextPost; class ImagePost; class VideoPost; class AudioPost; class Post;

class FileManager
{	
private:
	static void SavePost(Post* post, std::string postFileString);
public:
	static void SaveTextPost(TextPost& post);
	static void SaveImagePost(ImagePost& post);
	static void SaveVideoPost(VideoPost& post);
	static void SaveAudioPost(AudioPost& post);

	static void SaveAddedFriend(int user1ID, int user2ID);
	static void SaveAddedGroupUser(int groupID, int userID);
	static void SaveLike(int postID, int userID);

	static void SaveComment(Comment& comment, int postID);
	static void SaveUserToFile(User& user);

	static void RemovePost(int postID);
	static void RemoveComment(int commentID);
	static void RemoveFriend(int user1ID, int user2ID);
	static void RemoveGroupUser(int groupID, int userID);
	static void RemoveLike(int postID, int userID);

	static Timer LoadTimerFromFile(std::ifstream& file, std::string& line);
	static void GetDefaultPostValues(std::ifstream& file, std::string& line, 
		Timer& creationTime, std::vector<int>& likes);


	static const std::string mainUserDataPath;
	static const std::string usersDataPath;
	static const std::string groupsDataPath;
	static const std::string postsDataPath;
	static const std::string commentsDataPath;
};

