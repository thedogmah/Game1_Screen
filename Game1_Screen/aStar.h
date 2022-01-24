#pragma once
#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <SFML/Audio.hpp>
#include <SFML/Network.hpp>
#include <vector>
#include <iostream>
//#include "oldConsoleGameEngine.h"
#include <string>
#include <vector>
#include <iostream>
#include <algorithm>
#include <list>

class aStar
{

public:

	aStar();

	struct paths {

		float x;
		float y;

		paths(float x, float y)
		{
			this->x = x;
			this->y = y;
		}
		paths() {
			x = 0;
			y = 0;
		}
	};

	std::vector<paths> path;

	struct sNode {


		bool bObstacle = false;
		bool bVisited = false;
		float fGlobalGoal;		//Distance to goal so far
		float fLocalGoal;		//Distance to goal if we took alternative
		float x;
		float y;
		std::vector<sNode*> vecNeighbours;
		sNode* parent;

	};
	sf::VertexArray vaGrid;
	sf::VertexArray vaLine;
	std::vector<sf::Vertex> gridRecs;
	sf::Event ev;
	sNode* nodes = nullptr;
	int nMapWidth;
	int nMapHeight;

	sNode* nodeStart = nullptr;
	sNode* nodeEnd = nullptr;
	sNode* nodeEndBuffer = nullptr;
	sNode* nodeStartBuffer = nullptr;


	sf::Vector2f currentSteps;
	sf::Vector2f prevSteps;

		
public:
	virtual bool OnUserCreate();
	
	bool checkObstacle(int, int);
	
	void init();
	bool solve_AStar();
	std::vector<paths> pathGangShit(std::vector<paths>);
	paths isFacing(paths);



public:
	 std::vector<paths> OnUserUpdate(float fElapsedTime);
	
	
};



//int main()
//{
//	//olcConsoleGameEngine game;
//	Console_PathFinder game;
//	game.ConstructConsole(120, 120, 6, 6);
//	game.Start();
//	std::cout << "done";
//
//	//ryan.onUserCreate();
//	return 0;
//}