#pragma once
#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>

#include <vector>
#include <random>

class Game;
class Boids
{
public:
	Boids(std::default_random_engine &random);
	Boids();
	
	std::default_random_engine* localengine;
	//std::random_device r;
	sf::CircleShape bird{ 30,10 };
	
	sf::Vector2f position;
	sf::Vector2f velocity;
	sf::Vector2f acceleration;
	float maxForce = 0.1;
	sf::Vector2f setMag(sf::Vector2f vec, float range1, float range2, std::default_random_engine& generator);
	void add(sf::Vector2f, sf::Vector2f);
	sf::Vector2f getUnit(sf::Vector2f);
	sf::Vector2f random2D(std::default_random_engine& rando );
	
	std::vector<Boids> boids;
	sf::Vector2f align(std::vector<Boids> &boids);
	void flock(std::vector<Boids>& boids);
	void update();
	float dist(float x1, float y1, float x2, float y2);
};

