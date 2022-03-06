#include "boids.h"
#include <iostream>
#include <random>

Boids::Boids(std::default_random_engine &random, int sizemin) {
	//if (!imgboid.loadFromFile("cloud.png"))
	//	std::cout << "Didnt loud cloud";
	//texboid.loadFromFile("bird.png");
	//this->bird.setTexture(&texboid,true);
		this->variance = 0 + rand() % 6;
	this->localengine = &random;
	this->position = (sf::Vector2f(2800 + rand() % 1200, 2400+ rand() % 1200));
	this->velocity = random2D(random);
	this->velocity = setMag(velocity, 1.,2.4, random);
	//this->acceleration = random2D(random);
	int ran = 1 + rand() % 197;
	int ran2 = 1 + rand() % 154;
	int ran3 = 1 + rand() % 214;
	 alp  = 115 + rand() % 145;
	if (variance > 3)
	{
		this->bird.setFillColor(sf::Color(255, 255, 255,255));
	
	}
	else
	{
		
	//	this->bird.setFillColor(sf::Color(47+ran ,50+ ran2, 40+ran3,  alp));
		
	}
	//this->bird.setOutlineColor(sf::Color::Yellow);
	this->bird.setOrigin(0.5, 0.5);
	int size = sizemin + rand() % 24;
	this->bird.setSize(sf::Vector2f(size, size));
	this->bird.setPosition(5000, 2650);
	this->bird.setOutlineThickness(0);
	//this->bird.setScale(sf::Vector2f(0.75 + rand() % 1, 0.25 + rand() % 1));
	
}

Boids::Boids() {};

sf::Vector2f Boids::setMag(sf::Vector2f vec, float mag1, float mag2, std::default_random_engine &generator)
{
	//std::cout << "\n Vec is originally: " << vec.x << ", " << vec.y << '\n';
	
	std::uniform_real_distribution <float> distrib (mag1, mag2);
	float num = distrib(generator);
	//set magnitue of vector
	//std::cout << "Random was: " << num;
//std::cout << "Vec is now:" << vec.x * num << ", " << vec.y * num << "\n";
	return vec * num;
}

void Boids::add(sf::Vector2f, sf::Vector2f)
{

	
}

sf::Vector2f Boids::getUnit(sf::Vector2f vec)
{

	float c;
	float newx;
	float newy;
	c = sqrt(vec.x*vec.x + vec.y*vec.y);
	//std::cout << "\nMagnitude of vector " << vec.x << ", " << vec.y << " is " << c;
	sf::Vector2f newVec(vec.x / c, vec.y / c);
	//std::cout << "\nUnit Vector of: " << vec.x << " , " << vec.y << " is: " << newVec.x << ", " <<newVec.y << "\n" ;
		return newVec;
}

sf::Vector2f Boids::random2D(std::default_random_engine&  rando)
{


	std::uniform_real_distribution <float> distrib(0, 360);
	float num = distrib(rando);

//	int newx =  rand() % (360);
	num = num * 3.1415926 / 180; 

	return sf::Vector2f(std::cos(num), std::sin(num));
}

sf::Vector2f Boids::align(std::vector<Boids> &boids)
{
	int perceptionRadius =58;
	sf::Vector2f desired; //= random2D(*this->localengine);
	
	
	int total=0;
	

	for (auto& other : boids)
	{
		float d = dist(
			this->position.x,
			this->position.y,
			other.position.x,
			other.position.y);
		//float d = dist(this->position.x, this->position.y, other.x, other.y);
		

		if (&other != this && d < perceptionRadius)
		{
			desired += other.velocity;
			total++;
		}
	//	std::cout << "hey";
		
	}
	if (total > 0)
	{
		desired.x = desired.x / total;
		desired.y = desired.y / total;
	//	std::cout << desired.x << ", " << desired.y << " before. \n";
		desired = getUnit(desired);
		desired = setMag(desired, 3.4, 3.4, *this->localengine);
		//std::cout << desired.x << ", " << desired.y << " after. \n";
		desired -= this->velocity;
		
		if (getMag(desired) > maxForce)
		{
			//std::cout << desired.x << ", " << desired.y << "\n";
			desired = limit(desired, 0.025);
		}
	}
	//std::cout << desired.x << ", " << desired.y << "\n";
	return desired;
}

sf::Vector2f Boids::cohesion(std::vector<Boids>& boids)
{
	int perceptionRadius = 25;
	sf::Vector2f desired; //= random2D(*this->localengine);


	int total = 0;


	for (auto& other : boids)
	{
		float d = dist(
			this->position.x,
			this->position.y,
			other.position.x,
			other.position.y);
		//float d = dist(this->position.x, this->position.y, other.x, other.y);


		if (&other != this && d < perceptionRadius)
		{
			desired += other.position;
			total++;
		}
		//	std::cout << "hey";

	}
	if (total > 0)
	{
		desired.x = desired.x / total;
		desired.y = desired.y / total;
		//	std::cout << desired.x << ", " << desired.y << " before. \n";
	//	desired = getUnit(desired);
		//desired = setMag(desired, 3.8, 3.8, *this->localengine);
		//std::cout << desired.x << ", " << desired.y << " after. \n";
		desired -= this->position;
		desired -= this->velocity;

		if (getMag(desired) > maxForce)
		{
			//std::cout << desired.x << ", " << desired.y << "\n";
			desired = limit(desired, 0.020);
		}
	}
	//std::cout << desired.x << ", " << desired.y << "\n";
	return desired;
}


sf::Vector2f Boids::separation(std::vector<Boids>& boids)
{
	int perceptionRadius = 22;
	sf::Vector2f desired; //= random2D(*this->localengine);


	int total = 0;


	for (auto& other : boids)
	{
		float d = dist(
			this->position.x,
			this->position.y,
			other.position.x,
			other.position.y);
		//float d = dist(this->position.x, this->position.y, other.x, other.y);


		if (&other != this && d < perceptionRadius)
		{
			sf::Vector2f diff;
			diff = (this->position - other.position);
			diff /= d;
			desired += diff;
			total++;
		}
		//	std::cout << "hey";

	}
	if (total > 0)
	{
		desired.x = desired.x / total;
		desired.y = desired.y / total;
		//	std::cout << desired.x << ", " << desired.y << " before. \n";
	//	desired = getUnit(desired);
		//desired = setMag(desired, 3.8, 3.8, *this->localengine);
		//std::cout << desired.x << ", " << desired.y << " after. \n";
		//desired -= this->position;
		desired -= this->velocity;

		if (getMag(desired) > maxForce)
		{
			//std::cout << desired.x << ", " << desired.y << "\n";
			desired = limit(desired, 0.030);
		}
	}
	//std::cout << desired.x << ", " << desired.y << "\n";
	return desired;
}

void Boids::update()
{
	//this->velocity.x = this->position.x * 2;
	//this->velocity.y = this->position.y * 2;
	//std::cout << velocity.x << " " << velocity.y << '\n';
	this->velocity.x += acceleration.x;
	this->velocity.y += acceleration.y;

	this->position.x += this->velocity.x;
	this->position.y += this->velocity.y;
	
	//this->align(this->boids);
	
	this->bird.setPosition(position.x, position.y);
	//this->acceleration.x *= 0.0;
	//this->acceleration.y *= 0.0;
}

void Boids::flock(std::vector<Boids>& boids)
{
	sf::Vector2f sep;
	sf::Vector2f coh;
	sf::Vector2f alig;

	this->acceleration.x *= 0.0;
	this->acceleration.y *= 0.0;


	sep = separation(boids);
	coh = cohesion(boids);
	alig = align(boids);
	//sep = separation(boids);
	double tannum = velocity.y / velocity.x;
	if (velocity.x != 0 && velocity.y != 0)
	{
	 bird.setRotation(atan2(velocity.y, velocity.x) * (180.f / 3.1415926f));
	 if (bird.getRotation() > 90 && bird.getRotation() < 270)
		 flipped = true;
	 else
		 flipped = false;
	// bird.setRotation(0);
	 float angle = bird.getRotation();
	// std::cout << angle << '\n';
	// this->angle = atan2(velocity.y, velocity.x) * (180.f / 3.1415926f);
	//these variables are impacted by GUI sliders to incfrease effect of 3 functions, separation cohesion and align.
	sep *= s;
	coh *= c;
	alig *= a;
	//std::cout << s << " " << c << " " << a;
	this->acceleration += alig;
	
	this->acceleration += sep;
	this->acceleration += coh;
	

	//	std::cout << "\n " << bird.getRotation();
	//std::cout << acceleration.x << acceleration.y;
}
	//std::cout << this->acceleration.x << this->acceleration.y;
	//this->acceleration += alignment;

}

float Boids::dist(float x1, float y1, float x2, float y2)
{
	
		// Calculating distance
		return sqrt(pow(x2 - x1, 2) +
			pow(y2 - y1, 2) * 1.0);
	

}

sf::Vector2f Boids::limit(sf::Vector2f vec, float lim)
{
	sf::Vector2f tempVec = getUnit(vec);
		tempVec = tempVec  * lim;


		return tempVec;
}

float Boids::getMag(sf::Vector2f vec)
{

	return sqrt(vec.x * vec.x + vec.y * vec.y);
	
}

void Boids::edges(float x, float y)
{
	//x and y are player co ordinates, and the check is out of screen bounds, approximately (slightly further)
	if (this->position.x >x+1500)
		this->position.x = x-1500;

	else if (this->position.x < x- 1500)
		this->position.x = x+1500;

	if (this->position.y > y+1200)
		this->position.y = y-1200;

	else if (this->position.y < y-1200)
		this->position.y = y+1200;
	//this->bird.setFillColor(sf::Color(50 + rand() % 200, 50 + rand() % 200, 50 + rand() % 200, alp));
}
