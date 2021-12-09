#include "Game.h"
#include <iostream>

using namespace sf;

//weather variables
 //void sfmlconnect();
void multi();
int main()
{
	
	sf::IpAddress ip = sf::IpAddress::getPublicAddress();
	sf::TcpSocket socket;
	
	sf::Packet gamepacket;
	bool connect = false;
	while (!connect)
		if (socket.connect(sf::IpAddress::LocalHost, 2000) == sf::Socket::Done)
		{
			connect = true;
			std::cout << "Connection\n ";
			std::cout << "Connection\n ";
			std::cout << "Connection\n ";
			std::cout << "Connection\n ";
			std::cout << "Connection\n ";
			std::cout << "Connection\n ";
			std::cout << "Connectio\n\n\n\n\nn ";
			std::cout << "Connection\n ";
			std::cout << "Connection\n\n\n\n\n\n\n ";
			std::cout << "Connection\n\n\n\n\n\n\n\n\n\n\n\n\n ";

		}
		else
			std::cout << "probing for server\n " << sf::IpAddress::LocalHost<< "3000\n";


	//sfmlconnect();
	//multi();
	

//Init srand
	std::srand(static_cast<unsigned>(time(NULL)));
	Game game;
	//Game Loop
	game.running();
	sf::Music music;
	music.openFromFile("Enigma.ogg");
	//music.play();

	while (game.running())
	{
		//update
		game.update();


		//render
		game.render();
	}

	
}