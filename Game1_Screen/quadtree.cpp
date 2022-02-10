#include "quadtree.h"
#include <iostream>

	Point::Point(int x, int y)

	{
			this->x = x;
			this->y = y;
	}
	Point::Point() {}



	Rectangle::Rectangle(int x, int y, int w, int h)
	{
			this->x = x;
			this->y = y;
			this->w = w;
			this->h = h;

			
		
	}
	
	Rectangle::Rectangle()
	{
		x = 0;
		y = 0;
		w = 0;
		h = 0;

		

	};

	bool Rectangle::contains(Point point)
	{
		return (
			point.x >= this->x - this->w &&
			point.x < this->x + this->w &&
			point.y >= this->y - this->h &&
			point.y < this->y + this->h
			);

	}

	QuadTree::QuadTree()
{
		qtqueryx = new char( ' ');
		qtqueryy = new char(' ');
		qtqueryh = new char(' ');
		qtqueryw = new char(' ');
};

	
	QuadTree::QuadTree(Rectangle boundary, int capacity)
	{
		this->boundary = boundary;
		this->capacity = capacity;
		//std::cout << this->capacity << "\n";
	};

	void QuadTree::subdivide()
	{
	
		int x = this->boundary.x;
		int y = this->boundary.y;
		int w = this->boundary.w;
		int h = this->boundary.h;

		Rectangle ne(x + w / 2, y - h / 2, w / 2, h / 2);
		this->northeast = new QuadTree(ne, this->capacity);
		Rectangle nw(x - w / 2, y - h / 2, w / 2, h / 2);
		this->northwest = new QuadTree(nw, this->capacity);
		Rectangle se(x + w / 2, y + h / 2, w / 2, h / 2);
		this->southeast = new QuadTree(se, this->capacity);
		Rectangle sw(x - w / 2, y + h / 2, w / 2, h / 2);
		this->southwest = new QuadTree(sw, this->capacity);
		//if subdivided, create 4 new Quadtrees.
		this->divided = true;

	}

	void QuadTree::show(sf::RenderWindow &window, int depth)
	{
		
		//sf::RectangleShape rec;
		
		rec.setPosition(this->boundary.x,
			this->boundary.y);
		rec.setSize(sf::Vector2f(this->boundary.w *2, this->boundary.h*2));
		rec.setFillColor(sf::Color::Transparent);
	
		rec.setOutlineColor(sf::Color::White);
		rec.setOutlineThickness(2.0);
		rec.setOrigin(rec.getSize().x/2, rec.getSize().y /2);
		window.draw(rec);


		switch (depth) {
		case 0:
			dot.setFillColor(sf::Color::Yellow);
			break;
		case 1:
			dot.setFillColor(sf::Color::Green);
			break;
		case 2:
			dot.setFillColor(sf::Color::White);
			break;
		case 3:
			dot.setFillColor(sf::Color::Magenta);
			break;
		case 4:
			dot.setFillColor(sf::Color::Cyan);
			break;
		case 5:
			dot.setFillColor(sf::Color::Black);
			break;
		case 6:
			dot.setFillColor(sf::Color::Red);
			break;
		case 7:
			dot.setFillColor(sf::Color::Blue);
			break;


		}
		for (auto& d : this->points) {
			//sf::CircleShape dot;
			
			dot.setRadius(13);
		//	dot.setFillColor(sf::Color::Red);
			
				dot.setOutlineThickness(1.0);
			//	dot.setFillColor(sf::Color::Green);
			

			dot.setPosition(d.x, d.y);
			dot.setOutlineColor(sf::Color::Blue);
			window.draw(dot);

		}

			if (this->divided) {
			
			this->northeast->show(window, depth+1);
			this->northwest->show(window, depth + 1);
			this->southeast->show(window, depth + 1);
			this->southwest->show(window, depth + 1);
			
		//	points.clear();
		}
			
	
	}
	void QuadTree::cleanseTree()
	{

		if (this->divided) {
			this->northeast->cleanseTree();
			this->northwest->cleanseTree();
			this->southeast->cleanseTree();
			this->southwest->cleanseTree();
			this->points.clear();
		}

		if (this->points.size() == 0)
		{
			delete northeast;
			delete northwest;
			delete southeast;
			delete southwest;
			this->northeast = nullptr;
			this->northwest = nullptr;
			this->southeast = nullptr;
			this->southwest = nullptr;
			this->divided = false;
			
		}
	}
	std::vector<Point> QuadTree::query(sf::RectangleShape& range, std::vector<Point> &points)
	{
		std::vector<Point> found;
		Rectangle query;//use this and create a bool to say whether we've already initialsied.
		query.x = range.getPosition().x;
		query.y = range.getPosition().y;
		query.w = range.getSize().x;
		query.h = range.getSize().y;
		//std::cout << "shape passed range width is: "<< query.w <<'\n';
		//std::cout << "range height is: " << query.h << '\n';
		//std::cout << "range x is: " << query.x << '\n';
		//std::cout << "range y is: " << query.y << '\n';

		queryPoint.x = query.x;
		queryPoint.y = 6000;
		//std::cout << "\n\nquery point x width is: " << queryPoint.x << '\n';
		//std::cout << "\n\nquery point y width is: " << queryPoint.y << '\n';
	if (!this->boundary.contains(queryPoint)) {
			return points;
		}
		else { 
			for (auto p : this->points) {
				if (query.contains(p)) {
					found.push_back(p);
				}
			}
			if (this->divided) {
				this->northwest->query(range, qpoints);
				this->northeast->query(range, qpoints);
				this->southwest->query(range, qpoints);
				this->southeast->query(range, qpoints);
			}
		}
	for (auto& point : found)
	{
		std::cout << "\n" << "found x " << point.x << ", y  " << point.y << " ";
		qpoints.push_back(point);
	}
		return qpoints;
	}
	bool QuadTree::insert(Point point)
	{
		if (!this->boundary.contains(point))
		{
			return false;
		}
		if (this->points.size() < this->capacity)
		{
			this->points.push_back(point);
			return true;
		}
		else {
			if (!this->divided) {
				this->subdivide();
			}
			if (this->northeast->insert(point)) {
				return true;
			}
			else if (this->northwest->insert(point)) {
				return true;
			}
			else if (this->southeast->insert(point)) {
				return true;
			}
			else if (this->southwest->insert(point)) {
				return true;
			}
		}


	}
