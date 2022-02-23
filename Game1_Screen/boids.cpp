#include "boids.h"
#include <iostream>
#include "Game.h"
#include <random>

Boids::Boids(std::default_random_engine &random) {
	this->localengine = &random;
	this->position = (sf::Vector2f(3800 + rand() %1700, 2200 + rand() % 2000));
	this->velocity = random2D(random);
	this->velocity = setMag(velocity, 1.6, 3.5, random);
	this->acceleration = random2D(random);
	this->bird.setFillColor(sf::Color::Green);
	this->bird.setOutlineColor(sf::Color::Green);
	
	this->bird.setRadius(24);
	this->bird.setPosition(5000, 2650);
	this->bird.setOutlineThickness(10);
	
}

Boids::Boids() {};

sf::Vector2f Boids::setMag(sf::Vector2f vec, float mag1, float mag2, std::default_random_engine &generator)
{
	//std::cout << "\n Vec is originally: " << vec.x << ", " << vec.y << '\n';
	
	std::uniform_real_distribution <float> distrib (mag1, mag2);
	float num = distrib(generator);
	//set magnitue of vector
	//std::cout << "Random was: " << num;
//std::cout << "Vec is now:" << vec.x * num << ", " << vec.y * num << "\n";
	return vec * num;
}

void Boids::add(sf::Vector2f, sf::Vector2f)
{

	
}

sf::Vector2f Boids::getUnit(sf::Vector2f vec)
{

	float c;
	float newx;
	float newy;
	c = sqrt(vec.x*vec.x + vec.y*vec.y);
	//std::cout << "\nMagnitude of vector " << vec.x << ", " << vec.y << " is " << c;
	sf::Vector2f newVec(vec.x / c, vec.y / c);
	//std::cout << "\nUnit Vector of: " << vec.x << " , " << vec.y << " is: " << newVec.x << ", " <<newVec.y << "\n" ;
		return newVec;
}

sf::Vector2f Boids::random2D(std::default_random_engine&  rando)
{


	std::uniform_real_distribution <float> distrib(0, 360);
	float num = distrib(rando);

//	int newx =  rand() % (360);
	num = num * 3.1415926 / 180;

	return sf::Vector2f(std::cos(num), std::sin(num));
}

sf::Vector2f Boids::align(std::vector<Boids> &boids)
{
	int perceptionRadius = 450;
	sf::Vector2f desired; //= random2D(*this->localengine);
	
	
	int total=0;
	

	for (auto& other : boids)
	{
		float d = dist(
			this->position.x,
			this->position.y,
			other.position.x,
			other.position.y);
		//float d = dist(this->position.x, this->position.y, other.x, other.y);
		

		if (&other != this && d < perceptionRadius)
		{
			desired += other.velocity;
			total++;
		}
	//	std::cout << "hey";
		
	}
	if (total > 0)
	{
		desired.x = desired.x / total;
		desired.y = desired.y / total;
	//	std::cout << "\n" << desired.x << ", " << desired.y << " : " << "Velocity: " << this->velocity.x << ", " << this->velocity.y << "\n";
		//std::cout << "Sum: " << desired.x - this->velocity.x << ", " << desired.y - this->velocity.y << "\n";
		//setMag(desired, 1.5, 3, *this->localengine);
		desired -= this->velocity;

	}
	return desired;
}

void Boids::update()
{
	//this->velocity.x = this->position.x * 2;
	//this->velocity.y = this->position.y * 2;

	this->velocity.x += this->acceleration.x;
	this->velocity.y += this->acceleration.y;

	this->position.x += this->velocity.x;
	this->position.y += this->velocity.y;
	
	//this->align(this->boids);
	this->bird.setPosition(position.x, position.y);
	this->acceleration.x *= 0.0;
	this->acceleration.y *= 0.0;
}

void Boids::flock(std::vector<Boids>& boids)
{
	//this->acceleration = align(boids);
	
	//std::cout << this->acceleration.x << this->acceleration.y;
	//this->acceleration += alignment;
	
}

float Boids::dist(float x1, float y1, float x2, float y2)
{
	
		// Calculating distance
		return sqrt(pow(x2 - x1, 2) +
			pow(y2 - y1, 2) * 1.0);
	

}
