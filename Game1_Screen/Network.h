#pragma once
#include <iostream>
#include <string>
#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <SFML/Audio.hpp>
#include <SFML/Network.hpp>

void sfmlconnect() ;
std::vector<sf::Sprite> Characters;
std::vector<std::string> unique_players;
std::string unique_player_name;
sf::IpAddress ip = sf::IpAddress::getPublicAddress();
sf::TcpSocket socket;
char connectionType, mode;
char buffer[2000];
std::size_t received;
std::string text = "Connected to: ";