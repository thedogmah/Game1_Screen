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



sf::Text message;
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
		std::string message;
		sf::Vector2f movement{};
		int worldT;
		unsigned char  location;
		//int header2
		int header;
		packet >> header;//>> worldT;

		if (header == 1)
		{
			packet >> worldT;// worldTime;
			worldTime = worldT;
		}
			else if (header == 2)
				{
			//packet >> data >> location >> username >> received_message
				packet >> location >> username>> message >> ip >> port;
			std::cout << "size of packet: " << packet.getDataSize();
			std::cout << "\ndata:"<< data << "user: "<< username << "locations: "<< location << "after cast: " << static_cast<int>(location) << ", message: " << message << ip << port;
			//	std::cout << "From server data: " << data << " now at: " << location.x << ", " << location.y << std::endl;
				//DEBUG COMMENTS std::cout << "Printing message from clientside function: " << message ;
			iPlayers.loadFromFile("RyanChar2.png");
			iPlayers.createMaskFromColor(sf::Color::Black);
			tPlayers.loadFromImage(iPlayers);
			sPlayers.setTexture(tPlayers);
			
			if (location & Right)
			{
				movement.x += 1;
			}

			if (location & Up)
			{	
				movement.y -= 1;
			}

			if (location & Down)
			{
				movement.y += 1;
			}

			if (location & Left)
			{
				movement.x -= 1;
			}

			std::cout << "\nmovement vector: " << movement.x << ", " << movement.y;
			//sPlayers.move(float(location.x), float(location.y));
				//vPlayers.push_back(sPlayers);

			playerData.direction = movement;
			playerData.status = Available;
			playerData.level = 1 + rand() % 9; //random level for testing features such as not being able to itneraact with certain levels etc.
		
			PlayerMap.insert(std::pair<std::string, Players>(username, playerData));


			//loop through vector and
			std::map<std::string, Players>::iterator it = PlayerMap.find(username);
			if (it != PlayerMap.end())
				it->second = playerData;


			ChatMap.insert(std::pair<std::string, std::string>(username, message));

			std::map<std::string, std::string>::iterator it2 = ChatMap.find(username);
			if (it2 != ChatMap.end())
				it2->second = message;


			for (auto e : ChatMap)
			{
				//DEBUG COMMENTS std::cout << "\n " << e.first;
				//DEBUG COMMENTS std::cout << "\n " << e.second;
			}
			/*for (auto e : vPlayers)
			{
				if e.id
			}*/
			//if ID found, update position, if not found, push back.

		}
		packet.clear();
	}
	
//		std::this_thread::sleep_for((std::chrono::milliseconds)10);
	//}
}

void clientside::startThreads()
{
//	std::thread receive_thread(&clientside::ReceivePackets, this, &rsocket);

	
	
}

