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

public:
	//map for storing players
	std::map<std::string, sf::Sprite>PlayerMap;
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
