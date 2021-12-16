#include "Animation.h"
#include <iostream>
Animation::Animation(sf::Texture* texture, sf::Vector2u imageCount, float switchTime)
{

	this->imageCount = imageCount;
	this->switchTime = switchTime;
	totalTime = 0.0f;
	currentImage.x = 0;

	uvRect.width = texture->getSize().x / float(imageCount.x);
	uvRect.height = texture->getSize().y / float(imageCount.y);
}



void Animation::Update(int row, float deltaTime, bool faceRight, bool faceDown, bool faceUp, bool still)
{

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
}
Animation::~Animation()
{

}