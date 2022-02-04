#pragma once

#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <SFML/Audio.hpp>
#include <SFML/Network.hpp>
#include "Animation.h"
#include "imgui/imgui.h"
#include "imgui/imgui-SFML.h"
class Player
{
public:
	struct vibes {
		int x;
		int y;
		int vibe;

	};
	
	struct profiles{

		std::string name = "Ryan";

		float space;
		float peace;
		float socialInfluence;
		float mediaInfleunce;

		//mind variables
		float anxiety = 10;
		float rest = 50.0;
		float social = 50.f;
		float powerVibeInstinct = 100.f;
		float powerVibeInstinctTolerance = 0.01f;

		//body variables
		float hunger = 50.f;
		float strength = 50.f;
		float style = 50.f;

		float mindhealth = 60.f;
		float bodyhealth = 60.f;
		float soulhealth = 60.f;
	};

	//bools for vibes
	bool bVibeInstinctSwitch = false;
	bool bvibeInstinctsDraw;

	vibes vibe;
	profiles profile;

	std::vector<vibes> vInstinctVibes;
	//Vector will collecdt the text of the vibes sensed around Player Character. Vector is reset every frame so it doesnt draw multiple.
	std::vector<sf::Text> vVibesText;
	sf::RectangleShape actor;
	Player();
	void vibeCatch(std::string);
	void Update();
	sf::CircleShape vibeInstinct;
};

