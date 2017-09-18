#include <menu.hpp>

void Menu::loadOptionsConfig()
{
	std::ifstream   save;
	std::string     line;

	save.open(std::string("save/options.config"), std::ios::in);
	if (!save.is_open())
		return Menu::loadDefaultOptionsConfig();
	while (std::getline(save, line))
	{
		std::vector<std::string> strSplits = MainGame::stringSplit(line, ' ');
		if (strSplits[0] == std::string("MusicVolume")){
			Menu::options.musicVolume = (float)std::atof(strSplits[1].c_str());
		}else if (strSplits[0] == std::string("SoundVolume")){
			Menu::options.soundVolume = (float)std::atof(strSplits[1].c_str());
		}else if (strSplits[0] == std::string("Mute")){
			Menu::options.mute = (bool)std::atoi(strSplits[1].c_str());
		}else if (strSplits[0] == std::string("ResolutionIndex")){
			Menu::options.resolutionIndex = std::atoi(strSplits[1].c_str());
		}else if (strSplits[0] == std::string("FullScreen")){
			Menu::options.fullScreen = (bool)std::atoi(strSplits[1].c_str());
		}else if (strSplits[0] == std::string("UpKey")){
			Menu::options.moveUp.name = strSplits[1];
			Menu::options.moveUp.glfwValue = std::atoi(strSplits[2].c_str());
		}else if (strSplits[0] == std::string("DownKey")){
			Menu::options.moveDown.name = strSplits[1];
			Menu::options.moveDown.glfwValue = std::atoi(strSplits[2].c_str());
		}else if (strSplits[0] == std::string("LeftKey")){
			Menu::options.moveLeft.name = strSplits[1];
			Menu::options.moveLeft.glfwValue = std::atoi(strSplits[2].c_str());
		}else if (strSplits[0] == std::string("RightKey")){
			Menu::options.moveRight.name = strSplits[1];
			Menu::options.moveRight.glfwValue = std::atoi(strSplits[2].c_str());
		}else if (strSplits[0] == std::string("PlaceBombKey")){
			Menu::options.placeBomb.name = strSplits[1];
			Menu::options.placeBomb.glfwValue = std::atoi(strSplits[2].c_str());
		}else if (strSplits[0] == std::string("PauseKey")){
			Menu::options.pause.name = strSplits[1];
			Menu::options.pause.glfwValue = std::atoi(strSplits[2].c_str());
		}
	}
}

void Menu::loadDefaultOptionsConfig()
{
	Menu::options.musicVolume = 1.0f;
	Menu::options.soundVolume = 1.0f;
	Menu::options.mute = false;
	Menu::options.resolutionIndex = (int)Menu::options.resolutionList.size() - 1;
	Menu::options.fullScreen = false;
	Menu::myGlfwGetKeyName(GLFW_KEY_UP, Menu::options.moveUp.name);
	Menu::options.moveUp.glfwValue = GLFW_KEY_UP;
	Menu::myGlfwGetKeyName(GLFW_KEY_DOWN, Menu::options.moveDown.name);
	Menu::options.moveDown.glfwValue = GLFW_KEY_DOWN;
	Menu::myGlfwGetKeyName(GLFW_KEY_LEFT, Menu::options.moveLeft.name);
	Menu::options.moveLeft.glfwValue = GLFW_KEY_LEFT;
	Menu::myGlfwGetKeyName(GLFW_KEY_RIGHT, Menu::options.moveRight.name);
	Menu::options.moveRight.glfwValue = GLFW_KEY_RIGHT;
	Menu::options.placeBomb.name = "Space";
	Menu::options.placeBomb.glfwValue = GLFW_KEY_SPACE;
	Menu::options.pause.name = "Esc";
	Menu::options.pause.glfwValue = GLFW_KEY_ESCAPE;
	Menu::copyOptions(Menu::tmpOptions, Menu::options);
}

void Menu::saveOptionsConfig()
{
	std::ofstream   save;

	save.open(std::string("save/options.config"), std::ios::out);
	if (!save.is_open())
		return;
	save << "MusicVolume " << Menu::options.musicVolume << std::endl;
	save << "SoundVolume " << Menu::options.soundVolume << std::endl;
	save << "Mute " << Menu::options.mute << std::endl;
	save << "ResolutionIndex " << Menu::options.resolutionIndex << std::endl;
	save << "FullScreen " << Menu::options.fullScreen << std::endl;
	save << "UpKey " << Menu::options.moveUp.name << " " << Menu::options.moveUp.glfwValue << std::endl;
	save << "DownKey " << Menu::options.moveDown.name << " " << Menu::options.moveDown.glfwValue << std::endl;
	save << "LeftKey " << Menu::options.moveLeft.name << " " << Menu::options.moveLeft.glfwValue << std::endl;
	save << "RightKey " << Menu::options.moveRight.name << " " << Menu::options.moveRight.glfwValue << std::endl;
	save << "PlaceBombKey " << Menu::options.placeBomb.name << " " << Menu::options.placeBomb.glfwValue << std::endl;
	save << "PauseKey " << Menu::options.pause.name << " " << Menu::options.pause.glfwValue << std::endl;
	save.close();
}