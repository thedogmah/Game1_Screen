#include "Player.h"

#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <SFML/Audio.hpp>
#include <SFML/Network.hpp>


	Player::Player() {
		this->actor.setSize(sf::Vector2f(97.0f, 200.0f));
	//	actor.setScale(0.21, 0.21);
		
		actor.setPosition(3180, 3980);
		vibeInstinct.setOrigin(0.0f, 0.0f);
		vibeInstinct.setRadius(230.0f);
		
		vibeInstinct.setOutlineColor(sf::Color::Blue);
		vibeInstinct.setOutlineThickness(5.0f);
		vibeInstinct.setFillColor(sf::Color::Transparent);
	}

	void Player::Update() {
	
		vibeInstinct.setPosition(this->actor.getPosition().x - ((this->actor.getSize().y/2)+85), this->actor.getPosition().y-60);
	}

