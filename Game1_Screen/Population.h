#pragma once
#include "Animation.h"
#include "aStar.h"

class Population
{

public:
	 
	Population();

	Population(int cpeopleAmount, int boundx, int boundy, sf::RenderWindow &window);

	int peopleAmount;
	struct cBounds {
		int x, y;
	} coordBounds;

	std::vector<Animation> people; //Vector for people of animation class.
	sf::Clock populationClock;		//clock for timing of people to update animation
	float populationDeltaTime;
	sf::RenderWindow* window;
	sf::Image imgHuman;
	sf::Image imgHuman2;
	sf::Sprite sprHuman;
	sf::Texture texHuman;
	Animation Human;
	sf::Shader water;
	bool populate();
	bool textureSwitch = true;
	bool drawPeople(float, float);
	aStar aStarPath;

	
private:


	
	
	
};

