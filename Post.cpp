#include "Post.h"
#include "User.h"

#include "DataManager.h"

void Post::Like_Unlike(int userID)
{
	for (size_t i = 0; i < usersLiked.size(); i++)
	{
		if (usersLiked[i] == userID)
		{
			usersLiked.erase(usersLiked.begin() + i); //Unlike
			FileManager::RemoveLike(ID, userID);
			return;
		}
	}
	usersLiked.push_back(userID);
	FileManager::SaveLike(ID, userID);

}

void Post::AddComment(int userID, std::string content)
{
	int commentID = DataManager::getNewCommentID();
	Comment comment(commentID, userID, content);
	comments.emplace_back(comment);
	FileManager::SaveComment(comment, ID);
}

void Post::RemoveComment(int commentID)
{
	for (size_t i = 0; i < comments.size(); i++)
	{
		if (comments[i].getID() == commentID)
		{
			comments.erase(comments.begin() + i);
			FileManager::RemoveComment(commentID);
			return;
		}
	}
}

std::string Post::getCreationTimeString()
{
	return creationTime.getTimeString();
}

std::string Post::getCreationTimeFileFormat()
{
	return creationTime.getTimeFileFormat();
}

std::vector<Comment>& Post::getComments()
{
	return comments;
}

std::vector<int>& Post::getLikes()
{
	return usersLiked;
}

int Post::getID()
{
	return ID;
}

int Post::getAuthorID()
{
	return authorID;
}

int Post::getGroupID()
{
	return groupID;
}


Post::Post(int _ID, int _authorID, int _groupID, Timer _creationTime, std::vector<int> _likes, std::vector<Comment> _comments) :
	ID(_ID), authorID(_authorID), groupID(_groupID), creationTime(_creationTime), usersLiked(_likes), comments(_comments) {}

Post::Post()
{
}

Post::~Post()
{
}

void Post::DisplayPost(sf::RenderWindow& window, sf::Font& font)
{
	GUIHelper::DrawImage(window, DataManager::getUser(authorID).getProfilePicPath(), { 10, 30 });
	GUIHelper::DrawText(window, font, DataManager::getUser(authorID).getNameAndSurname() + " - " +
		creationTime.getTimeString(),
		sf::Color::Black, 50, { 130, 50 });
	if (groupID >= 0)
	{
		GUIHelper::DrawText(window, font, "Dodal post do " + DataManager::getGroup(groupID).getName(),
			sf::Color::Cyan, 30, { 130, 120 });
	}
	GUIHelper::DrawText(window, font, "Polubienia: " + std::to_string(usersLiked.size()),
		sf::Color::Black, 40, { 150, 600 });
	GUIHelper::DrawText(window, font, "Komentarze: " + std::to_string(comments.size()),
		sf::Color::Black, 40, { 600, 600 });
}

bool operator== (const Post& pL, const Post& pR)
{
	return pL.ID == pR.ID && pL.authorID == pR.authorID;
}

