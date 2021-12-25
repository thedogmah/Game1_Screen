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
	

	void getPath();
//	Player player;
	sf::RectangleShape actor;	//Rectangle will display NPC image.
	sf::IntRect uvRect;
	aStar::paths steps;
	std::vector<aStar::paths> path;
	sf::Vector2u imageCount; 
	sf::Vector2u currentImage;
	float switchTime;
	bool completed; // is GetPath function done filling in missing steps?
	float npcTotalTime = 0.0f; //total time given independently from delta time.
	float npcStepTime = 0.3f; //how fast to update coordinates from path to actual sprite position. (Is contrasted with delta time)
	float npcWalkSpeed = 0.0f;
	float npcWalkSwitch = 0.38f;
	enum eDirectionFacing { North, South, East, West } eFacing;
//	enum {Moving ,Still, Sitting, Running } eAction;
	

	
	aStar::paths direction;
	int pathCount=0; // indicates total size of path vector
	int nextPathCount=0; //helps indicate next location in vector to help iterate
	int currentCount=0; //indicates current place in path, and will be used with NPCs to establish previous position.
	aStar::paths prevSteps; // Update member function will record previous step and use this to determine a direction change.
	aStar::paths currentSteps; // 'Update' member function will anticipate next step since this will me NPC enters a new grid space which may be resourceful knowledge in future
	void Update(int row, float deltaTime, bool faceRight, bool faceDown, bool faceUp, bool still);
private:

	std::vector<aStar::paths>::iterator iteratorSteps;
	

	float totalTime;
	


};

