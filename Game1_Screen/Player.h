#pragma once

#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <SFML/Audio.hpp>
#include <SFML/Network.hpp>
class Player
{
public:
	struct vibes {
		int x;
		int y;
		int vibe;

	};
	vibes vibe;
	std::vector<vibes> vInstinctVibes;
	sf::RectangleShape actor;
	Player();
	void Update();
	sf::CircleShape vibeInstinct;
};

