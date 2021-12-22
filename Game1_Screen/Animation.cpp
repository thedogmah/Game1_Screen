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
	
	uvRect.width = texture->getSize().x / float(imageCount.x);
	uvRect.height = texture->getSize().y / float(imageCount.y);
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
//Caution. This if statement sets initial parameters only for NPCS
	if (this->path.size() > 0)
	{
		faceRight = true;
	} //}

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
	row = 0;
	actor.setSize(sf::Vector2f(100, 100));

	actor.setTextureRect(uvRect);
	if (this->path.size() > 0) {
		
		pathCount = path.size();
		if(currentCount<=pathCount)
			//calculate direction based on previous. (4 if statements will do it.

		actor.setTextureRect(uvRect);
		actor.setPosition((this->path[this->currentCount].x *100), (this->path[this->currentCount].y *100));
		direction.x = path[pathCount+1].x;
		direction.y = path[pathCount+1].y;
		if (currentCount < pathCount)
			currentCount++;
		else
			currentCount = 0;
	}
}
Animation::~Animation()
{

}