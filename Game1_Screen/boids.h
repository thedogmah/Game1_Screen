#pragma once
#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>

#include <vector>
#include <random>

//class Game;
class Boids
{
public:
	Boids(std::default_random_engine &random, int sizemin);
	Boids();
	bool flipped = false;
	std::default_random_engine* localengine;
	//std::random_device r;
	sf::RectangleShape bird; //{ 30, 18 };
//	sf::RectangleShape birdRect;
	int variance{};
	float s = 1.;
	float c = 1.;
	float a = 1.;
	float angle;
	float alp;
	//boid textures
	//sf::Image imgboid;
	
	
	sf::Vector2f position;
	sf::Vector2f velocity;
	sf::Vector2f acceleration;
	float maxForce = 1;
	sf::Vector2f setMag(sf::Vector2f vec, float range1, float range2, std::default_random_engine& generator);
	void add(sf::Vector2f, sf::Vector2f);
	sf::Vector2f getUnit(sf::Vector2f);
	float getMag(sf::Vector2f);
	sf::Vector2f limit(sf::Vector2f, float range1);
	sf::Vector2f random2D(std::default_random_engine& rando );
	
	std::vector<Boids> boids;

	sf::Vector2f align(std::vector<Boids> &boids);
	sf::Vector2f cohesion(std::vector<Boids>& boids);
	sf::Vector2f separation(std::vector<Boids>& boids);
	void flock(std::vector<Boids>& boids);
	void update();
	void edges(float, float);
	float dist(float x1, float y1, float x2, float y2);
};

