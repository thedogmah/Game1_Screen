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
//Class as the game engine.

class Game
{
public:
	//packet movement string
	std::string movement;
	Player player;
	Player playerTwo;
	bool faceRight;
	bool faceDown;
	bool faceUp;
	bool still;
	sf::Packet rpacket;
	sf::TcpSocket rsocket;
	std::string username;

	sf::Shader water;
	
	//animatedCharacter
	
	//player.setPosition(500.0f, 300.0f);
	sf::Texture playerTexture;
	sf::Image iplayerTexture;
	
	//player2AnimatedChar
	sf::Texture playerTwoTexture;
	sf::Image iplayerTwoTexture;

	sf::Sprite spChar;
	

private:

	//Variables	
	
	//UI
	
	//Window
	sf::Vector2i screenSize ;
	sf::RenderWindow* window;
	sf::VideoMode videoMode;
	sf::Event ev;
	
	//Camera
	sf::View view;
	float vZoom = 1.0;
	float moveSpeed = 1000.0;
	sf::Vector2f aPosition;
	float zoomfactor = 0.63;
	//Mouse Positions


	sf::Vector2i mousePosWindow;
	sf::Vector2f mousePosView; 


	//Resources
	// Interface
	sf::Font fontUI;


	//Network
	
	


	//boundaries
	sf::RectangleShape Rec;
	

	//Sprites
	

	//Actors
	sf::Texture actorTexture;
	sf::Sprite actorMain;
	sf::Texture bg;
	sf::Texture Char;
	sf::Texture Char2;
	sf::Texture Char3;
	sf::Texture Char4;
	sf::Sprite CharBG;
	


	sf::Image image;



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
	//Private functions

	void initInterface();
	void initVariables();
	void initRain();
	void initWindow();
	void initEnemies();
	void initSprites();
	void initConnection();
	
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
};

