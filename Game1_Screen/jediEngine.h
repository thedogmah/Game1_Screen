#pragma once
#include <string>
#include <iostream>
#include <sstream>
#include <fstream>
#include <istream>
#include <vector>
#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <SFML/Audio.hpp>
#include <SFML/Network.hpp>
class jediEngine
{

public:

	jediEngine();
	void loadAssetFile();
	void addAsset(char* asset);
	void saveAssets();
	std::fstream assetFstream;
	std::stringstream ssAsset;
	std::string assetName;
	std::string currentAsset;
	std::vector<std::string> vAssetLabel;
	std::vector<sf::RectangleShape> vAssets;
	
	//Asset image resources
	sf::Texture assetTexture;
	sf::Image assetImage;
	sf::RectangleShape assetRect;
};

