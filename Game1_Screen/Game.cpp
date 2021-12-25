#include "Game.h"
#include "clientside.h"
#include <iostream>
#include <string>
#include <sstream>
#include "Animation.h"
#include "Player.h"
void Game::initInterface()
{

	if (!fontUI.loadFromFile("Arial.ttf"))
	{
		std::cout << "Fond aerial not loaded from 'Font UI, loadInterfact function" << std::endl;
	}
	else
		std::cout << "loadInterface - Fonts loaded." << std::endl;
	
	this->uiText.setFont(this->fontUI);
	this->uiText.setCharacterSize(32);
	this->uiText.setFillColor(sf::Color::Red);
	this->uiText.setString("SCORES!");
	//scores.set
	sf::Text scores("Score:", fontUI, 25);
	ui.push_back(scores);
	userText.setFont(this->fontUI);
	userText.setCharacterSize(13);
	userText.setFillColor(sf::Color::Red);
	
}

//Private member functions
void Game::initVariables()
{
	screenSize.x = 1200;
	screenSize.y = 1600;
	this->window = nullptr;
//resizing Players across server.
client.sPlayers.setScale(0.21, 0.21);
	// fill rectangle colours for rain
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

	this->videoMode.height = 1070;
	this->videoMode.width = 1710;
//	this->videoMode.getDesktopMode;
	this->window = new sf::RenderWindow(this->videoMode, "Game: Text Engine", sf::Style::Titlebar | sf::Style::Close);
	this->window->setFramerateLimit(70);

	//view.setCenter(1000, 900);
	view.zoom(zoomfactor);
	view.setViewport(sf::FloatRect(0, 0, 1, 1));
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
	

	iplayerTexture.loadFromFile("WomanWalking4.png");
	iplayerTexture.createMaskFromColor(sf::Color::Black);
	if (!playerTexture.loadFromImage(iplayerTexture))
	{
		std::cout << "image not loaded";
	}
	else
		std::cout << " loadedAnimation";
	player.actor.setTexture(&playerTexture);


	//load second player textures
	iplayerTwoTexture.loadFromFile("WomanWalking2.png");
	iplayerTwoTexture.createMaskFromColor(sf::Color::Black);
	if (!playerTwoTexture.loadFromImage(iplayerTwoTexture))
	{
		std::cout << "image not loaded";
	}
	else
		std::cout << " loadedAnimation";
	playerTwo.actor.setTexture(&playerTwoTexture);
	
	//sf::Clock clock;
	



	water.loadFromFile("sWater.txt", sf::Shader::Fragment);
	water.setUniform("iResolution", sf::Vector3f(3000, 2200.0, 1000.0));
	water.setUniform("iTime", 100);
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
	npc1.loadFromFile("wo1.png");
	npc1.createMaskFromColor(sf::Color::Black);
	texNpc1.loadFromImage(npc1);
	sprNpc1.setTexture(texNpc1);
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
	sprNpc1.scale(0.21, 0.21);
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
	spChar.setPosition(8180, 1180);
	
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
		//view.rotate(180);
		//spChar.rotate(180);
		window->setView(view);
		//spChar.setScale(-0.21, 0.21);
	
		spChar.move(0, 0);
	}
}
//Constructors / Destructors.
Game::Game()
{
	
	this->initVariables();
	this->initNPC();
	this->initWindow();
	this->initEnemies();
	this->initInterface();
	this->initConnection();
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

void Game::initConnection()
{

	

	bool connect = false;
	while (!connect)
		if (client.socket.connect(client.ip, 2000) == sf::Socket::Done)
		{
			connect = true;
			std::cout << "Connection made\n ";
			std::cout << "Connected to server \n " << client.socket.getRemoteAddress();


		}
		else
			std::cout << "probing for server\n " << sf::IpAddress::LocalHost << "3000\n";


}
void Game::initNPC()
{
	if (!texNpc1.loadFromImage(iplayerTexture))
	{
		std::cout << "didn't load NPC1";
	}
	else std::cout << "\nloaded NPC texture";
	//Animation npc(&texNpc1, sf::Vector2u(1, 1), 0.14f, routefind.path);
	npc.actor.setSize(sf::Vector2f(97, 200));
	npc.eFacing = Animation::eDirectionFacing::East;

	npc.imageCount = sf::Vector2u(4, 4);
	npc.currentImage.x = 0;
	
	//npc.npcStepTime = 0.14f;
	//npc.actor.setTexture(&playerTexture);
	npc.actor.setTexture(&texNpc1);
	npc.uvRect.width = texNpc1.getSize().x / float(npc.imageCount.x);
	npc.uvRect.height = texNpc1.getSize().y / float(npc.imageCount.y);
	
//	npc.actor.setTextureRect(npc.uvRect);
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

			case sf::Event::TextEntered:
				if (ev.text.unicode < 128)
				{
					chat.playerInput += ev.text.unicode;
					chat.playerText.setString(chat.playerInput);
					chat.playerText.setPosition(spChar.getPosition().x, spChar.getPosition().y - 150);
				}
			
			

		//case sf::Event::Closed:
		//	this->window->close();
			break;
		case sf::Event::KeyPressed:
			if (this->ev.key.code == sf::Keyboard::Delete)
				chat.playerInput = "";
			if (this->ev.key.code == sf::Keyboard::Escape)
				this->window->close();
			if (this->ev.key.code == sf::Keyboard::Down)
			{
				//spChar.setTexture(Char);
				still = false;
				faceRight = false;
				faceUp = false;
				faceDown = true;
				
				sf::Vector2i location;
				location.x = spChar.getPosition().x;
				location.y = spChar.getPosition().y;

				//DEBUG COMMENTS std::cout << "Direct location: " << location.x << ", " << location.y << std::endl;
				std::string down = "Down ";
				down += username;
				client.sendingpacket << username << down << location.x << location.y << chat.playerInput.toAnsiString();
				//DEBUG COMMENTS std::cout << "Printing message from Game.dowNkey function: " << chat.playerInput.toAnsiString();

				movement = "Down ";
				movement += username;
				client.sendingpacket << movement;
				
				if (client.socket.send(client.sendingpacket) != sf::Socket::Done) {
					std::cout << "packet 'Down' not sent";
				}
				client.sendingpacket.clear();

				spChar.move(0, 8);
				player.actor.move(0, 8);
				aPosition.x = spChar.getPosition().x + 25 - (screenSize.x / 2);
				aPosition.y = spChar.getPosition().y + 25 - (screenSize.y / 2);

				if (aPosition.x < 0)
					aPosition.x = 0;
				if (aPosition.y < 0)
					aPosition.y = 0;

				view.reset(sf::FloatRect(aPosition.x, aPosition.y, screenSize.x, screenSize.y));
				

				view.zoom(zoomfactor);
				window->setView(view);
			}
			if (this->ev.key.code == sf::Keyboard::Left)
			{
				
				still = false;
				faceUp = false;
				faceDown = false;
				faceRight = false;
				//spChar.setTexture(Char2);
				spChar.move(-8, 0);
				player.actor.move(-8, 0);
				sf::Vector2i location;
				location.x = spChar.getPosition().x;
				location.y = spChar.getPosition().y;

				//DEBUG COMMENTSstd::cout << "Direct location: " << location.x << ", " << location.y << std::endl;
				std::string left = "Left ";
				left += username;
				client.sendingpacket << username << left << location.x << location.y << chat.playerInput.toAnsiString();

				if (client.socket.send(client.sendingpacket) != sf::Socket::Done) {
					std::cout << "packet 'Left' not sent";
				}
				client.sendingpacket.clear();

				aPosition.x = spChar.getPosition().x + 25 - (screenSize.x / 2);
				aPosition.y = spChar.getPosition().y + 25 - (screenSize.y / 2);

				if (aPosition.x < 0)
					aPosition.x = 0;
				if (aPosition.y < 0)
					aPosition.y = 0;

				view.reset(sf::FloatRect(aPosition.x, aPosition.y, screenSize.x, screenSize.y));
				
				view.zoom(zoomfactor);
				window->setView(view);
			}
			if (this->ev.key.code == sf::Keyboard::Up)
			{
				player.actor.move(0, -8);
				//spChar.setTexture(Char3);
				still = false;
				faceDown = false;
				faceUp = true;
				faceRight = false;
				sf::Vector2i location;
				location.x = spChar.getPosition().x;
				location.y = spChar.getPosition().y;

				//DEBUG COMMENTS  std::cout << "Direct location: " << location.x << ", " << location.y << std::endl;
				std::string up = "Up ";
				up += username;
				client.sendingpacket << username << up << location.x << location.y << chat.playerInput.toAnsiString();

				//spChar.setTexture(Char4);
				if (client.socket.send(client.sendingpacket) != sf::Socket::Done) {
					std::cout << "packet 'Up' not sent";
				}
				client.sendingpacket.clear();

				spChar.move(0, -8);
				aPosition.x = spChar.getPosition().x + 25 - (screenSize.x / 2);
				aPosition.y = spChar.getPosition().y + 25 - (screenSize.y / 2);

				if (aPosition.x < 0)
					aPosition.x = 0;
				if (aPosition.y < 0)
					aPosition.y = 0;
				
				view.reset(sf::FloatRect(aPosition.x, aPosition.y, screenSize.x, screenSize.y));
				
				view.zoom(zoomfactor);
				window->setView(view);
			}	if (this->ev.key.code == sf::Keyboard::Right)
			{
				still = false;
				faceDown = false;
				faceUp = false;
				faceRight = true;
				sf::Vector2i location;
				location.x = spChar.getPosition().x;
				location.y = spChar.getPosition().y;

				//DEBUG COMMENTS std::cout << "Direct location: " << location.x << ", " << location.y << std::endl;
				std::string right = "Right ";
				right += username;
				client.sendingpacket << username << right << location.x << location.y << chat.playerInput.toAnsiString();


				//spChar.setTexture(Char4);
				if (client.socket.send(client.sendingpacket) != sf::Socket::Done) {
					std::cout << "packet 'Right' not sent"; 
					}
				client.sendingpacket.clear();
				spChar.move(8, 0);
				player.actor.move(8, 0);
				
				aPosition.x = spChar.getPosition().x + 25 - (screenSize.x / 2);
				aPosition.y = spChar.getPosition().y + 25 - (screenSize.y / 2);
				
				if (aPosition.x < 0)
					aPosition.x = 0;
				if (aPosition.y < 0)
					aPosition.y = 0;

				view.reset(sf::FloatRect(aPosition.x, aPosition.y, screenSize.x, screenSize.y));
				
				view.zoom(zoomfactor);
				window->setView(view);
			}
			//	this->enemy.setPosition(sf::Vector2f(102.f, 100.f));
			
			if (this->ev.key.code == sf::Keyboard::RShift)
				if (grid == true)
					grid = false;
				else
					grid = true;

			if (this->ev.key.code == sf::Keyboard::RControl)
				if (gridPath == true)
					gridPath = false;
				else
					gridPath = true;
			break;
			break;

		case sf::Event::KeyReleased:
		{still = true;
		
		//DEBUG COMMENTS std::cout << "Key released";
		}
			
		break;
		case sf::Event::MouseWheelMoved:
		{
			vZoom = 1.0;
			vZoom -= 0.05 * ev.mouseWheel.delta;
			//std::cout << vZoom <<std::endl; //uncomment to display zoom level to console
			view.zoom(vZoom);
			window->setView(view);
			std::cout << vZoom;
			break;
		}

		case sf::Event::MouseButtonPressed:

			bool pathColor = false;
			int nNodeSize =100;
			sf::Vector2i XY = sf::Mouse::getPosition(*window);
			//int nSelectedNodeY = sf::Mouse::getPosition(window) ;
			
			std::cout << "\nPixels" << XY.x << ", " << XY.y << "";
				sf::Vector2f worldPos = window->mapPixelToCoords(XY,view);

			
			std::cout << "\nCoords" << worldPos.x << ", " << worldPos.y;
			if (ev.type == sf::Event::MouseButtonPressed) // Use mouse to draw maze, shift and ctrl to place start and end
			{
				
				
			
				//routefind.nodeEnd->x = (worldPos.x / 100);
				// routefind.nodeEnd->y =( worldPos.y / 100);
				 //std::cout << "\nNew node end: " << routefind.nodeEnd->x << ", " << routefind.nodeEnd->y << "\n";
			
				{
					if (ev.mouseButton.button == sf::Mouse::Left)
					{
						
						if (!pathColor)
							pathColor = true;
						else
							pathColor = false;
						std::cout << "\nNew Node Start is: " << routefind.nodeStart->x << ", " << routefind.nodeStart->y << "\n";
						std::cout << "Original end node is: " << routefind.nodeEnd->x << ", " << routefind.nodeEnd->y << "\n";
						routefind.nodeStart = &routefind.nodes[(int(worldPos.y) / 100) * routefind.nMapWidth + (int(worldPos.x) / 100)];
						
						npc.path.clear();
						npc.currentCount = 0;
						routefind.solve_AStar();

						
						std::cout << "\n\n Size of path count in NPC animation update function: " << npc.pathCount;
						
						std::cout << "\nPath (starting at the end) is: ";
					
					

					}
					else if (ev.mouseButton.button == sf::Mouse::Right)
					{
						//npc.path.clear();
						if (pathColor)
							pathColor = true;
						else
							pathColor = false;

						routefind.nodeEnd = &routefind.nodes[(int(worldPos.y) / 100) * routefind.nMapWidth + (int(worldPos.x) / 100)];
						
						std::cout << "\nNew end node is: " << routefind.nodeEnd->x << ", " << routefind.nodeEnd->y << "";
						std::cout << "\nOriginal Start node is: " << routefind.nodeStart->x << ", " << routefind.nodeStart->y << "\n";
						
						
						
						std::cout << "\n\n Size of path count in NPC animation update function: " << npc.pathCount;

						std::cout << "\nPath (starting at the end) is: ";

						

							
					}
					else
					{
						npcs.clear();
						routefind.nodes[(int(worldPos.y) / 100) * routefind.nMapWidth + (int(worldPos.x) / 100)].bObstacle = true;
					}
					

						//routefind.nodes[XY.y * routefind.nMapWidth + XY.x].bObstacle = !routefind.nodes[XY.y * routefind.nMapWidth + XY.x].bObstacle;

					 // Solve in "real-time" gives a nice effect
				}
				
			}
			
			
			
			if (pathColor)
				pathColor = false;
			if (!pathColor)
				pathColor = true;
			
			













		}
		
	}

	//set text name locations
	userText.setPosition(spChar.getPosition().x - 15, spChar.getPosition().y - 65);
	
	
}

	//void Game::initClient(sf::TcpSocket* rsocket)
	//{

	//		while (true) {
	//		//rsocket->setBlocking(false);
	//		if (rsocket->receive(this->rpacket) == sf::Socket::Done)
	//		{
	//			std::cout << "received data";
	//		}
	//		else
	//			std::cout << "\npacket not received";

	//		std::this_thread::sleep_for((std::chrono::milliseconds)10);
	//		//}
	//	}

	//}

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
			randomh = 1 + rand() % (15);
			randomsp[i] = 15 + rand() % (28);
			randomx = 1 + rand() % (2200);
			randomr = 210 + rand() % (45);
			randomg = 210 + rand() % (45);
			randomb = 210 + rand() % (45);
			rectangle[i].setSize(sf::Vector2f(4, randomh));
			rectangle[i].setPosition(randomx, -7);
			rectangle[i].setFillColor(sf::Color(randomr, randomg, randomb));



		}
	}
		for (int i = 0; i < 900; i++)
		{}//	this->window->draw(rectangle[i]);


		for (int i = 0; i < 700; i++) {
		if (rectangle[i].getPosition().y > 2000){
			randomx = -300 + rand() % (2500);
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
	//window->draw(spChar);
	
	//window->draw(actorMain);
	this->renderPlayers();

	
	

	routefind.solve_AStar();
	npc.path = routefind.OnUserUpdate(0.0f);
	//npc.pathCount = routefind.path.size();
	
	


	
	this->window->draw(player.actor);
	float deltaTime = 0.0f;
	deltaTime = npcClock.restart().asSeconds();
	npc.Update(0, deltaTime, faceRight, faceDown, faceUp, still);
	

	for (auto a : routefind.path)
	{
		sf::RectangleShape shape;
		shape.setSize(sf::Vector2f(100 / 7, 100 / 7));
		shape.setPosition(sf::Vector2f(a.x * 100, a.y * 100));
		
			shape.setFillColor(sf::Color::Blue);
		npcs.push_back(shape);

		std::cout << "\nDrawn Node results at : " << a.x << ", " << a.y * 100 << " |  ";
	}

	npc.actor.setTextureRect(npc.uvRect);
	this->window->draw(npc.actor);
	if (npc.currentCount >= npc.path.size()-1) {
		routefind.nodeStartBuffer = routefind.nodeStart;
		routefind.nodeEndBuffer = routefind.nodeEnd;
		routefind.nodeEnd = routefind.nodeStartBuffer;
		routefind.nodeStart = routefind.nodeEndBuffer;
		npc.currentCount = 0;
	}
	//this->window->draw(playerTwo.actor);
	this->window->draw(userText);
	window->draw(chat.playerText);
	
	//Testing NPC animation.
	window->draw(sprNpc1);
	if (grid)
	{
		this->window->draw(routefind.vaGrid);
		this->window->draw(routefind.vaLine);
	}
	if (gridPath)
	{
		for (auto npc : npcs)
			this->window->draw(npc);

	}
	this->window->display(); //Tell app that window is done drawing.
	
	
}

void Game::update()
{

	//Event Polling
	this->pollEvents();
	this->updateMousePositions();
	
	this->updateEnemies();
	
	client.ReceivePackets(&client.socket);
	this->renderPlayers();
}


void Game::updateMousePositions()
{
	this->mousePosWindow = sf::Mouse::getPosition(*this->window);
	this->mousePosView = this->window->mapPixelToCoords(this->mousePosWindow);
	//Uncomment to display mouse position in console
	//std::cout << "Mouse is at: " << mousePosWindow.x << ", " << mousePosWindow.y << std::endl;
}

void Game::login()
{

	std::cout << "\nPlease enter Character name: ";
	username = "RyanB1";
	userText.setString(username);
}

void Game::renderPlayers()
{//renders play chat when online.
	for (auto& e : client.PlayerMap)
	{
		//std::cout << "\n" << e.getPosition().x << ", " << e.getPosition().y << std::endl;
		if (e.first != username)
		{
			this->window->draw(e.second);
			for (auto& f : client.ChatMap)

			{
				if (e.first == f.first)
				{
					sf::String playerInput{};
					playerInput = f.second;
					sf::Text playerText;
					playerText.setString(playerInput);
					playerText.setCharacterSize(15);
					playerText.setPosition(e.second.getPosition().x, e.second.getPosition().y +220);
					playerText.setFont(fontUI);
					this->window->draw(playerText);
				}
			}
		}

	/*	for (auto& f : client.ChatMap)
		{
			std::cout << e.first << f.first << f.second << std::endl;
		*/
			/*if (e.first == f.first)
			{
				sf::String playerInput;
				sf::Text playerText;
				playerInput = f.second;
				playerText.setPosition(e.second.getPosition().x, e.second.getPosition().y);
				playerText.setFillColor(sf::Color::Red);
				playerText.setString(playerInput);
				playerText.setFont(fontUI);
				this->window->draw(playerText);

				}*/
//				conversations.push_back(playerText);
				
				//location to draw text. ;
				
			

		
	}

		



		//client.vPlayers.clear();
		return void();
	
}