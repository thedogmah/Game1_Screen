#pragma once
#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <SFML/Audio.hpp>
#include <SFML/Network.hpp>

#include "imgui/imgui.h"
#include "imgui/imgui-SFML.h"
#include <string>
#include <iostream>
#include <sstream>
#include <fstream>
#include <istream>
#include <vector>
//#include "Animation.h"
#include "aStar.h"
#include "Population.h"
class Game;
class socialEngine
{
public:

	socialEngine();
	bool bShowInteract = false;
	//aStar astar{};
	Game* game;
	void interact(Animation* npc);
	void interactParty(std::vector<Animation*> npc);
	void disbandParty(std::vector<Animation*> npc);
	void worldChat(std::map<std::string, std::string> clientmap);
	void partyVisuals(std::vector<Animation*> npc);
	void drawGrid();
	void drawSocial();
	void moveNPCs(std::vector<Animation*> npc);
	//get the currently selected npc
	Animation* selectedNpc;
	//Animation selectedClone;
	Population worldBase;
	std::string sLabelChoose = "Choose interaction with: ";
	Animation* vInteract;

	//resources for drawing to the world
	//receives pointers to each member in the current social gathering
	//Is use to create text based on its size (text per person) and uses their location, in addition to drawing their mug shot on the appropriate UIs.
	std::vector<Animation*> vInteraction;

	std::vector<sf::Sprite> vPartySpeechBubbles;

	//image resources
	sf::Texture bubbleTexture;
	sf::Texture bubbleTexture2;
	sf::Texture bubbleTexture3;
	sf::Texture bubbleTexture4;
	sf::Texture bubbleTexture5;
	sf::Texture bubbleTexture6;
	sf::Texture bubbleTexture7;
	sf::Texture bubbleTexture8;
	sf::Texture bubbleTexture9;

	//dynamic bool + party dynamics / variables
	bool partySizeChange = false;
	bool partyStarted = false;
	sf::CircleShape partyGrid;
	sf::Vector2f partyAnchor;
	sf::Clock socialPartyTimer;
	float deltaTime;
	float partyTime;

};