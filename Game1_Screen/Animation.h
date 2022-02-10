#pragma once
#include <SFML/Graphics.hpp>
#include "aStar.h"
#include <math.h>
//#include "Player.h"
class Animation
{
public: 
	Animation();
	Animation(sf::Texture* texture, sf::Vector2u imageCount, float switchTime);
	Animation(sf::Texture* texture, sf::Vector2u imageCount, float switchTime, std::vector<aStar::paths> path);
	~Animation();
	
	sf::Vector2i getLocality();
	void getPath();
	//getPath Variables which will tell animation update not to call the getPath function 
	bool getPathCall = true;
	int getPathCount = 0;
	int lerpCount;
//	Player player;
	bool isNPC = true;
	//	character bio

	int ID; //unique ID for each animation created, particularly for poplation class. Will link NPC with bio data / database
	int intelligence;
	int IQ;
	int sexuality;
	int married;
	int mindHealth;
	int bodyHealth;
	int soulHealth;
	int influencer;
	int career;
	int crime;
	int instinct;
	int anxiety;
	int depression;
	int wealth;
	int vibe;
	int religion;
	int spirituality;
	//population class will offset each person once randomly below bool 'moved'
	bool moved = false;
	int offsetX = 0;
	int offsetY = 0;
	int offsetStep = 0;
	bool stopAnimate = false; 
	bool stopMove = false;
	bool stopOverride = false;//have they been stopped.
	bool hasControl = true;

	//npc message
	sf::Font fon;
	sf::Text msg;
	sf::RectangleShape actor;	//Rectangle will display NPC image.
	sf::IntRect uvRect;
	aStar pathSearch;
	aStar interactedPathSearch;
	aStar::paths steps;
	aStar::paths interactedSteps;
	std::vector<aStar::paths> path;
	std::vector<aStar::paths> interactedPath;

	sf::Vector2u imageCount; 
	sf::Vector2u currentImage;
	float switchTime=0.28;
	bool completed; // is GetPath function done filling in missing steps?
	float npcTotalTime = 0.0f; //total time given independently from delta time.
	float npcStepTime = 0.2f; //how fast to update coordinates from path to actual sprite position. (Is contrasted with delta time)
	float npcWalkSpeed = 0.0f;
	float npcWalkSwitch = 0.20f;
	enum eDirectionFacing { North, South, East, West } eFacing;
//	enum {Moving ,Still, Sitting, Running } eAction;
	

	
	aStar::paths direction;
	int pathCount=0; // indicates total size of path vector
	int nextPathCount=0; //helps indicate next location in vector to help iterate
	int currentCount=0; //indicates current place in path, and will be used with NPCs to establish previous position.
	aStar::paths prevSteps; // Update member function will record previous step and use this to determine a direction change.
	aStar::paths currentSteps; // 'Update' member function will anticipate next step since this will me NPC enters a new grid space which may be resourceful knowledge in future
	void Update(int row, float deltaTime, bool faceRight, bool faceDown, bool faceUp, bool still);
	void UpdateNpc(int row, float deltaTime);
	void UpdateTransport(int row, float deltaTime);
	void StopNpc();


private:

	std::vector<aStar::paths>::iterator iteratorSteps;
	

	float totalTime;
	


};

