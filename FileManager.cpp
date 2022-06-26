#include "FileManager.h"
#include "PostsHeaders.h"

std::string const FileManager::mainUserDataPath = "Resources/Data/MainUserData.txt";
std::string const FileManager::usersDataPath = "Resources/Data/Users";
std::string const FileManager::groupsDataPath = "Resources/Data/Groups";
std::string const FileManager::postsDataPath = "Resources/Data/Posts";
std::string const FileManager::commentsDataPath = "Resources/Data/Comments";

void FileManager::SavePost(Post* post, std::string postFileString)
{
	std::ofstream file(FileManager::postsDataPath + "/Post" + std::to_string(post->getID()) +
		" U" + std::to_string(post->getAuthorID()) + ".txt");

	std::string fileStream = "#P\n" + std::to_string(post->getID()) + "\n" +
		std::to_string(post->getAuthorID()) + "\n" + std::to_string(post->getGroupID()) + "\n" +
		postFileString + post->getCreationTimeFileFormat() + "#L";

	for (size_t i = 0; i < post->getLikes().size(); i++)
	{
		fileStream += "\n" + std::to_string(post->getLikes()[i]);
	}
	file << fileStream;

	file.close();

}
void FileManager::SaveTextPost(TextPost& post)
{
	SavePost(&post, "#TP\n" + post.getContent() + "\n");
}
void FileManager::SaveImagePost(ImagePost& post)
{
	SavePost(&post, "#IP\n" + post.getDescription() + "\n" + post.getImagePath() + "\n");
}
void FileManager::SaveVideoPost(VideoPost& post)
{
	SavePost(&post, "#VP\n" + post.getDescription() + "\n" + post.getVideoPath() + "\n");
}
void FileManager::SaveAudioPost(AudioPost& post)
{
	SavePost(&post, "#AP\n" + post.getAudioPath() + "\n");

}
void FileManager::SaveAddedFriend(int user1ID, int user2ID)
{
	bool oneAlreadyAdded = false;

	for (const std::filesystem::directory_entry& entry : std::filesystem::directory_iterator(FileManager::usersDataPath))
	{
		std::ifstream inFile;
		inFile.open(entry.path().string(), std::ios::in);

		if (!inFile.is_open()) continue;
		std::string line;

		std::getline(inFile, line);
		if (line == "#U")
		{
			std::getline(inFile, line);

			if (std::stoi(line) == user1ID)
			{
				inFile.close();
				std::ofstream outFile;
				outFile.open(entry.path().string(), std::ofstream::app);
				if (!outFile.is_open()) continue;
				outFile << "\n" + std::to_string(user2ID);
				outFile.close();

				if (oneAlreadyAdded)
					return;
				else
					oneAlreadyAdded = true;
			}
			else if (std::stoi(line) == user2ID)
			{
				inFile.close();
				std::ofstream outFile;
				outFile.open(entry.path().string(), std::ofstream::app);
				if (!outFile.is_open()) continue;
				outFile << "\n" + std::to_string(user1ID);
				outFile.close();

				if (oneAlreadyAdded)
					return;
				else
					oneAlreadyAdded = true;
			}
			else
				inFile.close();
		}
	}
}
void FileManager::SaveAddedGroupUser(int groupID, int userID)
{
	for (const std::filesystem::directory_entry& entry : std::filesystem::directory_iterator(FileManager::groupsDataPath))
	{
		std::ifstream inFile;
		inFile.open(entry.path().string(), std::ios::in);

		if (!inFile.is_open()) continue;
		std::string line;

		std::getline(inFile, line);
		if (line == "#G" || line == "#E")
		{
			std::getline(inFile, line);

			if (std::stoi(line) == groupID)
			{
				inFile.close();
				std::ofstream outFile;
				outFile.open(entry.path().string(), std::ofstream::app);
				if (!outFile.is_open()) return;
				outFile << "\n" + std::to_string(userID);
				outFile.close();
				return;
			}
		}
	}
}
void FileManager::SaveComment(Comment& comment, int postID)
{
	std::ofstream file;
	file.open(FileManager::commentsDataPath + "/Comment" + std::to_string(comment.getID()) +
		" P" + std::to_string(postID) + ".txt");

	if (!file.is_open()) return;

	file << "#C\n" + std::to_string(comment.getID()) + "\n" + std::to_string(postID) + "\n" +
		std::to_string(comment.getAuthorID()) + "\n" + comment.getContent();

	file.close();
}
void FileManager::SaveLike(int postID, int userID)
{
	for (const std::filesystem::directory_entry& entry : std::filesystem::directory_iterator(FileManager::postsDataPath))
	{
		std::ifstream inFile;
		inFile.open(entry.path().string(), std::ios::in);

		if (!inFile.is_open()) continue;
		std::string line;

		std::getline(inFile, line);
		if (line == "#P")
		{
			std::getline(inFile, line);

			if (std::stoi(line) == postID)
			{
				inFile.close();
				std::ofstream outFile;
				outFile.open(entry.path().string(), std::ofstream::app);
				if (!outFile.is_open()) return;
				outFile << "\n" + std::to_string(userID);
				outFile.close();
				return;
			}
		}
		inFile.close();
	}
}
void FileManager::SaveUserToFile(User& user)
{
	for (const std::filesystem::directory_entry& entry : std::filesystem::directory_iterator(FileManager::usersDataPath))
	{
		std::ifstream inFile;
		inFile.open(entry.path().string(), std::ios::in);

		if (!inFile.is_open()) continue;
		std::string line;

		std::getline(inFile, line);
		if (line == "#U")
		{
			std::getline(inFile, line);
			if (std::stoi(line) == user.getUserID())
			{
				inFile.close();

				std::ofstream outFile;
				outFile.open(entry.path().string());
				if (!outFile.is_open()) continue;
				outFile << User::GetUserFileString(user);
				outFile.close();
				return;
			}
		}
		inFile.close();
	}

	std::ofstream outFile;
	outFile.open(FileManager::usersDataPath + "/User" + std::to_string(user.getUserID()) + ".txt");
	if (!outFile.is_open()) return;
	outFile << User::GetUserFileString(user);
	outFile.close();
}

void FileManager::RemovePost(int postID)
{
	for (const std::filesystem::directory_entry& entry : std::filesystem::directory_iterator(postsDataPath))
	{
		std::ifstream file;
		file.open(entry.path().string(), std::ios::in);

		if (!file.is_open()) continue;
		std::string line;

		std::getline(file, line);
		if (line == "#P")
		{
			std::getline(file, line);

			if (std::stoi(line) == postID)
			{
				file.close();
				std::filesystem::remove(entry.path().string());
				return;
			}
		}
		file.close();
	}
}
void FileManager::RemoveComment(int commentID)
{
	for (const std::filesystem::directory_entry& entry : std::filesystem::directory_iterator(commentsDataPath))
	{
		std::ifstream file;
		file.open(entry.path().string(), std::ios::in);

		if (!file.is_open()) continue;
		std::string line;

		std::getline(file, line);
		if (line == "#C")
		{
			std::getline(file, line);

			if (std::stoi(line) == commentID)
			{
				file.close();
				std::filesystem::remove(entry.path().string());
				return;
			}
		}
		file.close();
	}
}
void FileManager::RemoveFriend(int user1ID, int user2ID)
{
	bool oneAlreadyRemoved = false;
	std::string newFileString;

	for (const std::filesystem::directory_entry& entry : std::filesystem::directory_iterator(FileManager::usersDataPath))
	{
		std::ifstream inFile;
		inFile.open(entry.path().string(), std::ios::in);

		if (!inFile.is_open()) continue;
		std::string line;

		std::getline(inFile, line);
		if (line == "#U")
		{
			newFileString = "#U";

			std::getline(inFile, line);
			newFileString += "\n" + line;

			if (std::stoi(line) == user1ID)
			{
				do
				{
					std::getline(inFile, line);
					newFileString += "\n" + line;
				} while (line != "#F");

				while (std::getline(inFile, line))
				{
					int ID = std::stoi(line);
					if (ID != user2ID)
						newFileString += "\n" + line;
				}
				inFile.close();

				std::ofstream outFile;
				outFile.open(entry.path().string());
				if (!outFile.is_open()) continue;
				outFile << newFileString;
				outFile.close();

				if (oneAlreadyRemoved)
					return;
				else
					oneAlreadyRemoved = true;
			}
			else if (std::stoi(line) == user2ID)
			{
				do
				{
					std::getline(inFile, line);
					newFileString += "\n" + line;
				} while (line != "#F");

				while (std::getline(inFile, line))
				{
					int ID = std::stoi(line);
					if (ID != user1ID)
						newFileString += "\n" + line;
				}
				inFile.close();

				std::ofstream outFile;
				outFile.open(entry.path().string());
				if (!outFile.is_open()) continue;
				outFile << newFileString;
				outFile.close();

				if (oneAlreadyRemoved)
					return;
				else
					oneAlreadyRemoved = true;
			}
			else
				inFile.close();
		}
	}
}
void FileManager::RemoveGroupUser(int groupID, int userID)
{
	std::string newFileString;
	for (const std::filesystem::directory_entry& entry : std::filesystem::directory_iterator(FileManager::groupsDataPath))
	{
		std::ifstream inFile;
		inFile.open(entry.path().string(), std::ios::in);

		if (!inFile.is_open()) continue;
		std::string line;

		std::getline(inFile, line);
		if (line == "#G" || line == "#E")
		{
			newFileString = line;

			std::getline(inFile, line);
			newFileString += "\n" + line;

			if (std::stoi(line) == groupID)
			{
				do
				{
					std::getline(inFile, line);
					newFileString += "\n" + line;
				} while (line != "#U");

				while (std::getline(inFile, line))
				{
					int ID = std::stoi(line);
					if (ID != userID)
						newFileString += "\n" + line;
				}
				inFile.close();

				std::ofstream outFile;
				outFile.open(entry.path().string());
				if (!outFile.is_open()) continue;
				outFile << newFileString;
				outFile.close();
				return;
			}
		}
		inFile.close();
	}
}
void FileManager::RemoveLike(int postID, int userID)
{
	std::string newFileString;
	for (const std::filesystem::directory_entry& entry : std::filesystem::directory_iterator(FileManager::postsDataPath))
	{
		std::ifstream inFile;
		inFile.open(entry.path().string(), std::ios::in);

		if (!inFile.is_open()) continue;
		std::string line;

		std::getline(inFile, line);
		if (line == "#P")
		{
			newFileString = line;

			std::getline(inFile, line);
			newFileString += "\n" + line;

			if (std::stoi(line) == postID)
			{
				do
				{
					std::getline(inFile, line);
					newFileString += "\n" + line;
				} while (line != "#L");

				while (std::getline(inFile, line))
				{
					int ID = std::stoi(line);
					if (ID != userID)
						newFileString += "\n" + line;
				}
				inFile.close();

				std::ofstream outFile;
				outFile.open(entry.path().string());
				if (!outFile.is_open()) continue;
				outFile << newFileString;
				outFile.close();
				return;
			}
		}
		inFile.close();
	}
}

Timer FileManager::LoadTimerFromFile(std::ifstream& file, std::string& line)
{
	int year, month, day, hour, min, sec;
	std::getline(file, line);
	year = std::stoi(line);
	std::getline(file, line);
	month = std::stoi(line);
	std::getline(file, line);
	day = std::stoi(line);
	std::getline(file, line);
	hour = std::stoi(line);
	std::getline(file, line);
	min = std::stoi(line);
	std::getline(file, line);
	sec = std::stoi(line);
	return Timer(year, month, day, hour, min, sec);
}
void FileManager::GetDefaultPostValues(std::ifstream& file, std::string& line, 
	Timer& creationTime, std::vector<int>& likes)
{
	creationTime = LoadTimerFromFile(file, line);

	std::getline(file, line);//#L
	while (std::getline(file, line))
	{
		likes.emplace_back(std::stoi(line));
	}
}