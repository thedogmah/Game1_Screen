#include "Game.h"
#include <iostream>
#include "clientside.h"
#include "Player.h"
#include "aStar.h"
using namespace sf;

//weather variables
 //void sfmlconnect();
void multi();
int main()
{
	
	

	//sfmlconnect();
	//multi();
	

//Init srand
	std::srand(static_cast<unsigned>(time(NULL)));
	Game game;

	game.login();
	//Game Loop
	game.running();
	
	sf::Music music;
	music.openFromFile("Enigma.ogg");
	//music.play();
	sf::Clock clock;
	//game.initClient(&game.rsocket);
	Animation character(&game.playerTexture, sf::Vector2u(4, 4), 0.14f);
	
	//Animation walkFront(&game.playerTwoTexture, sf::Vector2u(4, 2), 0.15f);
	game.routefind.OnUserCreate();
	game.routefind.solve_AStar();

	float deltaTime = 0.0f;
	while (game.running())
	{
		//update
		deltaTime = clock.restart().asSeconds();
		game.routefind.OnUserUpdate(0.05f);
		
		game.update();
	
		
		character.Update(0, deltaTime, game.faceRight, game.faceDown, game.faceUp, game.still);
		
		//walkFront.Update(1, deltaTime, game.faceRight);
		
		//render
		//std::cout << game.spChar.getScale().x;
		//game.spChar.setScale(1, 1);
		//game.spChar.setTextureRect(character.uvRect);
		game.player.actor.setTextureRect(character.uvRect);
		
		//game.playerTwo.actor.setTextureRect(walkFront.uvRect);
		game.render();
		
	}

	
}