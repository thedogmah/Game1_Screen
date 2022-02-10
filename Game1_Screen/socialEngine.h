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
#include "Animation.h"
#include "aStar.h"
#include "Population.h"
class Game;
class socialEngine
{
public:
	
	socialEngine();
	bool bShowInteract = false;
	aStar astar{};
	Game* game;
	void interact(Animation *npc);
	//get the currently selected npc
	Animation* selectedNpc;
	Animation selectedClone;
	Population worldBase;
	std::string sLabelChoose = "Choose interaction with: ";

	std::vector<Animation> *vInteraction;

};