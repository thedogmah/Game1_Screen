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

	if (!uiFont.loadFromFile("Arial.ttf"))
		std::cout << "Font not loaded for player / combat";
	VibeText.setFont(uiFont);
	soundbuffer.loadFromFile("swordswing.wav");
	sattack.setBuffer(soundbuffer);
	//sattack.set
		
}

	void Player::attack(Animation& npc, sf::RectangleShape &weapon)
	{
	
		
			if (weapon.getGlobalBounds().intersects(sf::FloatRect(sf::Vector2f(npc.actor.getPosition().x, npc.actor.getPosition().y), sf::Vector2f(npc.actor.getSize().x, npc.actor.getSize().y))) &&npc.bodyHealth > 0)
			{
				//npc.move(15, 15);
				sattack.play();
				VibeText.setCharacterSize(13);
				VibeText.setFillColor(sf::Color(sf::Uint32(30 + rand() % (225)), sf::Uint32(30 + rand() % (225)), sf::Uint32(30 + rand() % (225))));
				VibeText.setString(sf::String(std::to_string(rand() % (10))));
				VibeText.setPosition(npc.actor.getPosition().x + rand() % (70), npc.actor.getPosition().y - 10 - rand() % (70));
				//std::cout << "Body Health: " << npc.bodyHealth;
				npc.bodyHealth= npc.bodyHealth -0.1;
				//std::cout << "Body Health: " << npc.bodyHealth;
				this->vibeCatch(VibeText.getString());
				this->vCombatText.push_back(VibeText);
				enemyHealth.setFillColor(sf::Color::Red);
				if (npc.bodyHealth > 0)
					enemyHealth.setSize(sf::Vector2f(npc.bodyHealth * 1.5, 15));
				else if (npc.bodyHealth < 1) {
					npc.stopAnimate = true;
					npc.stopMove = true;
					npc.hasControl = false;
					enemyHealth.setSize(sf::Vector2f(2, 15));
					npc.actor.setRotation(90);
					npc.actor.setScale(0.6, 0.6);
					npc.actor.move(100, 100);
					this->profile.xp = this->profile.xp + 0.2;
					
				}
				enemyHealth.setPosition(npc.actor.getPosition().x + 5, npc.actor.getPosition().y - 30);

				enemy = &enemyHealth;
			}
			return;
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

