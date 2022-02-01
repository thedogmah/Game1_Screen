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

	void Player::vibeCatch(std::string vibe)
	{
		//Output vibe for debugging
		//std::cout << vibe;
		
		
		//If no instinct left, switch off.
		if (profile.powerVibeInstinct <= 0)
		{
			
			bVibeInstinctSwitch = false;
	
		}
		if (vibe == "Angry" || vibe == "Intimidating" || vibe == "Creepster")
			profile.anxiety += 0.01;

		if (vibe == "Peaceful" || vibe == "Chill" || vibe == "Friendly") {
			profile.anxiety -= 0.01;
			profile.rest += 0.01;

			//dont allow vibes to drop below one or above 100
			if (profile.anxiety <= 0)
				profile.anxiety = 0;
			if (profile.rest <= 0)
				profile.rest = 0;


			if (profile.anxiety >= 100)
				profile.anxiety = 100;
			if (profile.rest >= 100)
				profile.rest = 100;
		}
	}

	void Player::Update() {
	
		vibeInstinct.setPosition(this->actor.getPosition().x - ((this->actor.getSize().y/2)+85), this->actor.getPosition().y-60);
		profile.mindhealth = profile.rest - profile.anxiety;
		if (profile.mindhealth >= 100) profile.mindhealth = 100;
		if (profile.mindhealth <= 0) profile.mindhealth = 0;
	

	}

