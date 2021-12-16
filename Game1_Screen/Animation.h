#pragma once
#include <SFML/Graphics.hpp>

class Animation
{
public: 
	Animation(sf::Texture* texture, sf::Vector2u imageCount, float switchTime);
	~Animation();
	
	sf::IntRect uvRect;

	void Update(int row, float deltaTime, bool faceRight, bool faceDown, bool faceUp, bool still);
private:

	sf::Vector2u imageCount;
	sf::Vector2u currentImage;

	float totalTime;
	float switchTime;


};

