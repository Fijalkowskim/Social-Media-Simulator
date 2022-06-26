#include "GUIManager.h"

#pragma region Main Rendering
void GUIManager::RenderLoop()
{
    PollEvents();
    Render();
}
void GUIManager::Render()
{
    window->clear(sf::Color::White);
    ButtonsVisualUpdate();
    DisplayCurrentPage();
    window->display();
}
void GUIManager::DisplayCurrentPage()
{
    switch (currentPage)
    {
    case MainMenu:
        GUIHelper::DrawText(*window, defaultFont, "Witaj " + DataManager::MainUser()->getName(),
            sf::Color::Blue, 50, sf::Vector2f( (width / 2.0) - 200, 10 ));
        break;

    case PostDisplay:
        if (targetetPosts == nullptr) break;
        GUIHelper::DrawText(*window, defaultFont, "Post " +
            std::to_string(currentPostIndex + 1) + "/" + std::to_string(targetetPosts->size()),
            sf::Color::Blue, 30, sf::Vector2f((width / 2.0) - 200, 00));

        if (currentPost == nullptr) break;
        currentPost->DisplayPost(*window, defaultFont);
        break;
    case Comments:
        if (currentComment == nullptr || currentPost == nullptr) break;
        currentComment->DisplayComment(*window, defaultFont);

        GUIHelper::DrawText(*window, defaultFont, std::to_string(currentCommentIndex + 1) + "/" + 
            std::to_string(currentPost->getComments().size()), sf::Color::Black, 80, sf::Vector2f(300, 10));
        break;

    case ProfilePage:
        GUIHelper::DrawText(*window, defaultFont, "Twoj profil", sf::Color::Blue, 50, sf::Vector2f((width / 2.0) - 200, 0));
        DataManager::MainUser()->DisplayProfile(*window, defaultFont);
        break;

    case UserDisplay:
        if (currentUser == nullptr) return;
        if (currentlyWatchedUser != nullptr) //Watching friends
        {
            GUIHelper::DrawText(*window, defaultFont, "Znajomi uzytkownika " + currentlyWatchedUser->getNameAndSurname() + ": " +
                std::to_string(currentUserIndex + 1) + "/" + std::to_string(targetedUsers->size()),
                sf::Color::Blue, 35, sf::Vector2f(500, 0));
        }
        else if (currentGroup != nullptr)
        {
            GUIHelper::DrawText(*window, defaultFont, "Czlonkowie grupy " + currentGroup->getName() + ": " +
                std::to_string(currentUserIndex + 1) + "/" + std::to_string(targetedUsers->size()),
                sf::Color::Blue, 35, sf::Vector2f(500, 0));
        }
        else
        {
            GUIHelper::DrawText(*window, defaultFont, "Uzytkownicy: "+ std::to_string(currentUserIndex + 1) + "/" + std::to_string(targetedUsers->size()),
                sf::Color::Blue, 40, sf::Vector2f(800, 0));
        }
      
       
        currentUser->DisplayProfile(*window, defaultFont);
        break;

    case Groups:
            if (currentGroup == nullptr) break;

            GUIHelper::DrawText(*window, defaultFont, "Grupy i wydarzenia uzytkownika " + currentUser->getNameAndSurname() + " " + 
                std::to_string(currentGroupIndex + 1) + "/" + std::to_string(currentUser->getGroups().size()),
                sf::Color::Blue, 25, sf::Vector2f(400, 0));
            currentGroup->DisplayGroup(*window, defaultFont);
            break;

    case Posting:
        GUIHelper::DrawText(*window, defaultFont, "Wybierz jaki post chcesz dodac a nastepnie wprowadz dane w konsoli"
            , sf::Color::Blue, 50, sf::Vector2f(100, 80));

        break;
    }
}
#pragma endregion

#pragma region Logic
void GUIManager::PollEvents()
{
    while (window->pollEvent(event))
    {    
        MousePosCheck();
        switch (event.type)
        {
        case sf::Event::Closed:
            window->close();
            break;
        case sf::Event::KeyPressed:
            KeyInputLogic();
            break;
        case sf::Event::MouseButtonPressed:
            MousePressed();
            break;
        case sf::Event::MouseButtonReleased:
            MouseReleased();
            break;
        }
    }
}
void GUIManager::KeyInputLogic()
{
    switch (event.key.code)
    {
    case sf::Keyboard::Escape:
        window->close();
        break;
    case sf::Keyboard::Right:
        switch (currentPage)
        {
        case GUIManager::PostDisplay:
            NextPost();
            break;
        case GUIManager::Comments:
            NextComment();
            break;
        case GUIManager::UserDisplay:
            NextFriend();
            break;
        case GUIManager::Groups:
            NextGroup();
            break;
        }
        break;

    case sf::Keyboard::Left:
        switch (currentPage)
        {
        case GUIManager::PostDisplay:
            PreviousPost();
            break;
        case GUIManager::Comments:
            PreviousComment();
            break;
        case GUIManager::UserDisplay:
            PreviousFriend();
            break;
        case GUIManager::Groups:
            PreviousGroup();
            break;
        }
        break;
    default:
        ;
    }
}

void GUIManager::MousePressed()
{
    for (size_t i = 0; i < buttons[currentPage].size(); i++)
    {
        buttons[currentPage][i]->MouseClick(*window);
    }
    for (size_t i = 0; i < conditionalButtons[currentPage].size(); i++)
    {
        conditionalButtons[currentPage][i].first->MouseClick(*window);
    }
}
void GUIManager::MouseReleased()
{
}
void GUIManager::MousePosCheck()
{
    for (size_t i = 0; i < buttons[currentPage].size(); i++)
    {
        buttons[currentPage][i]->MouseOverCheck(*window);
    }
    for (size_t i = 0; i < conditionalButtons[currentPage].size(); i++)
    {
        conditionalButtons[currentPage][i].first->MouseOverCheck(*window);
    }
}

#pragma endregion

#pragma region Buttons
void GUIManager::InstantiateButtons()
{
#pragma region Main menu buttons
    buttons[MainMenu].emplace_back(new ImageButton(std::bind(&GUIManager::ToPostDisplay, this), 
        sf::Vector2f(500, 200), sf::Vector2f(.7, .7), sf::Vector2f(.8, .8),
        "Resources/GUI/PostsButton.png"));

    buttons[MainMenu].emplace_back(new ImageButton(std::bind(&GUIManager::SelectPostToAddButton, this), sf::Vector2f(1000, 200),
        sf::Vector2f(.7, .7), sf::Vector2f(.8, .8), "Resources/GUI/AddPostButton.png"));
    buttons[MainMenu].emplace_back(new ImageButton(std::bind(&GUIManager::ToProfilePageButton, this), sf::Vector2f(500, 500),
        sf::Vector2f(.7, .7), sf::Vector2f(.8, .8), "Resources/GUI/ProfileButton.png"));
    buttons[MainMenu].emplace_back(new ImageButton(std::bind(&GUIManager::ToAllUsers, this), sf::Vector2f(1000, 500),
        sf::Vector2f(.7, .7), sf::Vector2f(.8, .8), "Resources/GUI/AllUsers.png"));
#pragma endregion

#pragma region Post display buttons
    buttons[PostDisplay].emplace_back(new ImageButton(std::bind(&GUIManager::ToCommentsButton, this), sf::Vector2f(650, 700),
        sf::Vector2f(1, 1), sf::Vector2f(1.1, 1.1), "Resources/GUI/Comment.png"));
    buttons[PostDisplay].emplace_back(new ImageButton(std::bind(&GUIManager::ToMainMenuButton, this), sf::Vector2f(1300, 30),
        sf::Vector2f(1, 1), sf::Vector2f(1.1, 1.1), "Resources/GUI/Home.png"));
    buttons[PostDisplay].emplace_back(new ImageButton(std::bind(&GUIManager::CommentCurrentPost, this), sf::Vector2f(1100, 300),
        sf::Vector2f(.7, .7), sf::Vector2f(.8, .8), "Resources/GUI/AddComment.png"));

    conditionalButtons[PostDisplay].emplace_back(std::make_pair(new ImageButton(std::bind(&GUIManager::RemovePostButton, this),
        sf::Vector2f(1100, 600), sf::Vector2f(.7, .7), sf::Vector2f(.8, .8), "Resources/GUI/RemovePost.png"),
        [this]()
        {
            if (currentPost == nullptr) return false;
            return currentPost->getAuthorID() == DataManager::MainUser()->getUserID();
        }));

    conditionalButtons[PostDisplay].emplace_back(std::make_pair(new ImageButton(std::bind(&GUIManager::LikeCurrentPost, this), sf::Vector2f(200, 700),
        sf::Vector2f(.7, .7), sf::Vector2f(.8, .8), "Resources/GUI/Like.png"),
        [this]()
        {
            if (currentPost == nullptr) return false;
            return !std::count(currentPost->getLikes().begin(), currentPost->getLikes().end(), DataManager::MainUser()->getUserID());
        }));
    conditionalButtons[PostDisplay].emplace_back(std::make_pair(new ImageButton(std::bind(&GUIManager::LikeCurrentPost, this), sf::Vector2f(200, 700),
        sf::Vector2f(.7, .7), sf::Vector2f(.8, .8), "Resources/GUI/Dislike.png"),
        [this]()
        {
            if (currentPost == nullptr) return false;
            return (bool)std::count(currentPost->getLikes().begin(), currentPost->getLikes().end(), DataManager::MainUser()->getUserID());
        }));
#pragma endregion

#pragma region Comments buttons
    buttons[Comments].emplace_back(new ImageButton(std::bind(&GUIManager::ToPostDisplay, this), sf::Vector2f(1300, 30),
        sf::Vector2f(1, 1), sf::Vector2f(1.1, 1.1), "Resources/GUI/Back.png"));
    conditionalButtons[Comments].emplace_back(std::make_pair(new ImageButton(std::bind(&GUIManager::RemoveCommentButton, this), sf::Vector2f(1300, 500),
        sf::Vector2f(.7, .7), sf::Vector2f(.8, .8), "Resources/GUI/DeleteComment.png"),
        [this]()
        {
            if (currentComment == nullptr) return false;
            return currentComment->getAuthorID() == DataManager::MainUser()->getUserID();
        }));
#pragma endregion

#pragma region Profile page
    
    buttons[ProfilePage].emplace_back(new ImageButton(std::bind(&GUIManager::ToMainMenuButton, this), sf::Vector2f(1500, 0),
        sf::Vector2f(1, 1), sf::Vector2f(1.1, 1.1), "Resources/GUI/Home.png"));
    buttons[ProfilePage].emplace_back(new ImageButton(std::bind(&GUIManager::ToFriendsButton, this), sf::Vector2f(1300, 110),
        sf::Vector2f(.7, .7), sf::Vector2f(.8, .8), "Resources/GUI/FriendsButton.png"));
    buttons[ProfilePage].emplace_back(new ImageButton(std::bind(&GUIManager::ToGroupsButton, this), sf::Vector2f(1300, 310),
        sf::Vector2f(.7, .7), sf::Vector2f(.8, .8), "Resources/GUI/GroupsButton.png"));
    buttons[ProfilePage].emplace_back(new ImageButton(std::bind(&GUIManager::EditProfile, this), sf::Vector2f(1300, 510),
        sf::Vector2f(.7, .7), sf::Vector2f(.8, .8), "Resources/GUI/EditProfile.png"));
    buttons[ProfilePage].emplace_back(new ImageButton(std::bind(&GUIManager::RemoveProfile, this), sf::Vector2f(1300, 710),
        sf::Vector2f(.7, .7), sf::Vector2f(.8, .8), "Resources/GUI/DeleteProfile.png"));
    buttons[ProfilePage].emplace_back(new ImageButton(std::bind(&GUIManager::ToUsersPosts, this), sf::Vector2f(400, 510),
        sf::Vector2f(.7, .7), sf::Vector2f(.8, .8), "Resources/GUI/UserPostsButton.png"));
#pragma endregion

#pragma region User display
    buttons[UserDisplay].emplace_back(new ImageButton(std::bind(&GUIManager::ToFriendsButton, this), sf::Vector2f(1300, 200),
        sf::Vector2f(.7, .7), sf::Vector2f(.8, .8), "Resources/GUI/FriendsButton.png"));
    buttons[UserDisplay].emplace_back(new ImageButton(std::bind(&GUIManager::ToMainMenuButton, this), sf::Vector2f(1300, 30),
        sf::Vector2f(1, 1), sf::Vector2f(1.1, 1.1), "Resources/GUI/Home.png"));
    buttons[UserDisplay].emplace_back(new ImageButton(std::bind(&GUIManager::ToGroupsButton, this), sf::Vector2f(1300, 400),
        sf::Vector2f(.7, .7), sf::Vector2f(.8, .8), "Resources/GUI/GroupsButton.png"));
    buttons[UserDisplay].emplace_back(new ImageButton(std::bind(&GUIManager::ToUsersPosts, this), sf::Vector2f(400, 650),
        sf::Vector2f(.7, .7), sf::Vector2f(.8, .8), "Resources/GUI/UserPostsButton.png"));

    conditionalButtons[UserDisplay].emplace_back(std::make_pair(new ImageButton(std::bind(&GUIManager::AddFriend, this), sf::Vector2f(400, 400),
        sf::Vector2f(.7, .7), sf::Vector2f(.8, .8), "Resources/GUI/AddFriend.png"),
        [this]()
        {
            if (currentUser == nullptr) return false;
            if(currentUser->getUserID() == DataManager::MainUser()->getUserID())return false;
            return !std::count(DataManager::MainUser()->getFriends().begin(), DataManager::MainUser()->getFriends().end(), currentUser->getUserID());
        }));
    conditionalButtons[UserDisplay].emplace_back(std::make_pair(new ImageButton(std::bind(&GUIManager::RemoveFriend, this), sf::Vector2f(400, 400),
        sf::Vector2f(.7, .7), sf::Vector2f(.8, .8), "Resources/GUI/RemoveFriend.png"),
        [this]()
        {
            if (currentUser == nullptr) return false;
            if (currentUser->getUserID() == DataManager::MainUser()->getUserID())return false;
            return (bool)std::count(DataManager::MainUser()->getFriends().begin(), DataManager::MainUser()->getFriends().end(), currentUser->getUserID());
        }));
#pragma endregion

#pragma region Groups
    buttons[Groups].emplace_back(new ImageButton(std::bind(&GUIManager::ToMainMenuButton, this), sf::Vector2f(1300, 30),
        sf::Vector2f(1, 1), sf::Vector2f(1.1, 1.1), "Resources/GUI/Home.png"));
    buttons[Groups].emplace_back(new ImageButton(std::bind(&GUIManager::ToGroupPosts, this), sf::Vector2f(100, 500),
        sf::Vector2f(.7, .7), sf::Vector2f(.8, .8), "Resources/GUI/PostsButton.png"));
    buttons[Groups].emplace_back(new ImageButton(std::bind(&GUIManager::ToGroupMembersButton, this), sf::Vector2f(500, 500),
        sf::Vector2f(.7, .7), sf::Vector2f(.8, .8), "Resources/GUI/GroupMembers.png"));
    conditionalButtons[Groups].emplace_back(std::make_pair(new ImageButton(std::bind(&GUIManager::JoinGroup, this), sf::Vector2f(100, 700),
        sf::Vector2f(.7, .7), sf::Vector2f(.8, .8), "Resources/GUI/JoinGroup.png"),
        [this]()
        {
            if (currentGroup == nullptr) return false;
            return !std::count(DataManager::MainUser()->getGroups().begin(), DataManager::MainUser()->getGroups().end(), currentGroup->getID());
        }));
    conditionalButtons[Groups].emplace_back(std::make_pair(new ImageButton(std::bind(&GUIManager::LeaveGroup, this), sf::Vector2f(100, 700),
        sf::Vector2f(.7, .7), sf::Vector2f(.8, .8), "Resources/GUI/LeaveGroup.png"),
        [this]()
        {
            if (currentGroup == nullptr) return false;
            return (bool)std::count(DataManager::MainUser()->getGroups().begin(), DataManager::MainUser()->getGroups().end(), currentGroup->getID());
        }));
    conditionalButtons[Groups].emplace_back(std::make_pair(new ImageButton(std::bind(&GUIManager::SelectPostToAddButton, this), sf::Vector2f(500, 700),
        sf::Vector2f(.7, .7), sf::Vector2f(.8, .8), "Resources/GUI/AddPostButton.png"),
        [this]()
        {
            if (currentGroup == nullptr) return false;
            return (bool)std::count(DataManager::MainUser()->getGroups().begin(), DataManager::MainUser()->getGroups().end(), currentGroup->getID());
        }));

#pragma endregion

#pragma region Posting
    buttons[Posting].emplace_back(new ImageButton(std::bind(&GUIManager::CreateTextPost, this), sf::Vector2f(300, 300),
        sf::Vector2f(.7, .7), sf::Vector2f(.8, .8), "Resources/GUI/TextPostButton.png"));
    buttons[Posting].emplace_back(new ImageButton(std::bind(&GUIManager::CreateImagePost, this), sf::Vector2f(900, 300),
        sf::Vector2f(.7, .7), sf::Vector2f(.8, .8), "Resources/GUI/ImagePostButton.png"));
    buttons[Posting].emplace_back(new ImageButton(std::bind(&GUIManager::CreateVideoPost, this), sf::Vector2f(300, 600),
        sf::Vector2f(.7, .7), sf::Vector2f(.8, .8), "Resources/GUI/VideoPostButton.png"));
    buttons[Posting].emplace_back(new ImageButton(std::bind(&GUIManager::CreateAudioPost, this), sf::Vector2f(900, 600),
        sf::Vector2f(.7, .7), sf::Vector2f(.8, .8), "Resources/GUI/AudioPostButton.png"));

    buttons[Posting].emplace_back(new ImageButton(std::bind(&GUIManager::ToMainMenuButton, this), sf::Vector2f(1500, 300),
        sf::Vector2f(1, 1), sf::Vector2f(1.1, 1.1), "Resources/GUI/Home.png"));

#pragma endregion

    //Show all starting buttons
    for (size_t i = 0; i < buttons[currentPage].size(); i++)
    {
        buttons[currentPage][i]->ShowButton();
    }
}

void GUIManager::ConditionalButtonsCheck()
{
    for (size_t j = 0; j < conditionalButtons[currentPage].size(); j++)
    {
        if (conditionalButtons[currentPage][j].second())
            conditionalButtons[currentPage][j].first->ShowButton();
        else
            conditionalButtons[currentPage][j].first->HideButton();
    }
}
void GUIManager::ButtonsVisualUpdate()
{
    for (size_t i = 0; i < buttons[currentPage].size(); i++)
    {
        buttons[currentPage][i]->VisualUpdate(*window);
    }
    for (size_t j = 0; j < conditionalButtons[currentPage].size(); j++)
    {
        conditionalButtons[currentPage][j].first->VisualUpdate(*window);
    }
}
#pragma region Button functions
void GUIManager::AddFriend()
{
    if (currentUser == nullptr) return;
    if (DataManager::MainUser()->AddFriend(currentUser->getUserID()))
    {
        FileManager::SaveAddedFriend(DataManager::MainUser()->getUserID(), currentUser->getUserID());
        DataManager::LoadPostsToView();
        ConditionalButtonsCheck();
    }
}
void GUIManager::RemoveFriend()
{
    if (currentUser == nullptr) return;
    if (DataManager::MainUser()->RemoveFriend(currentUser->getUserID()))
    {
        FileManager::RemoveFriend(DataManager::MainUser()->getUserID(), currentUser->getUserID());
        DataManager::LoadPostsToView();
        if (DataManager::MainUser()->getFriends().size() == 0)
            ToMainMenuButton();
        else
             ToUserDisplay();
    }
}
void GUIManager::JoinGroup()
{
    if (currentGroup == nullptr) return;
    if (DataManager::getGroup(currentGroup->getID()).AddUser(DataManager::MainUser()->getUserID()))
    {
        FileManager::SaveAddedGroupUser(currentGroup->getID(), DataManager::MainUser()->getUserID());
        DataManager::LoadPostsToView();

        ConditionalButtonsCheck();
    }
}
void GUIManager::LeaveGroup()
{
    if (currentGroup == nullptr) return;
    if (DataManager::getGroup(currentGroup->getID()).RemoveUser(DataManager::MainUser()->getUserID()))
    {
        FileManager::RemoveGroupUser(currentGroup->getID(), DataManager::MainUser()->getUserID());
        DataManager::LoadPostsToView();

        if (DataManager::MainUser()->getGroups().size() == 0)
            ToMainMenuButton();
        else
             ToGroupsButton();
    }
}
void GUIManager::ToMainMenuButton() 
{ 
    currentlyWatchedUser = DataManager::MainUser();
    currentUser = DataManager::MainUser();
    targetetPosts = &DataManager::getLoadedPosts();
    currentPostIndex = 0; 
    currentGroupIndex = -1;
    SwitchPage(MainMenu); 
}
void GUIManager::ToProfilePageButton() 
{ 
    currentGroupIndex = 0;
    currentlyWatchedUser = DataManager::MainUser();
    currentUser = DataManager::MainUser();
    SwitchPage(ProfilePage); 
}
void GUIManager::ToPostDisplay()
{ 
    if (targetetPosts == nullptr || targetetPosts->size() == 0) return;
    currentPost = targetetPosts->at(currentPostIndex).get();
    SwitchPage(PostDisplay); 
}
void GUIManager::ToUserDisplay()
{
    if (targetedUsers == nullptr) return;
    currentUser = &DataManager::getUser(targetedUsers->at(currentUserIndex));
    SwitchPage(UserDisplay);
    ConditionalButtonsCheck();
}
void GUIManager::ToUsersPosts()
{
    if (currentUser->getPosts().size() == 0) return;

    currentPostIndex = 0;
    targetetPosts = &currentUser->getPosts();

    ToPostDisplay();
    ConditionalButtonsCheck();
}
void GUIManager::ToCommentsButton() 
{ 
    if (currentPost->getComments().size() == 0) return;
    currentCommentIndex = 0;
    currentComment = &currentPost->getComments()[0];
    SwitchPage(Comments);
    ConditionalButtonsCheck();

}
void GUIManager::ToFriendsButton()
{
    if (currentUser->getFriends().size() == 0) return;
    targetedUsers = &currentUser->getFriends();
    currentlyWatchedUser = currentUser;
    currentUserIndex = 0;
    currentGroup = nullptr;
    ToUserDisplay();

}
void GUIManager::ToGroupMembersButton()
{
    if (currentGroup->getUsers().size() == 0) return;

    targetedUsers = &currentGroup->getUsers();
    currentUserIndex = 0;
    currentlyWatchedUser = nullptr;

    ToUserDisplay();

}
void GUIManager::ToAllUsers()
{
    targetedUsers = &DataManager::getUsersIDs();
    currentUserIndex = 0;
    currentlyWatchedUser = nullptr;
    currentGroup = nullptr;
    ToUserDisplay();
}
void GUIManager::ToGroupsButton()
{
    if (currentUser->getGroups().size() == 0) return;
    currentGroupIndex = 0;
    currentGroup = &DataManager::getGroup(currentUser->getGroups()[0]);
    SwitchPage(Groups);
    ConditionalButtonsCheck();

}
void GUIManager::ToGroupPosts()
{
    if (currentGroup->getPosts().size() == 0) return;
    targetetPosts = &currentGroup->getPosts();
    currentPostIndex = 0;
    ToPostDisplay();
}
void GUIManager::SelectPostToAddButton()
{
    SwitchPage(Posting);
}

void  GUIManager::CreateTextPost()
{
    system("cls");

    std::string content;
    std::cout << "Wprowadz tresc postu i zatwierdz enterem:\n";
    std::getline(std::cin, content);

    int groupID = -1;
    if (currentGroupIndex >= 0)
    {
        groupID = DataManager::MainUser()->getGroups()[currentGroupIndex];
    }

    DataManager::MainUser()->AddTextPost(groupID, content);

    ToMainMenuButton();
    std::cout << "\n----Pomyslnie dodano post, mozesz wrocic do programu----";
}
void GUIManager::CreateImagePost()
{
    system("cls");

    std::string description;
    std::cout << "Wprowadz opis zdjecia i zatwierdz enterem:\n";
    std::getline(std::cin, description);

    std::string fileName;
    std::cout << "\nWprowadz nazwe pliku ze zdjeciem. Finalna lokalizacja to:\nResources/Pictures/(wprowadzona nazwa).png\n";
    std::getline(std::cin, fileName);
    
    std::string imagePath = "Resources/Pictures/" + fileName + ".png";

    int groupID = -1;
    if (currentGroupIndex >= 0)
    {
        groupID = DataManager::MainUser()->getGroups()[currentGroupIndex];
    }

    DataManager::MainUser()->AddImagePost(groupID, description, imagePath);

    ToMainMenuButton();
    std::cout << "\n----Pomyslnie dodano post, mozesz wrocic do programu----";
}
void  GUIManager::CreateVideoPost()
{
    system("cls");

    std::string description;
    std::cout << "Wprowadz opis zdjecia i zatwierdz enterem:\n";
    std::getline(std::cin, description);

    std::string fileName;
    std::cout << "\nWprowadz nazwe pliku wideo. Finalna lokalizacja to:\nResources/Video/(wprowadzona nazwa).png\n";
    std::getline(std::cin, fileName);

    std::string videoPath = "Resources/Video/" + fileName + ".png";

    int groupID = -1;
    if (currentGroupIndex >= 0)
    {
        groupID = DataManager::MainUser()->getGroups()[currentGroupIndex];
    }

    DataManager::MainUser()->AddVideoPost(groupID, description, videoPath);

    ToMainMenuButton();
    std::cout << "\n----Pomyslnie dodano post, mozesz wrocic do programu----";
}
void  GUIManager::CreateAudioPost()
{
    system("cls");
    std::string fileName;
    std::cout << "Wprowadz nazwe pliku audio. Finalna lokalizacja to:\nResources/Audio/(wprowadzona nazwa).png\n";
    std::getline(std::cin, fileName);

    std::string audioPath = "Resources/Audio/" + fileName + ".png";

    int groupID = -1;
    if (currentGroupIndex >= 0)
    {
        groupID = DataManager::MainUser()->getGroups()[currentGroupIndex];
    }

    DataManager::MainUser()->AddAudioPost(groupID, audioPath);

    ToMainMenuButton();
    std::cout << "\n----Pomyslnie dodano post, mozesz wrocic do programu----";

}
void GUIManager::RemovePostButton()
{
    if (currentPost == nullptr) return;
    int postID = currentPost->getID();

    currentPostIndex = 0;
    ToUsersPosts();

    DataManager::MainUser()->RemovePost(postID);
}
void GUIManager::RemoveCommentButton()
{
    if (currentComment == nullptr || currentPost == nullptr) return;
    currentPost->RemoveComment(currentComment->getID());
    ToPostDisplay();
}
void GUIManager::LikeCurrentPost()
{
    if (currentPost == nullptr) return;
    currentPost->Like_Unlike(DataManager::MainUser()->getUserID());
    ConditionalButtonsCheck();
}
void GUIManager::CommentCurrentPost()
{
    if (currentPost == nullptr) return;
    system("cls");
    std::cout << "Napisz komentarz i zatwierdz enterem\n"; 
    std::string content;
    std::getline(std::cin, content);
    currentPost->AddComment(DataManager::MainUser()->getUserID(), content);
    std::cout << "\n-------KOMENTARZ DODANY, WROC DO APLIKACJI-------\n";
}
void GUIManager::EditProfile()
{
    system("cls");
    std::string name, surname, por, description, ppp;
    User::Gender gender;
    int age = 30;
   
    ReadUserData(name, surname, por, description, ppp, gender, age);

    DataManager::MainUser()->ChangePersonalData(name, surname, age, gender, por, description, ppp);

    std::cout << "-----EDYCJA DANYCH POWIODLA SIE, WROC DO PROGRAMU-----\n";
}
void GUIManager::RemoveProfile()
{
    DataManager::RemoveUser(DataManager::MainUser()->getUserID());
    window->close();
}
#pragma endregion

#pragma endregion

#pragma region Page control

void GUIManager::SwitchPage(Page newPage)
{
    if (currentPage == newPage) return;
    
    for (size_t i = 0; i < buttons[currentPage].size(); i++)
    {
        buttons[currentPage][i]->HideButton();
    }
    for (size_t j = 0; j < conditionalButtons[currentPage].size(); j++)
    {
        conditionalButtons[currentPage][j].first->HideButton();
    }

    currentPage = newPage;

    for (size_t i = 0; i < buttons[currentPage].size(); i++)
    {
        buttons[currentPage][i]->ShowButton();
    }
    for (size_t j = 0; j < conditionalButtons[currentPage].size(); j++)
    {
        if(conditionalButtons[currentPage][j].second())
             conditionalButtons[currentPage][j].first->ShowButton();
    }
}

#pragma region Next/Previous
void GUIManager::NextPost()
{
    if (currentPostIndex + 1 >= targetetPosts->size()) return;
    currentPostIndex++;
    currentPost = targetetPosts->at(currentPostIndex).get();
    ConditionalButtonsCheck();

}
void GUIManager::PreviousPost()
{
    if (currentPostIndex - 1 < 0) return;
    currentPostIndex--;
    currentPost = targetetPosts->at(currentPostIndex).get();
    ConditionalButtonsCheck();

}
void GUIManager::NextComment()
{
    if (currentCommentIndex + 1 >= targetetPosts->at(currentPostIndex)->getComments().size()) return;
    currentCommentIndex++;
    currentComment = &targetetPosts->at(currentPostIndex)->getComments()[currentCommentIndex];

    ConditionalButtonsCheck();
}
void GUIManager::PreviousComment()
{
    if (currentCommentIndex - 1 < 0) return;
    currentCommentIndex--;
    currentComment = &targetetPosts->at(currentPostIndex)->getComments()[currentCommentIndex];

    ConditionalButtonsCheck();
}
void GUIManager::NextFriend()
{
    if (currentUserIndex + 1 >= targetedUsers->size()) return;
    currentUserIndex++;
    currentUser = &DataManager::getUser(targetedUsers->at(currentUserIndex));
    ConditionalButtonsCheck();

}
void GUIManager::PreviousFriend()
{
    if (currentUserIndex - 1 < 0) return;
    currentUserIndex--;
    currentUser = &DataManager::getUser(targetedUsers->at(currentUserIndex));
    ConditionalButtonsCheck();

}
void GUIManager::NextGroup()
{
    if (currentGroupIndex + 1 >= currentUser->getGroups().size()) return;
    currentGroupIndex++;
    currentGroup = &DataManager::getGroup(currentUser->getGroups()[currentGroupIndex]);
    ConditionalButtonsCheck();

}
void GUIManager::PreviousGroup()
{
    if (currentGroupIndex - 1 < 0) return;
    currentGroupIndex--;
    currentGroup = &DataManager::getGroup(currentUser->getGroups()[currentGroupIndex]);
    ConditionalButtonsCheck();

}
#pragma endregion

#pragma endregion

#pragma region Helpers
bool GUIManager::IsWindowOpen()
{
    return window->isOpen();
}
void GUIManager::OpenWindow(int width, int height)
{
    window = new sf::RenderWindow(sf::VideoMode(width, height), "Social Media");
}
void GUIManager::ReadUserData(std::string& name, std::string& surname, std::string& placeOfResidence,
    std::string& description, std::string& profilePicturePath, User::Gender& gender, int& age)
{
    std::string picturePath;
    do
    {
        std::cout << "Wprowadz nowe imie:\n";
        std::cin >> name;
        system("cls");
        if (name == "")
        {
            std::cout << "Imie nie moze byc puste !\n";
        }
    } while (name == "");
    do
    {
        std::cout << "Wprowadz nowe nazwisko:\n";
        std::cin >> surname;
        system("cls");
        if (surname == "")
        {
            std::cout << "Nazwisko nie moze byc puste !\n";
        }
    } while (surname == "");

    std::cin.ignore();
    std::cout << "Wprowadz nowe miejsce zamieszkania:\n";
    std::getline(std::cin, placeOfResidence);
    system("cls");

    std::cout << "Wprowadz nowy opis:\n";
    std::getline(std::cin, description);
    system("cls");

    while (true)
    {
        std::cout << "Wprowadz nowy wiek:\n";
        std::cin >> age;
        system("cls");
        if (!std::cin)
        {
            std::cout << "Wprowadz liczbe!\n";
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        }
        else
        {
            if (age <= 0)
            {
                std::cout << "Wprowadz liczbe dodatnia!\n";
                std::cin.clear();
                std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                continue;
            }
            break;
        }
    }
    char genderChar;
    do
    {
        std::cout << "Wprowadz nowa plec: (m - mezczyzna, k - kobieta, i - inne)\n";
        std::cin >> genderChar;
        system("cls");
        switch (genderChar)
        {
        case 'm':
            gender = User::M;
            break;
        case 'k':
            gender = User::F;
            break;
        case 'i':
            gender = User::O;
            break;
        default:
            std::cout << "Wpisz m / k / i !\n";
        }
    } while (genderChar != 'm' && genderChar != 'k' && genderChar != 'i');

    std::cin.ignore();
    std::cout << "Wprowadz nowa sciezke zdjecia profilowego:  (Resources/Pictures/(wprowadzona nazwa).png\n";
    std::getline(std::cin, picturePath);
    system("cls");
    profilePicturePath = "Resources/Pictures/" + picturePath + ".png";
}
#pragma endregion

GUIManager::GUIManager(int _width, int _height) : width(_width), height(_height),
    currentlyWatchedUser(DataManager::MainUser()), targetetPosts(&DataManager::getLoadedPosts()),
    currentUser(DataManager::MainUser()), currentPage(MainMenu)
{
    defaultFont.loadFromFile(defaultFontPath);

    OpenWindow(width, height);

    //Buttons
    InstantiateButtons();  

    ToMainMenuButton();
}
GUIManager::~GUIManager()
{
    delete window;
}
