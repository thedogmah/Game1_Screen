//#pragma comment (lib, "legacy_stdio_definitions.lib")
#include "Game.h"
#include <iostream>
#include "clientside.h"
#include "Player.h"
#include "aStar.h"
#include "imgui/imgui.h"
#include "imgui/imgui-SFML.h"
#include "json.hpp"
#include <fstream>
//#include <stdio.h>

#include "curl/curl.h"	
#include "cpr/cpr.h"

using namespace sf;

using json = nlohmann::json;

//void multi();
int main()
{
	std::ifstream file("users.json");
	json j;
	
	
	//std::cout << r.text;
	//file >> j;
	//std::cout << "\n\n"<< j << "\n\n";
	//std::cout << "\n\n" << j.at("Ryan") << "\n\n";
	//std::cout << "At rhys: "<< j["Rhys"] << '\n';
	//
//std::cout << a.at("data").at([0]"")


//Init srand
	std::srand(static_cast<unsigned>(time(NULL)));
	Game game;

//	game.login();
	//Game Loop
	game.running();
	
	sf::Music music;
	sf::Music vibe;
	music.openFromFile("RainThunder.ogg");
	music.setVolume(25);
	music.play();
	vibe.setVolume(10);
	music.setLoop(true);
	vibe.setLoop(true);
	vibe.openFromFile("Violin.ogg");
	vibe.play();
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