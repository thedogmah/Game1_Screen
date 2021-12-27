#include "Animation.h"
#include <iostream>
Animation::Animation()
{
	std::cout << "Animation created, must initialise variables in game body before use, try INIT function\n";
}
Animation::Animation(sf::Texture* texture, sf::Vector2u imageCount, float switchTime)
{

	this->imageCount = imageCount;
	this->switchTime = switchTime;
	totalTime = 0.0f;
	currentImage.x = 0;
	
	uvRect.width = texture->getSize().x / static_cast<float>(imageCount.x);
	uvRect.height = texture->getSize().y / static_cast<float>(imageCount.y);
}

Animation::Animation(sf::Texture* texture, sf::Vector2u imageCount, float switchTime, std::vector<aStar::paths> path)
{

	this->path = path;
	this->imageCount = imageCount;
	this->switchTime = switchTime;
	totalTime = 0.0f;
	currentImage.x = 0;

	uvRect.width = texture->getSize().x / float(imageCount.x);
	uvRect.height = texture->getSize().y / float(imageCount.y);

}




void Animation::Update(int row, float deltaTime, bool faceRight, bool faceDown, bool faceUp, bool still)
{
	////Caution. This if statement sets initial parameters only for NPCS
//	std::cout << "\n\nStart of animate npc function, still is: " << still;
	
	if (path.size() == 0)
	{
		//these 3 set the row (whether animation is facing up or down or not moving)
		if (still == true)
		{
			row += 3;
		}
		else
			if (faceDown == true)
				row += 1;
			else
				if (faceUp == true)
					row += 2;


		currentImage.y = row;
		totalTime += deltaTime;
		
		if (totalTime >= switchTime)
		{
			totalTime -= switchTime;
			currentImage.x++;
			if (currentImage.x >= imageCount.x)
			{
				currentImage.x = 0;
			}

		}

		uvRect.top = currentImage.y * uvRect.height;

		if (faceRight)
		{
			uvRect.left = currentImage.x * uvRect.width;
			uvRect.width = abs(uvRect.width);
		}
		else
		{
			uvRect.left = (currentImage.x + 1) * abs(uvRect.width);
			uvRect.width = -abs(uvRect.width);
		}

	}

	
	//adds time since last frape to npc total time
	npcWalkSpeed += deltaTime;
	npcTotalTime += deltaTime;
	//std::cout << "\n\nMiddle of animate npc function, still is: " << still;
	//actor.setTextureRect(uvRect);
	if (this->path.size() > 0) {
		//still = false;
	//	std::cout << "\nPath greater than 0\n";
		if (currentCount != 0 && currentCount < path.size()) //if its NOT the first step, meaning we have a previous step
		{
			currentSteps.x = path[currentCount].x;
			currentSteps.y = path[currentCount].y;
			this->prevSteps.x = this->path[this->currentCount - 1].x;
			this->prevSteps.y = this->path[this->currentCount - 1].y;
			getPath();
		//	std::cout << "\nPrevious steps set\n";
			
		}
		
		if (eFacing == South || eFacing == East || eFacing == North || eFacing == West)
		{
			still = false;
		}
		//set to still if previous step and current step / coordinates are teh same.
		if (currentCount <path.size())
		if (this->prevSteps.x == this->path[this->currentCount].x && this->prevSteps.y == this->path[this->currentCount].y)
			still = true;
		if (still == true || currentCount >= path.size())
		{
			row += 3;
		}
		else
			if (eFacing == South)
				row = 1;
			else
				if (eFacing == North)
					row = 2;


		currentImage.y = row;
		

		if (npcTotalTime >= npcStepTime)
		{
			npcTotalTime -= npcStepTime;
			currentImage.x++;
			if (currentImage.x >= imageCount.x)
			{
				currentImage.x = 0;
			}

		}

		uvRect.top = currentImage.y * uvRect.height;

		if (eFacing == East)
		{
			uvRect.left = currentImage.x * uvRect.width;
			uvRect.width = abs(uvRect.width);
		}
		else //facing West
		{
			uvRect.left = (currentImage.x + 1) * abs(uvRect.width);
			uvRect.width = -abs(uvRect.width);
		}


		pathCount = path.size();
		if (currentCount < path.size()-1)
		{
			if (npcWalkSpeed >= npcWalkSwitch)
			{
				npcWalkSpeed = 0;
			//	actor.setTextureRect(uvRect);
				actor.setPosition((this->path[this->currentCount].x * 100.0f), (this->path[this->currentCount].y * 100.0f));
			//	direction.x = static_cast<float>(path[pathCount].x);
				//direction.y = static_cast<float>(path[pathCount].y);
		//		std::cout << "\nNew position set";
				currentCount++;
				if (currentCount == (path.size()))
				{
					//currentCount = currentCount;
					//row = 3;
					//currentCount++;
				}
				//else
				//currentCount++;//current count will iterate to 1 within the GetPath function, breaking this deadlock // 
			}
		}
	
		
		
	}
	//std::cout << "\n\nEnd of animate npc function, still is: " << still; 
}
	//row = 0;


void Animation::getPath()
{
			if (this->prevSteps.x < this->currentSteps.x)
			{
				eFacing = East;
		//		std::cout << "\nFacing East from" << prevSteps.x << ", " << prevSteps.y << " to: " << currentSteps.x << ", " << currentSteps.y << std::endl;

			}
			if (this->prevSteps.x > this->currentSteps.x)
			{
		//		std::cout << "\nFacing West from" << prevSteps.x << ", " << prevSteps.y << " to: " << currentSteps.x << ", " << currentSteps.y << std::endl;
				eFacing = West;
			}
			if (this->prevSteps.y > this->currentSteps.y)
			{
		//		std::cout << "\nFacing North from" << prevSteps.x << ", " << prevSteps.y << " to: " << currentSteps.x << ", " << currentSteps.y << std::endl;
				eFacing = North;
			}
			if (this->prevSteps.y < this->currentSteps.y)
			{
				eFacing = South;
			//	std::cout << "\nFacing South from" << prevSteps.x << ", " << prevSteps.y << " to: " << currentSteps.x << ", " << currentSteps.y << std::endl;
			}
		
		
		
}
Animation::~Animation()
{

}
