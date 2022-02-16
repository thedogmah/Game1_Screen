#include "boids.h"

Boids::Boids() {

}

bool Boids::setMag(sf::Vector2f)
{

	//set magnitue of vector
	return false;
}

sf::Vector2f Boids::align(std::vector<Boids> boids)
{

	int perceptionRadius = 50;
	sf::Vector2f steering;
	int total{};

	for (auto& other : boids)
	{
		//float d = dist(this->position.x, this->position.y, other.x, other.y);


		if (this != &other);
	}
	return sf::Vector2f();
}

bool Boids::dist(float x1, float y1, float x2, float y2)
{
	{
		// Calculating distance
		return sqrt(pow(x2 - x1, 2) +
			pow(y2 - y1, 2) * 1.0);
	}
	return true;
}
