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
class clientside {

public:
	sf::Packet sendingpacket; // this is the packet used to send 
	sf::Packet rpacket; //receving packet
	clientside(); //connects to server?
	sf::IpAddress ip = sf::IpAddress::getLocalAddress();
	sf::TcpSocket socket;
	sf::TcpSocket rsocket;
	
	void ReceivePackets(sf::TcpSocket *);
	
	//initialisers
	void startThreads();
};