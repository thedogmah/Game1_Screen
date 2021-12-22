#pragma once
#include <SFML/Graphics.hpp>
#include "aStar.h"
//#include "Player.h"
class Animation
{
public: 
	Animation();
	Animation(sf::Texture* texture, sf::Vector2u imageCount, float switchTime);
	Animation(sf::Texture* texture, sf::Vector2u imageCount, float switchTime, std::vector<aStar::paths> path);
	~Animation();
	
//	Player player;
	sf::RectangleShape actor;	//Rectangle will display NPC image.
	sf::IntRect uvRect;
	aStar::paths steps;
	std::vector<aStar::paths> path;
	sf::Vector2u imageCount; 
	float switchTime;
	
	struct Direction {
	int x;
	int y;
	} ;

	Direction direction;
	int pathCount=0; // indicates total size of path vector
	int nextPathCount=0; //helps indicate next location in vector to help iterate
	int currentCount=0; //indicates current place in path
	aStar::paths prevSteps; // Update member function will record previous step and use this to determine a direction change.
	aStar::paths nextSteps; // 'Update' member function will anticipate next step since this will me NPC enters a new grid space which may be resourceful knowledge in future
	void Update(int row, float deltaTime, bool faceRight, bool faceDown, bool faceUp, bool still);
private:

	
	sf::Vector2u currentImage;

	float totalTime;
	


};

