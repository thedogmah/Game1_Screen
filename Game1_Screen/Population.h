#pragma once
#include "Animation.h"
#include "aStar.h"
#include <algorithm>
//#include "socialEngine.h"
class Population
{

public:
	//socialEngine* socialengine;
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
	//sf::Image imgHuman2;
	sf::Sprite sprHuman;
	sf::Texture texHuman;
	Animation Human;
	sf::Shader water;
	bool textureSwitch = true;
	virtual bool populate();
	void newPath(Animation& npc, sf::RectangleShape player);
	bool moved = false;
	virtual bool drawPeople(float, float, float);
	//aStar aStarPath;

	sf::FloatRect rt;
	virtual void createBounds();
	std::vector<sf::Text> vTextMessage();
std::vector<sf::Text> vecText;
	sf::Text vMessages;
private:


	
	
	
};

