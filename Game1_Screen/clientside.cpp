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

sf::Packet& operator << (sf::Packet& packet, sf::Vector2i& location)

{	
	return packet << location.x << location.y;
}


sf::Packet& operator >> (sf::Packet& packet, sf::Vector2i& location)

{
	
	return packet >> location.x << location.y;
}

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
		std::string ip;
		std::string port;
		std::string username;
		sf::Vector2i  location;
		packet >> username >> data >> location.x >> location.y >> ip >> port;
	//	std::cout << "From server data: " << data << " now at: " << location.x << ", " << location.y << std::endl;
		
		iPlayers.loadFromFile("wo1.png");
		iPlayers.createMaskFromColor(sf::Color::Black);
		tPlayers.loadFromImage(iPlayers);
		sPlayers.setTexture(tPlayers);
		
		sPlayers.setPosition(float(location.x), float(location.y));
	//	vPlayers.push_back(sPlayers);
		PlayerMap.insert(std::pair<std::string, sf::Sprite>(username, sPlayers));
		//loop through vector and
		std::map<std::string, sf::Sprite>::iterator it = PlayerMap.find(username);
		if (it != PlayerMap.end())
			it->second = sPlayers;


		/*for (auto e : vPlayers)
		{
			if e.id
		}*/
		//if ID found, update position, if not found, push back.


		packet.clear();
	}
	
//		std::this_thread::sleep_for((std::chrono::milliseconds)10);
	//}
}

void clientside::startThreads()
{
//	std::thread receive_thread(&clientside::ReceivePackets, this, &rsocket);

	
	
}

