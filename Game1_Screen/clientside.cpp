#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <SFML/Audio.hpp>
#include <SFML/Network.hpp>
#include <vector>
#include <iostream>
#include <ctime>
#include "Game.h"
#include <iostream>
#include "clientside.h"
#include <thread>
#include <mutex>
clientside::clientside()
{

	//enum class CommandsToServer {
	//	Connect, // empty
	//	Disconnect, // empty
	//	Data // Vector2f
	//}
	startThreads();
	
		//
	//	sf::Packet packet;
	//
	//sf::IpAddress ip = sf::IpAddress::getLocalAddress();
	//sf::TcpSocket socket;

	//sf::Packet gamepacket;
	//bool connect = false;
	//while (!connect)
	//	if (socket.connect(ip, 2000) == sf::Socket::Done)
	//	{
	//		connect = true;
	//		std::cout << "Connection made\n ";
	//		std::cout << "Connected to server \n " << socket.getRemoteAddress();


	//	}
	//	else
	//		std::cout << "probing for server\n " << sf::IpAddress::LocalHost << "3000\n";

}

void clientside::ReceivePackets(sf::TcpSocket* socket)
{

	
	sf::Packet packet;
	//while (true) {
	socket->setBlocking(false);
	if (socket->receive(packet) == sf::Socket::Done)
	{
		std::string data;
		packet >> data;
		std::cout << "From server data: " << data << std::endl;
	}
	
		std::this_thread::sleep_for((std::chrono::milliseconds)10);
	//}
}

void clientside::startThreads()
{
//	std::thread receive_thread(&clientside::ReceivePackets, this, &rsocket);

	
	
}

