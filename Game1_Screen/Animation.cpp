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
	//Constructor differs in that it takes a path, but not opted to use it myself so far.
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
//	std::cout << "\n\nStart of animate npc funcurrenction, still is: " << still;
//	std::cout << "\n\nStart of animate npc funcurrenction, still is: " << still;
	npcWalkSwitch = 0.001;
	//npc
	
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
			if (lerpCount !=0){
			currentSteps.x = path[currentCount].x;
			currentSteps.y = path[currentCount].y;
			this->prevSteps.x = this->path[this->currentCount - 1].x;
			this->prevSteps.y = this->path[this->currentCount - 1].y;
		//	if (getPathCall == true)

//			{
				getPath();
////getPathCount++;
//				getPathCoubt
//			}
//			else
			}
//				getPathCount++;
//				if (getPathCount == 9)
//					getPathCall = tr
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
		totalTime +=deltaTime;
		//npcWalkSwitch = 0.02;
		if (totalTime >switchTime  )
		{
			totalTime =0;
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
	//	if (currentCount < path.size())
		//{
			//if (getPathCall == false)
			//	npcWalkSwitch = 0.2;
			//else {
			//	//npcWalkSwitch = 0.25;
			//	npcWalkSwitch = 0.2;
			//}
			
			
				
				
				//	actor.setTextureRect(uvRect);
			//	actor.setPosition((this->path[this->currentCount].x * 90.0f - count), (this->path[this->currentCount].y * 90.0f - count));
				if (lerpCount >9)
				{
				//	npcWalkSpeed = 0;
					actor.setPosition((this->path[this->currentCount].x * 100.0f), (this->path[this->currentCount].y * 100.0f));
					currentCount++;
					lerpCount = 0;
				}
			
			//	direction.x = static_cast<float>(path[pathCount].x);
				//direction.y = static_cast<float>(path[pathCount].y);
		//		std::cout << "\nNew position set";
				
				//if (count ==9)
				
				if (currentCount == (path.size()))
				{
					//currentCount = currentCount;
					//row = 3;
					//currentCount++;
				}
				//else
				//currentCount++;//current count will iterate to 1 within the GetPath function, breaking this deadlock // 
			}
			//npcWalkSwitch -= 0.47;
	//	}
	
		
		
	
	//std::cout << "\n\nEnd of animate npc function, still is: " << still; 
}
void Animation::UpdateNpc(int row, float deltaTime)
{

	npcWalkSpeed += deltaTime;
	npcTotalTime += deltaTime;

	//If check passes, then this is npc or some enemy and not a player character, therefore, control is taken over by the class to animate and move.
	if (this->path.size() > 0) {
		
		if (currentCount != 0 && currentCount <= path.size())
		{
			if (lerpCount != 0) {
				currentSteps.x = path[currentCount].x;
				currentSteps.y = path[currentCount].y;
				this->prevSteps.x = this->path[this->currentCount - 1].x;
				this->prevSteps.y = this->path[this->currentCount - 1].y;
			
				getPath();
			
			}
		

		}

	
			if (eFacing == South)
				row = 1;
			else
				if (eFacing == North)
					row = 2;


		//After setting correctd row and taking time from loop, image is checked to move to next animation slide (For walking animation etc)
		currentImage.y = row;
		totalTime += deltaTime;
		
		if (totalTime > switchTime)
		{
			totalTime -= switchTime;
			currentImage.x++;
			if (currentImage.x >= imageCount.x)
			{
				currentImage.x = 0;
			}

		}


		//flips image dependant on the way it is facing.
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




		if (lerpCount > 99 && currentCount <= path.size())
		{
			//	npcWalkSpeed = 0;
			actor.setPosition((this->path[this->currentCount].x * 100.0f), (this->path[this->currentCount].y * 100.0f));
			currentCount++;
			lerpCount = 0;
			
		}
		
	}


}
	


sf::Vector2i Animation::getLocality()
{

	//used for checking bounds on customer character and returning as vector 2i
	sf::Vector2i locality;
	int x, y;
	x = static_cast<int>(this->actor.getPosition().x);
	y = static_cast<int>(this->actor.getPosition().y);
	//std::cout << "\n original float position before locality: " << float(actor.getPosition().x) << ", y: " << float(actor.getPosition().y);
	locality.x = x;
	locality.y = y;
	return locality;


}

void Animation::UpdateTransport(int row, float deltaTime)
{

	npcWalkSpeed += deltaTime;
	npcTotalTime += deltaTime;

	//If check passes, then this is npc or some enemy and not a player character, therefore, control is taken over by the class to animate and move.
	if (this->path.size() > 0) {

		if (currentCount != 0 && currentCount <= path.size())
		{
			if (lerpCount != 0) {
				currentSteps.x = path[currentCount].x;
				currentSteps.y = path[currentCount].y;
				this->prevSteps.x = this->path[this->currentCount - 1].x;
				this->prevSteps.y = this->path[this->currentCount - 1].y;

				getPath();

			}


		}


		if (eFacing == South)
			row = 1;
		else
			if (eFacing == North)
				row = 2;


		//After setting correctd row and taking time from loop, image is checked to move to next animation slide (For walking animation etc)
		currentImage.y = row;
		totalTime += deltaTime;

		if (totalTime > switchTime)
		{
			totalTime -= switchTime;
			currentImage.x++;
			if (currentImage.x >= imageCount.x)
			{
				currentImage.x = 0;
			}

		}


		//flips image dependant on the way it is facing.
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




		if (lerpCount > 33 && currentCount <= path.size())
		{
			//	npcWalkSpeed = 0;
			actor.setPosition((this->path[this->currentCount].x * 100.0f), (this->path[this->currentCount].y * 100.0f));
			currentCount++;
			lerpCount = 0;

		}

	}


}






void Animation::getPath()
{
	
			if (this->prevSteps.x < this->currentSteps.x)
			{
				eFacing = East;
		//		std::cout << "\nFacing East from" << prevSteps.x << ", " << prevSteps.y << " to: " << currentSteps.x << ", " << currentSteps.y << std::endl;
				return;
				getPathCall = false;
				for (int i = 1; i < 2; i++) {
					steps.x = path[0].x + 1/10.0;
					steps.y = path[0].y;
					this->path.insert(path.begin() + i, steps);
					steps.x = path[0].x + 2 / 10.0;
					steps.y = path[0].y;
					this->path.insert(path.begin() + 2, steps);
					steps.x = path[0].x + 3 / 10.0;
					steps.y = path[0].y;
					this->path.insert(path.begin() + 3, steps);
					steps.x = path[0].x + 4 / 10.0;
					steps.y = path[0].y;
					this->path.insert(path.begin() + 4, steps);
					steps.x = path[0].x + 5 / 10.0;
					steps.y = path[0].y;
					this->path.insert(path.begin() + 5, steps);
					steps.x = path[0].x + 6 / 10.0;
					steps.y = path[0].y;
					this->path.insert(path.begin() + 6, steps);
					steps.x = path[0].x + 7 / 10.0;
					steps.y = path[0].y;
					this->path.insert(path.begin() + 7, steps);
					steps.x = path[0].x + 8 / 10.0;
					steps.y = path[0].y;
					this->path.insert(path.begin() + 8, steps);
					steps.x = path[0].x + 9 / 10.0;
					steps.y = path[0].y;
					this->path.insert(path.begin() + 9, steps);
					//std::cout << i;
					//npcWalkSwitch *= 10;

					
				}
			}
			if (this->prevSteps.x > this->currentSteps.x)
			{
				//		std::cout << "\nFacing West from" << prevSteps.x << ", " << prevSteps.y << " to: " << currentSteps.x << ", " << currentSteps.y << std::endl;
				eFacing = West;
				return;
				getPathCall = false;
				for (int i = 1; i < 2; i++) {
					steps.x = path[0].x - 1 / 10.0;
					steps.y = path[0].y;
					this->path.insert(path.begin() + i, steps);
					steps.x = path[0].x - 2 / 10.0;
					steps.y = path[0].y;
					this->path.insert(path.begin() + 2, steps);
					steps.x = path[0].x - 3 / 10.0;
					steps.y = path[0].y;
					this->path.insert(path.begin() + 3, steps);
					steps.x = path[0].x - 4 / 10.0;
					steps.y = path[0].y;
					this->path.insert(path.begin() + 4, steps);
					steps.x = path[0].x - 5 / 10.0;
					steps.y = path[0].y;
					this->path.insert(path.begin() + 5, steps);
					steps.x = path[0].x - 6 / 10.0;
					steps.y = path[0].y;
					this->path.insert(path.begin() + 6, steps);
					steps.x = path[0].x - 7 / 10.0;
					steps.y = path[0].y;
					this->path.insert(path.begin() + 7, steps);
					steps.x = path[0].x - 8 / 10.0;
					steps.y = path[0].y;
					this->path.insert(path.begin() + 8, steps);
					steps.x = path[0].x - 9 / 10.0;
					steps.y = path[0].y;
					this->path.insert(path.begin() + 9, steps);
				}
			}
			if (this->prevSteps.y > this->currentSteps.y)
			{
		//		std::cout << "\nFacing North from" << prevSteps.x << ", " << prevSteps.y << " to: " << currentSteps.x << ", " << currentSteps.y << std::endl;
				eFacing = North;
				return;

				getPathCall = false;
				for (int i = 1; i < 2; i++) {
					steps.x = path[0].y - 1 / 10.0;
					steps.y = path[0].x;
					this->path.insert(path.begin() + i, steps);
					steps.x = path[0].y - 2/ 10.0;
					steps.y = path[0].x;
					this->path.insert(path.begin() + 2, steps);
					steps.x = path[0].y - 3/ 10.0;
					steps.y = path[0].x;
					this->path.insert(path.begin() + 3, steps);
					steps.x = path[0].y - 4/ 10.0;
					steps.y = path[0].x;
					this->path.insert(path.begin() + 4, steps);
					steps.x = path[0].y - 5 / 10.0;
					steps.y = path[0].x;
					this->path.insert(path.begin() + 5, steps);
					steps.x = path[0].y - 6 / 10.0;
					steps.y = path[0].x;
					this->path.insert(path.begin() + 6, steps);
					steps.x = path[0].y - 7 / 10.0;
					steps.y = path[0].x;
					this->path.insert(path.begin() + 7, steps);
					steps.x = path[0].y - 8 / 10.0;
					steps.y = path[0].x;
					this->path.insert(path.begin() + 8, steps);
					steps.x = path[0].y - 9 / 10.0;
					steps.y = path[0].x;
					this->path.insert(path.begin() + 9, steps);
					//std::cout << i;
					//npcWalkSwitch *= 10;

				}
			}
			if (this->prevSteps.y < this->currentSteps.y)
			{
				eFacing = South;
				return;
				getPathCall = false;
				for (int i = 1; i < 2; i++) {
					steps.x = path[0].y + 1 / 10.0;
					steps.y = path[0].x;
					this->path.insert(path.begin() + i, steps);
					steps.x = path[0].y + 2 / 10.0;
					steps.y = path[0].x;
					this->path.insert(path.begin() + 2, steps);
					steps.x = path[0].y + 3 / 10.0;
					steps.y = path[0].x;
					this->path.insert(path.begin() + 3, steps);
					steps.x = path[0].y + 4 / 10.0;
					steps.y = path[0].x;
					this->path.insert(path.begin() + 4, steps);
					steps.x = path[0].y + 5 / 10.0;
					steps.y = path[0].x;
					this->path.insert(path.begin() + 5, steps);
					steps.x = path[0].y + 6 / 10.0;
					steps.y = path[0].x;
					this->path.insert(path.begin() + 6, steps);
					steps.x = path[0].y + 7 / 10.0;
					steps.y = path[0].x;
					this->path.insert(path.begin() + 7, steps);
					steps.x = path[0].y + 8 / 10.0;
					steps.y = path[0].x;
					this->path.insert(path.begin() + 8, steps);
					steps.x = path[0].y + 9 / 10.0;
					steps.y = path[0].x;
					this->path.insert(path.begin() + 9, steps);
					//std::cout << i;
					//npcWalkSwitch *= 10;

				}
			//	std::cout << "\nFacing South from" << prevSteps.x << ", " << prevSteps.y << " to: " << currentSteps.x << ", " << currentSteps.y << std::endl;
			}
		
		
		
}
Animation::~Animation()
{

}
