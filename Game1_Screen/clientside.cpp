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
#include "socialEngine.h"

sf::Packet& operator << (sf::Packet& packet, sf::Vector2i& location)

{	
	return packet << location.x << location.y;
}


sf::Packet& operator >> (sf::Packet& packet, sf::Vector2i& location)

{
	
	return packet >> location.x << location.y;
}

sf::Packet& operator << (sf::Packet& packet, sf::Vector2f& location)

{
	return packet << location.x << location.y;
}


sf::Packet& operator >> (sf::Packet& packet, sf::Vector2f& location)

{

	return packet >> location.x << location.y;
}



sf::Text message;
clientside::clientside()
{
	sPlayers.setPosition(4637, 3000);
	playerData.avatar = sPlayers;
	playerData.position.x = 4636;
	playerData.position.y = 3000;
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
	while (socket->receive(packet) == sf::Socket::Done)
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
			packet >> location >> username >> message >> ip >> port;
			//std::cout << "size of packet: " << packet.getDataSize();
		//	std::cout << "\ndata:"<< data << "user: "<< username << "locations: "<< location << "after cast: " << static_cast<int>(location) << ", message: " << message << ip << port;
			//	std::cout << "From server data: " << data << " now at: " << location.x << ", " << location.y << std::endl;
				//DEBUG COMMENTS std::cout << "Printing message from clientside function: " << message ;
			iPlayers.loadFromFile("RyanChar2.png");
			iPlayers.createMaskFromColor(sf::Color::Black);
			tPlayers.loadFromImage(iPlayers);
			sPlayers.setTexture(tPlayers);
			//playerData.avatar.setPosition(playerData.position);
			if (location & Right)
			{
				movement.x += 1.1;
			}

			if (location & Up)
			{
				movement.y -= 1.1;
			}

			if (location & Down)
			{
				movement.y += 1.1;
			}

			if (location & Left)
			{
				movement.x -= 1.1;
			}

			//	std::cout << "\nmovement vector: " << movement.x << ", " << movement.y;
				//sPlayers.move(float(location.x), float(location.y));
					//vPlayers.push_back(sPlayers);

			playerData.direction.x = movement.x;
			playerData.direction.y = movement.y;
			playerData.status = Available;
			playerData.level = 1 + rand() % 9; //random level for testing features such as not being able to itneraact with certain levels etc.
	//		std::cout << "\nBefore Insert" << playerData.position.x << ", " << playerData.position.y;
			PlayerMap.insert(std::pair<std::string, Players>(username, playerData));


			//loop through vector and
			std::map<std::string, Players>::iterator it = PlayerMap.find(username);
			if (it != PlayerMap.end()) {
				it->second.direction = movement;
				it->second.avatar.setPosition(it->second.position);
				//it->second.avatar.setPosition(it->second.position);
				//std::cout << "\after Insert" << it->second.position.x << ", " << it->second.position.y;
			}



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


		else if (header == 3) {
			sf::Vector2f positionUpdate;
			//	std::cout << "\nPositions received.";
			packet >> username >> positionUpdate.x >> positionUpdate.y;
			std::map<std::string, Players>::iterator it = PlayerMap.find(username);
			if (it != PlayerMap.end()) {

				it->second.position.x = positionUpdate.x;
				it->second.position.y = positionUpdate.y;
				//it->second.position.x = positionUpdate.x;
			//	it->second.position.y = positionUpdate.y;
			}
		}
			else if (header == 20)
			{
				std::string username;
				packet >> username;
				this->social->serverTradeRequest(username);
			}

			//header 21 for receiving hug
			else if (header == 21)
		{
		std::string username;
		packet >> username;
		this->social->serverHugReceived(username);
		std::cout << "\nReceived hug from " << username;
		}

			else if (header == 22)
		{
		std::string username;
		std::string url;
		packet >> username >> url;
		social->bReceivedMemeWindow = true;
		social->memeURL = url;
		social->memeUsername = username;
		social->receivememeFirstRun = true;
		social->bMemeReady = false;
		delete social->AniGif;
		delete social->receivedMeme;
		social->AniGif = nullptr;
		social->receivedMeme = nullptr; //make the sprite for the AniGif nullptr.

		if ((std::filesystem::exists("ReceivedMeme.gif")))
		{
			int random = 1 + rand() % (10000); //creatin a random suffix to add onto old file for the renaming call.
			std::string filename = "oldmeme" + std::to_string(random) + ".gif";
			oldMemes.push_back(filename);
			std::error_code a;
			std::filesystem::rename("ReceivedMeme.gif",filename.c_str(), a);
			std::cout << "\n" << a.message().c_str();
		}
		social->clientReceivedMeme(url, username);
		std::cout << "\nReceived meme from " << username << "url is:" << url;
		}

			packet.clear();
			//		std::this_thread::sleep_for((std::chrono::milliseconds)10);
				//}
		
	}
}

void clientside::startThreads()
{
//	std::thread receive_thread(&clientside::ReceivePackets, this, &rsocket);

	
	
}

void clientside::getIP()
{
	return;
	std::cout << "Enter Host IP:";
		std::cin >> ip;
}

