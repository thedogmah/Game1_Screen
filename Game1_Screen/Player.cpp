#include "Player.h"

#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <SFML/Audio.hpp>
#include <SFML/Network.hpp>


	Player::Player() {
		this->actor.setSize(sf::Vector2f(97.0f, 200.0f));
	//	actor.setScale(0.21, 0.21);
		
		actor.setPosition(4180, 1080);
	
	}

	

