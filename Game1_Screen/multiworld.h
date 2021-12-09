#include <vector>
#include <SFML/Network.hpp>
#pragma once

sf::TcpListener mListener;
sf::SocketSelector mSelector;
bool done = false;
std::vector<sf::TcpSocket*> clients;

void multi();


