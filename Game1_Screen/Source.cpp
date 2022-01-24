#include "Game.h"
#include <iostream>
#include "clientside.h"
#include "Player.h"
#include "aStar.h"
#include "imgui/imgui.h"
#include "imgui/imgui-SFML.h"

using namespace sf;

//void multi();
int main()
{
	

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
	//game.humanity.aStarPath.OnUserCreate();
	//game.humanity.aStarPath.solve_AStar();
	float deltaTime = 0.0f;
	
	while (game.running())
	{
		
	
		deltaTime = clock.restart().asSeconds();
		game.routefind.OnUserUpdate(0.05f);
		
		game.update();
	
		
		character.Update(0, deltaTime, game.faceRight, game.faceDown, game.faceUp, game.still);
		//game.humanity.drawPeople(game.dayTime, game.uTime, game.npcDelta);
		//game.dogGR.drawPeople(game.dayTime, game.uTime, game.npcDelta);
	
		game.player.actor.setTextureRect(character.uvRect);
		
	
		game.render();

		
	}
	ImGui::SFML::Shutdown();
	
}