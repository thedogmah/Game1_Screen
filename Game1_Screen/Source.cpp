#pragma comment (lib, "legacy_stdio_definitions.lib")
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
	
	cpr::Response r = cpr::Get(cpr::Url{ "http://api.giphy.com/v1/gifs/search" },
	//cpr::Authentication{ "user", "pass" },
		cpr::Parameters{ 
			{"q", "Star Wars"},
			{"api_key", "4DAR2TmOOAwSfxiBAicf73seLkJgKYS7"},
			{"limit","20" }
		} );
	r.status_code;                  // 200
	r.header["content-type"];       // application/json; charset=utf-8
	r.text;                         // JSON text string
	j = r.text;
auto a =	json::parse(r.text);
	//std::cout << r.text;
	//file >> j;
	//std::cout << "\n\n"<< j << "\n\n";
	//std::cout << "\n\n" << j.at("Ryan") << "\n\n";
	//std::cout << "At rhys: "<< j["Rhys"] << '\n';
	//
//std::cout << a.at("data").at([0]"")

int arr = 0;
for (int i = 1; i < 2; i++) {
		std::string name = std::to_string(i) + ".gif";

		json content = a;
		json data = content["data"];
		json zeroeth = data[arr];
		json images = zeroeth["images"];
		json downsized = images["downsized"];
		json url = downsized["url"];
		std::string url_s = url.get<std::string>();
		std::ofstream of(name, std::ios::binary);
		cpr::Response rDL = cpr::Download(of, cpr::Url{ url_s });
		std::cout << "http status code = " << rDL.status_code << std::endl << std::endl;
		std::cout << url_s << "\n\n\n";
		arr++;
	}
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