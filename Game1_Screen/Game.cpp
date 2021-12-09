#include "Game.h"

#include <iostream>
#include <string>
#include <sstream>
void Game::initInterface()
{

	if (!fontUI.loadFromFile("Arial.ttf"))
	{
		std::cout << "Fond aerial not loaded from 'Font UI, loadInterfact function" << std::endl;
	}
	else
		std::cout << "loadInterface - Fonts loaded." << std::endl;
	
	this->uiText.setFont(this->fontUI);
	this->uiText.setCharacterSize(12);
	this->uiText.setFillColor(sf::Color::White);
	this->uiText.setString("SCORES!");
	//scores.set
	sf::Text scores("Score:", fontUI, 25);
	ui.push_back(scores);

}

//Private member functions
void Game::initVariables()
{
	screenSize.x = 2200;
	screenSize.y = 2000;
	this->window = nullptr;
	// fill rectangle colours
	for (int i = 0; i < 500; i++)
	{
		this->rectangle[i].setFillColor(sf::Color(235, 149, 13));
	}


	this->points = 0;
	this->enemySpawnTimerMax = 1.f;
	this->enemySpawnTimer = this->enemySpawnTimerMax;
	this->maxEnemies = 0;
	this->mouseHeld = false;
	this->health = 15;
	this->endGame = false;
	initSprites();
}

//void Game::initRain()



void Game::initWindow()
{

	this->videoMode.height = 2200;
	this->videoMode.width = 2000;
//	this->videoMode.getDesktopMode;
	this->window = new sf::RenderWindow(this->videoMode, "Game: Text Engine", sf::Style::Titlebar | sf::Style::Close);
	this->window->setFramerateLimit(70);

	view.setCenter(1000, 1100);
	view.zoom(2);
	view.setViewport(sf::FloatRect(0, 0, 0.9, 0.9));
	this->window->setView(view);
}
void Game::initEnemies()
{
	this->enemy.setPosition(sf::Vector2f(0.f, 0.f));
	this->enemy.setSize(sf::Vector2f(100.f, 100.f));
	//this->enemy.setScale(sf::Vector2f(3, 3));
	//this->enemy.setScale(sf::Vector2f(3, 3));
	this->enemy.setFillColor(sf::Color::Blue);
	//this->enemy.setOutlineColor(sf::Color::Blue);
	//this->enemy.setOutlineThickness(1.f);

	

}	
void Game::initSprites()
{
	
	//load actor
	actorTexture.loadFromFile("walkss.png");
	sf::IntRect aMain(1, 1, 142 , 180);

	
	actorMain.setTexture(actorTexture);
	actorMain.setTextureRect(aMain);
	actorMain.setPosition(300, 300);
	aMain.left += 146;
	actorMain.setTextureRect(aMain);
	image.loadFromFile("wo1.png");
	

	image.createMaskFromColor(sf::Color::Black);
	Char.loadFromImage(image);
	spChar.setTexture(Char);
	//spChar.rotate(45.0f);
	if (!bg.loadFromFile("RyanChar1.png"))
	{
		std::cout << "Couldn't load char_png texture" << std::endl;
	}

	CharBG.setTexture(bg);
	
	// CharBG.setScale(1.8, 1.8); (uncomment to change size of background)
	//if (!Char.loadFromFile("wo1.png"))
	//{
		//std::cout << "Couldn't load char_png texture" << std::endl;
	//}
	
	//spChar.setTexture(Char);
	CharBG.scale(1.0, 1.0);
	spChar.scale(0.21, 0.21);
	if (!Char2.loadFromFile("wo2.png"))
	{
		std::cout << "Couldn't load char_png texture" << std::endl;
	}
	
	if (!Char3.loadFromFile("wo3.png"))
	{
		std::cout << "Couldn't load char_png texture" << std::endl;
	}

	if (!Char4.loadFromFile("wo4.png"))
	{
		std::cout << "Couldn't load char_png texture" << std::endl;
	}
	
	sf::Texture Char2;
	sf::Texture Char4;
	//spChar.setTexture(Char2);
	spChar.setPosition(310, 1050);
	
}
void Game::checkCollide()
{
	sf::Vector2f posStop(spChar.getPosition().x, spChar.getPosition().y);
	if (spChar.getGlobalBounds().intersects(Rec.getGlobalBounds()))
	{
		std::cout << "Collision with Character and Rec\n";
	//	CharBG.setOrigin(CharBG.getGlobalBounds().height, CharBG.getGlobalBounds().width);
	//	CharBG.setScale(-1.1, 1.1);
		//window->draw(CharBG);
		//CharBG.setOrigin(0, 0);
		view.rotate(180);
		//spChar.rotate(180);
		window->setView(view);
		//spChar.setScale(-0.21, 0.21);
	
		spChar.move(-300, 0);
	}
}
//Constructors / Destructors.
Game::Game()
{
	this->initVariables();
	this->initWindow();
	this->initEnemies();
	this->initInterface();
} 

Game::~Game()
{
	delete this->window;
}


//Accessors
const bool Game::running() const
{
	return this->window->isOpen();
}

void Game::spawnEnemy()
{

	/*
	@return void

	Spawns enemies and sets their colour and positions
	-Sets a random position.
	-Sets a random colouir.
	-Adds enemy to the vector.
	*/
	this->enemy.setPosition(
		static_cast<float>(rand() % static_cast<int>(this->window->getSize().x)),
		0.f);
	


	this->enemy.setFillColor(sf::Color::Blue);
	this->enemies.push_back(this->enemy);

}

void Game::pollEvents()
{
	while (this->window->pollEvent(this->ev)) {
		switch (this->ev.type)
		{
		case sf::Event::Closed:
			this->window->close();
			break;
		case sf::Event::KeyPressed:
			if (this->ev.key.code == sf::Keyboard::Escape)
				this->window->close();
			if (this->ev.key.code == sf::Keyboard::Down)
			{
				//spChar.setTexture(Char);
				spChar.move(0, 10);
				aPosition.x = spChar.getPosition().x + 25 - (screenSize.x / 2);
				aPosition.y = spChar.getPosition().y + 25 - (screenSize.y / 2);

				if (aPosition.x < 0)
					aPosition.x = 0;
				if (aPosition.y < 0)
					aPosition.y = 0;

				view.reset(sf::FloatRect(aPosition.x, aPosition.y, screenSize.x, screenSize.y));
				window->setView(view);
			}
			if (this->ev.key.code == sf::Keyboard::Left)
			{
				//spChar.setTexture(Char2);
				spChar.move(-10, 0);
				aPosition.x = spChar.getPosition().x + 25 - (screenSize.x / 2);
				aPosition.y = spChar.getPosition().y + 25 - (screenSize.y / 2);

				if (aPosition.x < 0)
					aPosition.x = 0;
				if (aPosition.y < 0)
					aPosition.y = 0;

				view.reset(sf::FloatRect(aPosition.x, aPosition.y, screenSize.x, screenSize.y));
				window->setView(view);
			}
			if (this->ev.key.code == sf::Keyboard::Up)
			{
				//spChar.setTexture(Char3);
				spChar.move(0, -10);
				aPosition.x = spChar.getPosition().x + 25 - (screenSize.x / 2);
				aPosition.y = spChar.getPosition().y + 25 - (screenSize.y / 2);

				if (aPosition.x < 0)
					aPosition.x = 0;
				if (aPosition.y < 0)
					aPosition.y = 0;

				view.reset(sf::FloatRect(aPosition.x, aPosition.y, screenSize.x, screenSize.y));
				window->setView(view);
			}	if (this->ev.key.code == sf::Keyboard::Right)
			{
				//spChar.setTexture(Char4);
				spChar.move(10, 0);
				
				aPosition.x = spChar.getPosition().x + 25 - (screenSize.x / 2);
				aPosition.y = spChar.getPosition().y + 25 - (screenSize.y / 2);
				
				if (aPosition.x < 0)
					aPosition.x = 0;
				if (aPosition.y < 0)
					aPosition.y = 0;

				view.reset(sf::FloatRect(aPosition.x, aPosition.y, screenSize.x, screenSize.y));
				window->setView(view);
			}
			//	this->enemy.setPosition(sf::Vector2f(102.f, 100.f));
			break;
		case sf::Event::MouseWheelMoved:
		{
			vZoom = 1.0;
			vZoom -= 0.05 * ev.mouseWheel.delta;
			std::cout << vZoom <<std::endl;
			view.zoom(vZoom);
			window->setView(view);
			std::cout << vZoom;
			break;
		}


		}

	}}

void Game::updateEnemies()
{
	/*
	
		@return void
		Updates the enemy spawn timer and spawns enemies when the total amount of enemies is smaller than the maximum.
		Moves the enemies downwards.
		Removes the enemies at the edge of the screen.
	*/

	//Updating the timer for enemy spawning
	if (this->enemies.size() < this->maxEnemies) {
		if (this->enemySpawnTimer >= this->enemySpawnTimerMax)
		{
			//Spawn the enemy and reset the timer
			this->spawnEnemy();
			this->enemySpawnTimer = 0.f;

		}
		else
			this->enemySpawnTimer += 1.f;
	}

	//Move the enemies and updating
	for (int i = 0; i < this->enemies.size(); i++) {
	
		bool deleted = false;
		this->enemies[i].move(0.f, 3.f);

		if (this->enemies[i].getPosition().y > this->window->getSize().y)
		{
			this->enemies.erase(this->enemies.begin() + i);
			this->health -= 1;
			std::cout << "Health: " << health << std::endl;}
	}

	//Check if clicked upon
	if (sf::Mouse::isButtonPressed(sf::Mouse::Left)) {
		
		if (this->mouseHeld == false)
		{
			this->mouseHeld = true;
			bool deleted = false;
			for (size_t i = 0; i < this->enemies.size() && deleted == false; i++)
			{
				if (this->enemies[i].getGlobalBounds().contains(this->mousePosView))
				{
					deleted = true;
					this->enemies.erase(this->enemies.begin() + i);

					//gain points
					points += 10;
					std::cout << "Points: " << points << std::endl;
				}
			}
		}
	}
	else
		this->mouseHeld = false;

}



void Game::renderRain()
{

	if (part == 1) {
		for (int i = 0; i < 900; i++) {
			randomh = 1 + rand() % (25);
			randomsp[i] = 15 + rand() % (28);
			randomx = 1 + rand() % (2200);
			randomr = 210 + rand() % (45);
			randomg = 210 + rand() % (45);
			randomb = 210 + rand() % (45);
			rectangle[i].setSize(sf::Vector2f(6, randomh));
			rectangle[i].setPosition(randomx, -7);
			rectangle[i].setFillColor(sf::Color(randomr, randomg, randomb));



		}
	}
		for (int i = 0; i < 900; i++)
			this->window->draw(rectangle[i]);


		for (int i = 0; i < 900; i++) {
		if (rectangle[i].getPosition().y > 2000){
			randomx = 1 + rand() % (1800);
			rectangle[i].setPosition(randomx, -5);
			randomsp[i] = 15 + rand() % (28);
			randomr = 210 + rand() % (45);
			randomg = 210 + rand() % (45);
			randomb = 210 + rand() % (45);
		
		}

		}

		for (int i = 0; i < 900; i++)
		{
			rectangle[i].move(sf::Vector2f(6, randomsp[i]));
		}

		part = 2;
}
void Game::renderEnemies()
{
	for (auto& e : this->enemies)
	{
		this->window->draw(e);
	
	}
//	if (spChar.getPosition().x > 1450)
//		spChar.setPosition(10, 1050);
//else
//	spChar.move(2, 0);
}

void Game::renderInterface(sf::RenderTarget& target)
{
	Rec.setSize(sf::Vector2f(40.f, 80.f));
	Rec.setFillColor(sf::Color::Red);
	Rec.setPosition(1090, 1560);
	window->draw(Rec);
	std::stringstream ss;
	
	ui[0].setPosition(sf::Mouse::getPosition(*window).x + 35, sf::Mouse::getPosition(*window).y -40);

	//Two different ways to conver to string
	//First Method, with to_string function.
	std::string s_points = std::to_string(points);
	ui[0].setString(s_points);
	
	target.draw(ui[0]);
	s_points = std::to_string(points);
	uiText.setString(s_points);
	target.draw(uiText);
	
	//second method
	int scoretwo = points;
	ss << "Health: " << health;
	this->textTwo.setString(ss.str());
	textTwo.setPosition(0, 12);
	textTwo.setFillColor(sf::Color::White);
	textTwo.setFont(fontUI);
	textTwo.setCharacterSize(12);
	target.draw(textTwo);
}

void Game::render()
{

	/* 
	
	Renders the game objects.

	*/

	this->window->clear(); //Clear old frame
	
	//Draw game objects
	window->draw(CharBG);
	this->renderEnemies();

	this->renderInterface(*this->window);
	this->renderRain();
	this->checkCollide();
	window->draw(spChar);
	window->draw(actorMain);

	this->window->display(); //Tell app that window is done drawing.


}

void Game::update()
{

	//Event Polling
	this->pollEvents();
	this->updateMousePositions();
	this->updateEnemies();

}


void Game::updateMousePositions()
{
	this->mousePosWindow = sf::Mouse::getPosition(*this->window);
	this->mousePosView = this->window->mapPixelToCoords(this->mousePosWindow);
	//Uncomment to display mouse position in console
	//std::cout << "Mouse is at: " << mousePosWindow.x << ", " << mousePosWindow.y << std::endl;
}
