#include "Game.h"
#include <iostream>
#include "clientside.h"
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

	//game.initClient(&game.rsocket);

	while (game.running())
	{
		//update
		
		game.update();


		//render
		game.render();
	}

	
}