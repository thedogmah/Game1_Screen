#pragma once
#include <vector>
#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
class Point {
public:
	int x;
	int y;
	Point(int x, int y);
	Point();

};

class Rectangle {
public:
		int x, y, w, h;
		Rectangle(int x, int y, int w, int h);
		Rectangle();
		bool contains(Point point);

};

class QuadTree
{
public:
	//rectangle and dot visuals for drawing
	sf::RectangleShape rec;
	sf::CircleShape dot;
	QuadTree* northeast;
	QuadTree* northwest;
	QuadTree* southeast;
	QuadTree* southwest;

	int capacity;
	bool divided = false;
	std::vector<Point> points;
	Rectangle boundary;
	QuadTree();
	QuadTree(Rectangle boundary, int capacity);
	bool insert(Point);
	void subdivide();
	void show(sf::RenderWindow &window, int depth = 0);
	void cleanseTree();
	std::vector<Point> qpoints; //points collected in query, to be returned
	std::vector<Point>  query(sf::RectangleShape &range, std::vector<Point> &points);
	char* qtqueryx; //used to query through imgui input text in game.cpp
	char* qtqueryy;
	char* qtqueryh;
	char* qtqueryw;
	Point queryPoint;
};

