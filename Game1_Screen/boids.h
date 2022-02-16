#pragma once
#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <SFML/Audio.hpp>
#include <SFML/Network.hpp>
#include <vector>

class Boids
{
public:
	Boids();
	sf::Vector2f position;
	sf::Vector2f velocity;
	sf::Vector2f acceleration;
	float maxForce = 0.1;
	bool setMag(sf::Vector2f);
	std::vector<Boids> boids;
	sf::Vector2f align(std::vector<Boids> boids);

	bool dist(float x1, float y1, float x2, float y2);
};

