#pragma once
#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>

#include <vector>
#include <iostream>
class EngineVector
{

public :

	EngineVector();

	std::vector<sf::Vector2f> engineBoids;
	void add(sf::Vector2f);
};

