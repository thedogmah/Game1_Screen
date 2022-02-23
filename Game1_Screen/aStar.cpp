#include "aStar.h"
#include "Game.h"
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
#include <Windows.h>
#include <algorithm>
#include <list>

aStar::aStar() {
	this->nMapWidth = 119; this->nMapHeight = 83 ;
	this->init();
		}
	//m_sAppName = L"Path Finding";
	//std::cout << "hi";



	struct sNode {

		bool bObstacle = false;
		bool bVisited = false;
		float fGlobalGoal;		//Distance to goal so far
		float fLocalGoal;		//Distance to goal if we took alternative
		int x;
		int y;
		std::vector<sNode*> vecNeighbours;
		sNode* parent;

	};


	struct paths {

		float x;
		float y;

		paths(float x, float y)
		{
			this->x = x;
			this->y = y;
		}
		paths();
	};

	std::vector<paths> path;
	
	sNode* nodes = nullptr;


	sNode* nodeStart = nullptr;
	sNode* nodeEnd = nullptr;
	
	
	
	
	void aStar::init()
	{
	//	vaGrid.setPrimitiveType(sf::Lines);
	//	vaGrid.resize((nMapHeight * nMapWidth) * 2);
	//	
	//	vaLine.setPrimitiveType(sf::Lines);
	//	vaLine.resize((nMapHeight * nMapWidth) * 2);


	//	int nNodeSize = 100;
	//int nNodeBorder = 0;
	//for (int x = 0; x < nMapHeight; ++x)
	//{
	//	sf::Vertex* line = &vaGrid[x * 2];
	//	//std::cout << nMapWidth << ", "<<  x <<", "<< y << "\n";
	//// define its 2/4 points 
	//	line[0].position = sf::Vector2f(0, (x + 1) * nNodeSize);
	//	line[1].position = sf::Vector2f(nMapWidth * nNodeSize, (x + 1) * nNodeSize);
	//	line[0].color = sf::Color::Green;
	//	line[1].color = sf::Color::Green;


	//	for (int y = 0; y < nMapWidth; ++y) {
	//		sf::Vertex* hLine = &vaLine[y * 2];

	//		hLine[0].position = sf::Vector2f(y * nNodeSize, 0);
	//		hLine[1].position = sf::Vector2f(y * nNodeSize, nMapHeight * nNodeSize);
	//	hLine[0].color = sf::Color::Green;
	//	hLine[1].color = sf::Color::Green;


	//	sf::RectangleShape rectangle;
	//		rectangle.setSize(sf::Vector2f(nNodeSize, nNodeSize));
	//		rectangle.setOutlineColor(sf::Color::Red);
	//		rectangle.setOutlineThickness(5);
	//		rectangle.setPosition((x * nNodeSize), (y * nNodeSize));

	//	//	this->gridRecs.push_back(line);

	//	}
	}
				
				

		


	bool aStar::checkObstacle(int x, int y)
	{
	
		return true;
	}

	 bool aStar::OnUserCreate()
	{

		 std::fstream pathDataW;
		 std::vector<std::vector<char>> vPathCollide;
		 std::vector<char> vPathInts;
		 pathDataW.open("pathdata.txt");
		 if (pathDataW.is_open())
			
			 {
				 std::string line;
				 std::cout << std::endl;
				 while (std::getline(pathDataW, line))
				 {
					 //	std::cout << line;
					 for (auto &digit : line)
					 {
						 vPathInts.push_back(digit);
					 }
					 vPathCollide.push_back(vPathInts);
					 vPathInts.clear();
				 }
			 }

		



		nodes = new sNode[nMapWidth * nMapHeight];

		for (int x = 0; x < nMapWidth; x++)

		{
			for (int y = 0; y < nMapHeight; y++)
			{
				nodes[y * nMapWidth + x].x = x;
				nodes[y * nMapWidth + x].y = y;
				if (vPathCollide[x][y] == '1') 
					nodes[y * nMapWidth + x].bObstacle = true;
				else 
					nodes[y * nMapWidth + x].bObstacle = false;
				nodes[y * nMapWidth + x].bVisited = false;
				nodes[y * nMapWidth + x].parent = nullptr;
				//
							//	std::cout << nodes[y * x].x << ", " << nodes[y * x].y << " ";
			}

		}





		for (int x = 0; x < nMapWidth; x++)

		{
			for (int y = 0; y < nMapHeight; y++)
			{
				if (y > 0)
					nodes[y * nMapWidth + x].vecNeighbours.push_back(&nodes[(y - 1) * nMapWidth + (x + 0)]);
				if (y < nMapHeight - 1)
					nodes[y * nMapWidth + x].vecNeighbours.push_back(&nodes[(y + 1) * nMapWidth + (x + 0)]);
				if (x > 0)
					nodes[y * nMapWidth + x].vecNeighbours.push_back(&nodes[(y + 0) * nMapWidth + (x - 1)]);
				if (x < nMapWidth - 1)
					nodes[y * nMapWidth + x].vecNeighbours.push_back(&nodes[(y + 0) * nMapWidth + (x + 1)]);

			}

		}

		nodeStart = &nodes[2];
		nodeEnd = &nodes[3];
		return true;
	}

	bool aStar::solve_AStar()
	{
		// Reset Navigation Graph - default all node states
		for (int x = 0; x < nMapWidth; x++)
			for (int y = 0; y < nMapHeight; y++)
			{
				nodes[y * nMapWidth + x].bVisited = false;
				nodes[y * nMapWidth + x].fGlobalGoal = INFINITY;
				nodes[y * nMapWidth + x].fLocalGoal = INFINITY;
				nodes[y * nMapWidth + x].parent = nullptr;	// No parents
			}

		auto distance = [](sNode* a, sNode* b) // For convenience
		{
			return sqrtf((a->x - b->x) * (a->x - b->x) + (a->y - b->y) * (a->y - b->y));
		};

		auto heuristic = [distance](sNode* a, sNode* b) // So we can experiment with heuristic
		{
			return distance(a, b);
		};

		// Setup starting conditions
		sNode* nodeCurrent = nodeStart;
		nodeStart->fLocalGoal = 0.0f;
		nodeStart->fGlobalGoal = heuristic(nodeStart, nodeEnd);

		// Add start node to not tested list - this will ensure it gets tested.
		// As the algorithm progresses, newly discovered nodes get added to this
		// list, and will themselves be tested later
		std::list<sNode*> listNotTestedNodes;
		listNotTestedNodes.push_back(nodeStart);

		// if the not tested list contains nodes, there may be better paths
		// which have not yet been explored. However, we will also stop 
		// searching when we reach the target - there may well be better
		// paths but this one will do - it wont be the longest.
		while (!listNotTestedNodes.empty() && nodeCurrent != nodeEnd)// Find absolutely shortest path // && nodeCurrent != nodeEnd)
		{
			// Sort Untested nodes by global goal, so lowest is first
			listNotTestedNodes.sort([](const sNode* lhs, const sNode* rhs) { return lhs->fGlobalGoal < rhs->fGlobalGoal; });

			// Front of listNotTestedNodes is potentially the lowest distance node. Our
			// list may also contain nodes that have been visited, so ditch these...
			while (!listNotTestedNodes.empty() && listNotTestedNodes.front()->bVisited)
				listNotTestedNodes.pop_front();

			// ...or abort because there are no valid nodes left to test
			if (listNotTestedNodes.empty())
				break;

			nodeCurrent = listNotTestedNodes.front();
			nodeCurrent->bVisited = true; // We only explore a node once


			// Check each of this node's neighbours...
    			for (auto nodeNeighbour : nodeCurrent->vecNeighbours)
			{
				// ... and only if the neighbour is not visited and is 
				// not an obstacle, add it to NotTested List
				if (!nodeNeighbour->bVisited && nodeNeighbour->bObstacle == 0)
					listNotTestedNodes.push_back(nodeNeighbour);

				// Calculate the neighbours potential lowest parent distance
				float fPossiblyLowerGoal = nodeCurrent->fLocalGoal + distance(nodeCurrent, nodeNeighbour);

				// If choosing to path through this node is a lower distance than what ################
				// the neighbour currently has set, update the neighbour to use this node################
				// as the path source, and set its distance scores as necessary###########################~
				//#######
				//######
				if (fPossiblyLowerGoal < nodeNeighbour->fLocalGoal)
				{
					nodeNeighbour->parent = nodeCurrent;
					nodeNeighbour->fLocalGoal = fPossiblyLowerGoal;

					// The best path length to the neighbour being tested has changed, so
					// update the neighbour's score. The heuristic is used to globally bias
					// the path algorithm, so it knows if its getting better or worse. At some
					// point the algo will realise this path is worse and abandon it, and then go
					// and search along the next best path.
					nodeNeighbour->fGlobalGoal = nodeNeighbour->fLocalGoal + heuristic(nodeNeighbour, nodeEnd);
					//path
					//	std::cout << nodeNeighbour->parent->x << ", " << nodeNeighbour->parent->y << std::endl;
						
					
				}
			}
		
		}
		
		return true;
	}
	//std::vector<paths> aStar::pathGangShit(std::vector<paths> path)
	//{
	//	paths steps;
	//	for (auto& step : path)
	//	{
	//		isFacing(step, path);

	//			steps.x = path[0].x + 1 / 10.0;
	//		steps.y = path[0].y;
	//		this->path.insert(path.begin() + 1, steps);
	//		steps.x = path[0].x + 2 / 10.0;
	//		steps.y = path[0].y;
	//		this->path.insert(path.begin() + 2, steps);
	//		steps.x = path[0].x + 3 / 10.0;
	//		steps.y = path[0].y;
	//		this->path.insert(path.begin() + 3, steps);
	//		steps.x = path[0].x + 4 / 10.0;
	//		steps.y = path[0].y;
	//		this->path.insert(path.begin() + 4, steps);
	//		steps.x = path[0].x + 5 / 10.0;
	//		steps.y = path[0].y;
	//		this->path.insert(path.begin() + 5, steps);
	//		steps.x = path[0].x + 6 / 10.0;
	//		steps.y = path[0].y;
	//		this->path.insert(path.begin() + 6, steps);
	//		steps.x = path[0].x + 7 / 10.0;
	//		steps.y = path[0].y;
	//		this->path.insert(path.begin() + 7, steps);
	//		steps.x = path[0].x + 8 / 10.0;
	//		steps.y = path[0].y;
	//		this->path.insert(path.begin() + 8, steps);
	//		steps.x = path[0].x + 9 / 10.0;
	//		steps.y = path[0].y;
	//		this->path.insert(path.begin() + 9, steps);


	//	}
	//	
	//	return path;
	//}
	//paths aStar::isFacing(paths step)
	//{
	//	
	//		currentSteps.x = step.x;
	//		currentSteps.y = step.y;
	//	//	prevSteps.x = path[currentCount - 1].x;
	//		//prevSteps.y = path[currentCount - 1].y;
	//	
	//	
	//	return step;
	//}
	//;




	std::vector<aStar::paths> aStar::OnUserUpdate(float fElapsedTime)
	{
		float nNodeSize = 100;
		int nNodeBorder = 0;
		path.clear();
		//Integer Division to get cursor position in node space



		////	Fill(0, 0, ScreenWidth(), ScreenHeight(), L' ');
		//
		//	for (int x = 0; x < nMapWidth; x++)
		//		for (int y = 0; y < nMapHeight; y++) {
		//			{
		//				for (auto n : nodes[y * nMapWidth + x].vecNeighbours)
		//				{
		//					DrawLine(x * nNodeSize + nNodeSize / 2, y * nNodeSize + nNodeSize / 2,
		//						n->x * nNodeSize + nNodeSize / 2, n->y * nNodeSize + nNodeSize / 2, PIXEL_SOLID, FG_BLUE);

		//				}


		//			}
		//		}
		//Draw nodes on top

					//windowdraw(rectangle);


				/*Fill(x * nNodeSize + nNodeBorder, y * nNodeSize + nNodeBorder,	(x + 1) * nNodeSize - nNodeBorder, (y + 1) * nNodeSize - nNodeBorder,
				PIXEL_HALF, nodes[y * nMapWidth + x].bObstacle ? FG_WHITE : FG_BLUE);

				if (nodes[y * nMapWidth + x].bVisited)
					Fill(x * nNodeSize + nNodeBorder, y * nNodeSize + nNodeBorder, (x + 1) * nNodeSize - nNodeBorder, (y + 1) * nNodeSize - nNodeBorder, PIXEL_SOLID, FG_BLUE);

				if (&nodes[y * nMapWidth + x] == nodeStart)
					Fill(x * nNodeSize + nNodeBorder, y * nNodeSize + nNodeBorder, (x + 1) * nNodeSize - nNodeBorder, (y + 1) * nNodeSize - nNodeBorder, PIXEL_SOLID, FG_GREEN);

				if (&nodes[y * nMapWidth + x] == nodeEnd)
					Fill(x * nNodeSize + nNodeBorder, y * nNodeSize + nNodeBorder, (x + 1) * nNodeSize - nNodeBorder, (y + 1) * nNodeSize - nNodeBorder, PIXEL_SOLID, FG_RED);*/

		if (fElapsedTime == 0.1f)
		{
		
		if (nodeEnd != nullptr)
		{
			sNode* p = nodes;
			while (p->parent != nullptr)
			{
				paths coords;
				p = p->parent;
				//std::cout << p->x << ", " << p->y << "\n"; 
				coords.x = static_cast<float>(p->x);
				coords.y = static_cast<float>(p->y);
				path.push_back(coords);

			}
		}
		return path;
	}
			if (nodeEnd != nullptr)
			{
				sNode* p = nodeEnd; 
				while (p->parent != nullptr) 
				{
					paths coords;
					p = p->parent;
					//std::cout << p->x << ", " << p->y << "\n"; 
					coords.x = static_cast<float>(p->x);
					coords.y= static_cast<float>(p->y);
					path.push_back(coords);
					
				} 
			}
		
		return path;
	};



//Implementation.//
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