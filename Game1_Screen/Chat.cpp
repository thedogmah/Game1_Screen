#include "Chat.h"
#include <iostream>
#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
Chat::Chat() {
	playerText.setPosition(8460, 800);
	playerText.setFillColor(sf::Color::Red);
	chatFont.loadFromFile("Arial.ttf");
	playerText.setFont(chatFont);
	playerText.setCharacterSize(16);
	std::cout << "Chat initialising";
}