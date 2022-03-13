#pragma once
#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <SFML/Audio.hpp>
#include <SFML/Network.hpp>
#include <vector>
#include <iostream>
#include <ctime>
#include <thread>
#include <map>
#include <string>
class clientside {
	struct Players;
public:
	//map for storing players
	std::map<std::string, Players>PlayerMap;
	std::map<std::string, std::string>ChatMap;

	
	enum Status : unsigned char {

		Available = 0,
		Busy = 1 << 0,
		Talking = 1 << 1,
		Transaction = 1 << 2,
		Loading = 1 << 3
	}; 

	struct Players {

		sf::Vector2f direction;
		Status status;
		short level;

	} playerData;

	//player received packet enum
	enum Direction : unsigned char {
		Stop = 0,
		Right = 1 << 0,
		Left = 1 << 1,
		Up = 1 << 2,
		Down = 1 << 3
	};
	unsigned char direction;

	//worldSync Variables
	int worldTime{}; //the world time taken from the server and passed to game.cpp on a world sync event

	//Image resources for storing online players to draw and place in map (above) - within receive packet function
	std::vector<sf::Sprite> vPlayers;
	sf::Image iPlayers;
	sf::Texture tPlayers;
	sf::Sprite sPlayers;
	
	sf::Packet sendingpacket; // this is the packet used to send 
	sf::Packet rpacket; //receving packet
	clientside(); //connects to server?
	sf::IpAddress ip = sf::IpAddress::getLocalAddress();
	sf::TcpSocket socket;
	sf::TcpSocket rsocket;

	void ReceivePackets(sf::TcpSocket*);

	//initialisers
	void startThreads();
};

sf::Packet& operator << (sf::Packet& packet, sf::Vector2i& location);
sf::Packet& operator >> (sf::Packet& packet, sf::Vector2i& location);
