#pragma once
#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <SFML/Audio.hpp>
#include <SFML/Network.hpp>
#include <vector>
#include <iostream>
#include <ctime>
#include "clientside.h"
#include "Animation.h"
#include "Player.h"
#include "Chat.h"
#include "aStar.h"
#include "Population.h"
#include <thread>
#include "PopulationDog.h"
#include "PopulationDrone.h"
#include "PopulationScooter.h"
#include <fstream>
#include "quadtree.h"
//#include <sstream>
//Class as the game engine.

class Game
{
public:
	//packet movement string

	aStar routefind;
	aStar setObstacles;
	std::vector<std::vector<char>> vPathCollide; 
	std::vector<char> vPathInts;

	struct gsNode {

		bool bObstacle = false;
		bool bVisited = false;
		float fGlobalGoal;		//Distance to goal so far
		float fLocalGoal;		//Distance to goal if we took alternative
		int x;
		int y;
		std::vector<gsNode*> vecNeighbours;
		gsNode* parent;

	};


	std::string movement;
	Player player;
	Player playerTwo;
	bool grid;			//bool to switch / flag node grid on or off (see below)
	bool gridPath;		//bool to switch / flag a* drawing on or off
	bool faceRight;
	bool faceDown;
	bool faceUp;
	bool still;
	sf::Packet rpacket;
	sf::TcpSocket rsocket;
	std::string username;

	sf::Shader water;
	sf::Shader sFountain;
	//animatedCharacter
	
	//player.setPosition(500.0f, 300.0f);
	sf::Texture playerTexture;
	sf::Image iplayerTexture;
	
	//player2AnimatedChar
	sf::Texture playerTwoTexture;
	sf::Image iplayerTwoTexture;

	bool npcMove = false;
	//NPCS
	sf::Clock dogClock;
	sf::Clock npcClock;
	std::vector<sf::RectangleShape> npcs;
	std::vector<sf::RectangleShape> npcsPath;
	sf::Sprite spChar;
	sf::Image image;
	sf::Sprite sprNpc1;
	sf::Texture texNpc1;
	sf::Image npc1;
	Animation npc;
	std::thread collissionThread;
	Population humanity;
	Population humanityMaleGreen;
	Population humanityMaleSandyJacket;
	Population humanityMaleWhiteJacket;
	PopulationDog dogGR;
	PopulationDrone drones;
	PopulationScooter scooters;
	PopulationScooter scootersManSandyJacket;
	float npcDelta;
	float npcDeltaSwitch = 0.077f;
	float npcTimeHold;
	float dogTimeHold;
	float droneTimeHold;
	float scooterTimeHold;
	float uTime;
	float dayTime = 1.0;
	bool bRain = false;
	bool bScooters = true;
	bool bDrones = false;
	bool bDogs = false;
	bool bHumans = true;
	bool resetPath = false;
	bool bPathKey = false;
	bool bQuadTree = false;
	sf::Clock clockImGui;

	//player statistics
	int experience = 0;
	std::string exper="";
private:

	//Variables	
	
	//UI
	
	//Window
	sf::Vector2i screenSize ;
	sf::RenderWindow* window;
	
	sf::VideoMode videoMode;
	sf::Event ev;
	sf::Clock timeShader;
	sf::Clock npcDeltaClock;
	sf::Clock shaderClock;
	float fShaderClock;
	//Camera
	sf::View view;
	float vZoom = 0.8;
	float moveSpeed = 1000.0;
	sf::Vector2f aPosition;
	float zoomfactor = 1;
	//Mouse Positions

	
	sf::Vector2i mousePosWindow;
	sf::Vector2f mousePosView; 


	//Resources
		//file stream for reading pathfinding file
	
	//Pathfinding / Collissions
	std::ifstream pathdata;
	std::fstream pathDataW;

	Rectangle boundary; //is the boundary (sub part) of the quadtree
	QuadTree qt; //will act as the parent node / base of tree / father / root node.
	// Interface
	sf::Font fontUI;


	//Network
	
	//Chat instance
	Chat chat;
	


	//boundaries
	sf::RectangleShape Rec;
	std::vector<sf::RectangleShape> vPathVisualAid;
	sf::CircleShape circle; //mouse hover signifier
	int circleID;
	//Sprites
	sf::Texture texFountain;
	sf::Sprite sprFountain;
	sf::Texture noise;

	//Actors
	sf::Texture actorTexture;
	sf::Sprite actorMain;
	sf::Texture bg;
	sf::Texture Char;
	sf::Texture Char2;
	sf::Texture Char3;
	sf::Texture Char4;
	sf::Sprite CharBG;
	

	


	//Weather
	int randomh, randomr, randomg, randomb, randomx, randomsp[1500];
	int part = 1;

	sf::RectangleShape rectangle[1000];

	
	//Text
	sf::Text userText;
	sf::Text uiText;
	sf::Text textTwo;
	//Game logic
	unsigned points;
	float enemySpawnTimer;
	float enemySpawnTimerMax;
	int maxEnemies;
	int health;
	bool mouseHeld;
	bool endGame;
	//Game Objects
	sf::RectangleShape enemy;
	std::vector<sf::RectangleShape> enemies;
	std::vector<sf::Text> ui;
	std::vector<sf::Text> conversations;
	//Private functions
	
	void initInterface();
	void initVariables();
	void initRain();
	void initWindow();
	void initEnemies();
	void initSprites();
	void initConnection();
	void initNPC();
	std::string npcMarried(Animation person);
	std::string npcSexuality(Animation person);
	std::string npcCareer(Animation person);
	std::string npcVibe(Animation person);
	std::string npcReligion(Animation person);
	void checkCollide();
public:
	void initClient(sf::TcpSocket* rsocket);
	
	//Constructors / Destructors
	Game();
	virtual ~Game();
	clientside client; //initalise clientside object to create send packet and functions. see clientside.h
	//Accessors
	
	const bool running() const;


	//Functions
	void spawnEnemy();
	void login();
	
	void pollEvents();

	void updateEnemies();
	void update();
	
	void renderRain();
	void renderEnemies();
	void renderPlayers();
	void renderInterface(sf::RenderTarget &target);
	void render();
	void updateMousePositions();

	void pathUpdate(int x,int y);
	void pathReset();
	void pathSave();
};

