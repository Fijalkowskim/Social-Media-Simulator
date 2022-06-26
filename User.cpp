#include "User.h"
#include "Post.h" // Aby unikn¹æ "Circular dependencies"
#include "Group.h"
#include "Event.h"
#include "PostsHeaders.h"

#define DEFAULT_NAME "Jan"
#define DEFAULT_SURNAME "Kowalski"
#define DEFAULT_AGE 30
#define DEFAULT_POR "Poland" //Place of residance
#define DEFAULT_DESCRIPTION "" //Place of residance
#define DEFAULT_PPP "Resources/Pictures/default.png" //Profile picture path

#pragma region Constructors / destructor
User::User() : User(-1, DEFAULT_NAME, DEFAULT_SURNAME, DEFAULT_AGE, 
	Gender::O, DEFAULT_POR, DEFAULT_DESCRIPTION, DEFAULT_PPP)
{
}
User::User(int _ID, std::string _name, std::string _surname, int _age, Gender _gender, std::string _placeOfResidence, 
std::string _description, std::string _profilePicturePath,
	std::vector<std::shared_ptr<Post>> _posts) :
	ID(_ID), name(_name), surname(_surname), age(_age), gender(_gender), placeOfResidence(_placeOfResidence),
	profileDescription(_description), profilePicturePath(_profilePicturePath), posts(_posts)
{
	if (_name == "") { name = DEFAULT_NAME; }
	if (_surname == "") { surname = DEFAULT_SURNAME; }
	if (_age <= 0) { age = DEFAULT_AGE; }
	if (_profilePicturePath == "") { profilePicturePath = DEFAULT_PPP; }
}
User::User(int _ID, std::string _name, std::string _surname, int _age, Gender _gender, std::string _placeOfResidence,
	std::string _description, std::string _profilePicturePath) :
	User(_ID, _name, _surname, _age, _gender, _placeOfResidence,
		_description, _profilePicturePath, std::vector<std::shared_ptr<Post>>())
{
}
User::~User()
{
}
#pragma endregion



bool User::AddFriend(int userID)
{	
	if (userID == ID || std::count(friends.begin(), friends.end(), userID)) return false;
	friends.push_back(userID);
	DataManager::getUser(userID).getFriends().push_back(ID);

	return true;
}
bool User::RemoveFriend(int userID)
{
	for (size_t i = 0; i < friends.size(); i++)
	{
		if (friends[i] == userID)
		{
			friends.erase(friends.begin() + i);

			std::vector<int>& usersFriends = DataManager::getUser(userID).getFriends();

			for (size_t j = 0; j < usersFriends.size(); j++)
			{
				if (usersFriends[j] == ID)
				{
					usersFriends.erase(usersFriends.begin() + j);
					return true;
				}
			}
		}
	}
	return false;
}

bool operator== (const User& uL, const User& uR)
{
	return uL.ID == uR.ID;
}
User& User::operator=(const User& u)
{
	if (&u == this) return *this;
	ID = u.ID; name = u.name; surname = u.surname; profileDescription = u.profileDescription; placeOfResidence = u.placeOfResidence;
	profilePicturePath = u.profilePicturePath; age = u.age; gender = u.gender; posts = u.posts; friends = u.friends; groups = u.groups;
}

void User::DisplayProfile(sf::RenderWindow& window, sf::Font& font)
{
	GUIHelper::DrawImage(window, profilePicturePath, { 10, 30 });
	GUIHelper::DrawText(window, font, name + " " + surname + ", " +
		std::to_string(age) + " lat", sf::Color::Black, 50, {130, 50});
	GUIHelper::DrawText(window, font, profileDescription, sf::Color::Black, 30, { 130, 120 });
	std::string genderString;
	switch (gender)
	{
	case User::M:
		genderString = "Mezczyzna";
		break;
	case User::F:
		genderString = "Kobieta";
		break;
	case User::O:
		genderString = "Nieokreslona";
		break;
	}
	GUIHelper::DrawText(window, font, genderString, sf::Color::Black, 40, { 130, 160 });

	GUIHelper::DrawText(window, font, placeOfResidence, sf::Color::Black, 40, { 130, 210 });

}

void User::ChangePersonalData(std::string _name, std::string _surname, int _age, Gender _gender, std::string _placeOfResidence,
	std::string _description, std::string _profilePicturePath)
{
	name = _name; surname = _surname; age = _age; gender = _gender; placeOfResidence = _placeOfResidence; profileDescription = _description; profilePicturePath = _profilePicturePath;
	FileManager::SaveUserToFile(*this);
}

#pragma region Posting
void User::AddTextPost(int groupID, std::string textContent)
{
	std::shared_ptr<Post> post(new TextPost(DataManager::getNewPostID(), ID, groupID, Timer(), std::vector<int>(), std::vector<Comment>(), textContent));
	posts.emplace_back(post);

	FileManager::SaveTextPost(dynamic_cast<TextPost&>(*post));
}
void User::AddImagePost(int groupID, std::string description, std::string imagePath)
{
	std::shared_ptr<Post> post(new ImagePost(DataManager::getNewPostID(), ID, groupID, Timer(), std::vector<int>(), std::vector<Comment>(), description, imagePath));
	posts.emplace_back(post);

	FileManager::SaveImagePost(dynamic_cast<ImagePost&>(*post));
}
void User::AddVideoPost(int groupID, std::string description, std::string videoPath)
{
	std::shared_ptr<Post> post(new VideoPost(DataManager::getNewPostID(), ID, groupID, Timer(), std::vector<int>(), std::vector<Comment>(), description, videoPath));
	posts.emplace_back(post);

	FileManager::SaveVideoPost(dynamic_cast<VideoPost&>(*post));
}
void User::AddAudioPost(int groupID, std::string audioPath)
{
	std::shared_ptr<Post> post(new AudioPost(DataManager::getNewPostID(), ID, groupID, Timer(), std::vector<int>(), std::vector<Comment>(), audioPath));
	posts.emplace_back(post);

	FileManager::SaveAudioPost(dynamic_cast<AudioPost&>(*post));
}

void User::RemovePost(int postID)
{
	for (size_t i = 0; i < posts.size(); i++)
	{
		if (posts[i]->getID() == postID)
		{
			int groupID = posts[i]->getGroupID();

			posts.erase(posts.begin() + i);

			if (groupID >= 0)
			{
				DataManager::getGroup(groupID).RemovePost(postID);
			}

			FileManager::RemovePost(postID);
			return;
		}
	}
}
#pragma endregion

#pragma region Getters
std::string User::getNameAndSurname()
{
	return name + " " + surname;
}
const std::string& User::getName()
{
	return name;
}
const std::string& User::getSurname()
{
	return surname;
}
const std::string& User::getDescription()
{
	return profileDescription;
}
const std::string& User::getPlaceOfResidence()
{
	return placeOfResidence;
}
const std::string& User::getProfilePicPath()
{
	return profilePicturePath;
}
int User::getAge()
{
	return age;
}
User::Gender User::getGender()
{
	return gender;
}

int User::getUserID()
{
	return ID;
}
std::vector<int>& User::getFriends()
{
	return friends;
}
std::vector<int>& User::getGroups()
{
	return groups;
}
std::vector<std::shared_ptr<Post>>& User::getPosts()
{
	return posts;
}
#pragma endregion

std::string User::GetUserFileString(User& user)
{
	std::string str = "#U\n" + std::to_string(user.getUserID()) + "\n" + user.getName() + 
		"\n" + user.getSurname() + "\n" + user.getPlaceOfResidence()
		+ "\n" + user.getDescription() + "\n" + std::to_string(user.getAge()) + "\n";
	if (user.getGender() == User::M) str += "M\n";
	else if (user.getGender() == User::F) str += "F\n";
	else str += "O\n";

	str += user.getProfilePicPath() + "\n#F";

	for (size_t i = 0; i < user.getFriends().size(); i++)
	{
		str += "\n" + std::to_string(user.getFriends()[i]);
	}
	return str;
}