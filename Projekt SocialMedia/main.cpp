#include <iostream>
#include <vector>
#include <string>
#include "User.h"
#include "TextPost.h"
#include "GUIManager.h"
#include "DataManager.h"


int main()
{
    DataManager::LoadData();

    GUIManager gui(1920, 1080);

    while (gui.IsWindowOpen())
    {
        gui.RenderLoop();
    }
}
