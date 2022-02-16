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
#include <sstream>
#include "jediEngine.h"

#include "Animation.h"
//#include <sstream>
//Class as the game engine.
class socialEngine;

class Game
{
public:
	//packet movement string
	socialEngine *socialengine;
	aStar routefind;
	aStar setObstacles;
	std::vector<std::vector<char>> vPathCollide; 
	std::vector<char> vPathInts;
	Animation* currentNpc;
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

	struct sAsset {
		
		float x = 0.;
		float y = 0.;
		int ID = 0;
		int assetID=0;
	};

	jediEngine jediengine;
	
	std::string movement;
	
	//player resources
	Player player;
	sf::Vector2f velocity;
	float movementSpeed = 2.2;
	bool moving_right = false;
	bool moving_up = false;
	bool moving_down = false;
	bool moving_left = false;
	void entityMove();
	sf::Vector2f playerLastLocation{};
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
	Population humanityWomanSnugGrey;
	Population humanityWomanSnugBlack;
	Population humanityBlackMaleJacket;
	PopulationScooter deliverooBike;
	Animation *popCircleLocator =nullptr; //pointer to individual who should be 'wearing' 'NPC selected' circle.
//npcVibeText (used in collission function)
	sf::Text VibeText;

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
	bool bDrones = true;
	bool bDogs = false;
	bool bHumans = true;
	bool resetPath = false;
	bool bPathKey = false;
	bool bAssetOneKey = false;
	bool bQuadTree = false;
	sf::Clock clockImGui;
	//zoom 
	float zoomfactor = 1; // is accessed by other classes


	//variables to offset everything (for when changing map size)
	int offSetX{};
	int offSetY{};
	//Main asset for draying rect shape across Y axis, and is used by social engine (party grid)
	std::vector<sf::RectangleShape> vRectShapeDataVector;
	std::vector<sf::CircleShape> vSocialShapeDataVector;
	//player statistics
	int experience = 0;
	std::string exper="";
	sf::RenderWindow* window;

	//mouse drag box/
	sf::RectangleShape mouseDrag;
	bool mouseDown;


	//world assets.
	sf::RectangleShape Rectmsquarewall;
	sf::Image imgmsquarewall;
	sf::Texture textsquarewall;
	//sign posts on wall
	sf::RectangleShape recwallpost;
	sf::Image iwallpost;
	sf::Texture twallpost;
	sf::Texture twallpostOrange;
	sf::Texture twallpostBlue;
	std::vector<sf::RectangleShape> lastAssets;

	
private:

	//Variables	
	
	//UI
	
	//Window
	sf::ContextSettings settings;
	
	sf::Vector2i screenSize ;
	
	
	sf::VideoMode videoMode;
	sf::Event ev;
	sf::Clock timeShader;
	sf::Clock npcDeltaClock;
	sf::Clock shaderClock;
	float fShaderClock;
	//Camera
	sf::View view;
	sf::View followView;
	sf::View flyingView;
	bool bNpcFollow = false;
	bool bDroneFollow = false;
	bool bufferDroneSwitch = false;
	Animation *vNpcFollowPointer = nullptr;
	Animation* droneFollowPointer = nullptr;
	float vZoom = 0.8	;
	float moveSpeed = 1000.0;
	sf::Vector2f aPosition;
	
	//Mouse Positions
	bool viewDrawTwice = false;
	
	sf::Vector2i mousePosWindow;
	sf::Vector2f mousePosView; 


	//Resources
		//file stream for reading pathfinding file
	
	//Pathfinding / Collissions
	std::ifstream pathdata;
	std::fstream pathDataW;

	char* textinput;
	char* textinputbuff;
	//treeAsset resources
	//std::ostream assetDataO;
	std::fstream assetDataW;
	std::string sAssetTree;
	std::fstream oss;
	std::stringstream iss;
	std::string asset;
	std::vector<sAsset> vStructAssets;
	sAsset sTreeAsset;
	sf::Image imgTreeAsset;
	sf::Texture texTreeAsset;
	sf::Sprite sprTreeAsset;
	sf::RectangleShape rectTree;
	sf::Image imgTramBus;
	sf::Texture texTramBus;
	sf::Sprite sprTramBus;
	sf::RectangleShape recTramBus;
	sf::Image imgTramBus2;
	sf::Texture texTramBus2;
	sf::Sprite sprTramBus2;
	sf::RectangleShape recTramBus2;

	sf::Image imgTrees;
	sf::Texture texTrees;
	sf::RectangleShape recTrees;
	//sf::RectangleShape rectTree;
	Rectangle boundary; //is the boundary (sub part) of the quadtree
	QuadTree qt; //will act as the parent node / base of tree / father / root node.
	std::vector<Point> pointsReturned;
	// Interface
	sf::Font fontUI;
	//big wheel
	sf::RectangleShape bigwheel;
	sf::Image imgbigwheel;
	sf::Texture texbigwheel;

	sf::RectangleShape bigwheel2;
	sf::Image imgbigwheel2;
	sf::Texture texbigwheel2;

	sf::Clock carriageTimer;
	float carriageTime;
	sf::RectangleShape bigwheel3;
	sf::RectangleShape origin;
	sf::RectangleShape rotatorbin;
	sf::RectangleShape rotatorbin2;
	sf::Image imgrotatorbin;
	sf::Texture textrotatorbin;//to test ferris wheel bins. (Keep going , this is exciting)
	float rotateTime;
	//Network
	
	//Chat instance
	Chat chat;
	


	//boundaries
	sf::RectangleShape Rec;
	std::vector<sf::RectangleShape> vPathVisualAid;
	std::vector<sf::RectangleShape> vAssetVisualAid;
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
	

	//Combat
	//Player
	sf::Texture texPSword;
	sf::Image imgPSword;
	sf::Sprite sprPSword;
	sf::RectangleShape rectPSword;
	bool playerAttack = false;
	int rotate = 0;

	//Weather
	int randomh, randomr, randomg, randomb, randomx, randomsp[1500];
	int part = 1;

	sf::RectangleShape rectangle[100];

	//ScreenEffect Resources
	sf::RectangleShape fade;
	sf::RectangleShape WelcomeTo;
	sf::Image imgWelcomeTo;
	sf::Texture textWelcometo;
	float fadeTime=0;
	sf::Clock fadeClock;
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
	bool drawBox=false; //see above (mouse held)
	bool endGame;
	bool initialised = false;
	//Game Objects
	sf::RectangleShape enemy;
	std::vector<sf::RectangleShape> enemies;
	std::vector<sf::Text> ui;
	std::vector<sf::Text> uiMessages;
	std::vector<sf::Text> conversations;
	//Private functions
	
	void initInterface();
	void initVariables();
	void initRain();
	void initWindow();
	void initEnemies();
	void initSprites();
	void initMarketSquare();
	void initConnection();
	void initNPC();
	void resetFrame();
	void playerCollide(sf::Vector2i);

	
	void npcLookingGlass(Animation npc);
	std::vector<Animation> vNPCLookingGlass;
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
	void renderAssets();
	void updateMousePositions();
	
	void AssetUpdate(int x, int y);
	void pathUpdate(int x,int y);
	void pathReset();
	void pathSave();
	void treeAssetSave();

	void interact();
};

