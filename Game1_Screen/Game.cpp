#include "Game.h"
#include "clientside.h"
#include <iostream>
#include <string>
#include <sstream>
#include "Animation.h"
#include "Player.h"
#include "Population.h"
#include <math.h>
#include "imgui/imgui.h"
#include "imgui/imgui-SFML.h"
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
	clockImGui.restart(); 
	fShaderClock = shaderClock.restart().asSeconds();
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


	//initialise path collide vector
	pathDataW.open("pathdata.txt");
	if (pathDataW.is_open())
	{
		std::string line;
		std::cout << std::endl;
		while (std::getline(pathDataW, line))
		{
			//	std::cout << line;
			for (auto digit : line)
			{
				vPathInts.push_back(digit);
			}
			vPathCollide.push_back(vPathInts);
			vPathInts.clear();
		}
	}

	for (int i = 0; i < 74; i++)
	{
		for (int j = 0; j < 129; j++)
		{
			if (vPathCollide[i][j] == '1') {
				sf::RectangleShape dot;
				dot.setPosition(sf::Vector2f(i * 100, j * 100));
				dot.setSize(sf::Vector2f(10, 10));
				dot.setFillColor(sf::Color::Yellow);
				vPathVisualAid.push_back(dot);
			}
		}

	}
	//set circle (mouse hover signifier) variables
	circle.setRadius(20);
	circle.setOutlineColor(sf::Color::Red);
	circle.setOutlineThickness(3);
	circle.setFillColor(sf::Color::Blue);
	circle.setScale(1.25, 0.25);
	initSprites();

	
}


void Game::initWindow()
{
	
	this->videoMode.height = 1800;
	this->videoMode.width = 2600;
//	this->videoMode.getDesktopMode;
	this->window = new sf::RenderWindow(this->videoMode, "The Better Verse", sf::Style::Default);
	ImGui::SFML::Init(*window);
	ImGuiIO& io = ImGui::GetIO();
	io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
	this->window->setFramerateLimit(100);

	view.setCenter(3700, 1260);
	view.zoom(zoomfactor);
	view.setViewport(sf::FloatRect(0, 0, 1, 1));
	this->window->setView(view);
	
	humanity.peopleAmount =700;
	humanity.populate();
	
	humanity.window = window;
	humanity.createBounds();
	dogGR.peopleAmount = 150;
	dogGR.populate();
	dogGR.window = window;

	drones.peopleAmount = 90;
	drones.populate();
	drones.window = window;

	scooters.peopleAmount = 700;
	scooters.populate();
	scooters.window = window;

	
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
	noise.loadFromFile("noise.png");
	//test water shader
	if (!texFountain.loadFromFile("water.jpg"))
		std::cout << "Water for shader not loaded";

	sprFountain.setTexture(texFountain);
	sprFountain.setPosition(4700, 8100);
//	sprFountain.setOrigin(0.5, 0.5);
	sprFountain.setScale(0.65, 3);
	//sprFountain.setRotation(90);
	iplayerTexture.loadFromFile("WomanWalking4.png");
	iplayerTexture.createMaskFromColor(sf::Color::Black);
	if (!playerTexture.loadFromImage(iplayerTexture))
	{
		std::cout << "image not loaded";
	}
	else
		std::cout << "Loaded female animation sprite\n";
	player.actor.setTexture(&playerTexture);

	////load second player textures
	//iplayerTwoTexture.loadFromFile("WomanWalking2.png");
	//iplayerTwoTexture.createMaskFromColor(sf::Color::Black);
	//if (!playerTwoTexture.loadFromImage(iplayerTwoTexture))
	//{
	//	std::cout << "image not loaded";
	//}
	//else
	//	std::cout << " loadedAnimation";
	//playerTwo.actor.setTexture(&playerTwoTexture);

	//Load day/night shader(called water, change name to something relevant)
	water.loadFromFile("sWater.txt", sf::Shader::Fragment);
	water.setUniform("u_time", uTime);
	water.setUniform("u_resolution", sf::Vector2f(1710.0, 1070.0));
	water.setUniform("dayTime", dayTime);
	water.setUniform("u_mouse", sf::Vector2f(sf::Mouse::getPosition()));

	sFountain.loadFromFile("sFountain.txt", sf::Shader::Fragment);
	sFountain.setUniform("iResolution", sf::Vector2f(1070,1710));
	sFountain.setUniform("iTime", fShaderClock);
	sFountain.setUniform("iChannel0", texFountain);
	sFountain.setUniform("iChannel1", noise);
	//sFountain.setUniform("");
	sFountain.setUniform("iMouse", sf::Vector2f(sf::Mouse::getPosition()));
	//sFountain.setUniform();
	//sFountain.setUniform();
	
	//Char.loadFromImage(image);
	spChar.setTexture(Char);
	

	//Load main background file (replace with tile system / resource manager
	if (!bg.loadFromFile("RyanChar1.png"))
	{
		std::cout << "Couldn't load background file RyanChar1png" << std::endl;
	}
	CharBG.setTexture(bg);
	CharBG.scale(1.0, 1.0); //Background /world size scaling.
	spChar.scale(0.21, 0.21);
	
	//spChar.setTexture(Char2);
	spChar.setPosition(3180, 3980); //spChar is an old character that is now invisible, but is moved in event capturing. The main camera follows spChar.
	//Current new char is rendered at same coordinates so makes no difference. Delete spChar or keep for a backup char.
	
}
void Game::checkCollide()
{
	
	for (auto& npc : scooters.people)
	{
		//	npc.pathSearch.solve_AStar();
			//npc2.pathSearch.solve_AStar();
			//for (auto &npc2 : humanity.people) {
			//	sf::Vector2i XY = npc2.getLocality();
			//	std::cout << "npc2 position: " << npc2.actor.getPosition().x << ", " << npc2.actor.getPosition().y << "\n";
			//	sf::Vector2f worldPos = window->mapPixelToCoords(XY, view);
				//std::cout << "converted world pos" << worldPos.x << ", " << worldPos.y << "\n";

		if (npc.actor.getGlobalBounds().intersects(player.actor.getGlobalBounds()))//&& npc.ID != npc2.ID && npc.pathSearch.nodeEnd != nullptr)
		{	experience += 1;
		exper = std::to_string(experience);
		return;
		//std::cout << experience << " : " << exper;
		//npc.pathSearch.nodes[0].bObstacle = true;
		
			//npc.pathSearch.solve_AStar();

		//	std::cout << "node is | " << npc.pathSearch.nodes[static_cast<int>((worldPos.y) / 100 * npc.pathSearch.nMapWidth + (worldPos.x) / 100) - 1].x << ", " << npc.pathSearch.nodes[static_cast<int>((worldPos.y) / 100 * npc.pathSearch.nMapWidth + (worldPos.x) / 100) - 1].y << " | ";
			//std::cout << "node is | " << npc.pathSearch.nodes[static_cast<int>((worldPos.y) / 100 * npc.pathSearch.nMapWidth + (worldPos.x) / 100) - 1].x << npc.pathSearch.nodes[static_cast<int>((worldPos.y) / 100 * npc.pathSearch.nMapWidth + (worldPos.x) / 100) - 1].y << " | ";

			//npc.pathSearch.nodes[static_cast<int>((worldPos.y) / 100 * npc.pathSearch.nMapWidth + (worldPos.x) / 100)].bObstacle = true;
			//npc.pathSearch.solve_AStar();
			//npc2.pathSearch.solve_AStar();
			//npc.path = npc.pathSearch.OnUserUpdate(0.2f);
			//npc2.path = npc2.pathSearch.OnUserUpdate(0.2f);

		}
		
	}
		
}
Game::Game()
{
	//Check all sections are doing the relevant things.
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
//	if (!texNpc1.loadFromImage(iplayerTexture))
//	{
//		std::cout << "didn't load NPC1";
//	}
//	else std::cout << "\nloaded NPC texture";
//	//Animation npc(&texNpc1, sf::Vector2u(1, 1), 0.14f, routefind.path);
//	npc.actor.setSize(sf::Vector2f(97, 200));
//	//npc.eFacing = Animation::eDirectionFacing::East;
//	npc.switchTime = 0.2f;
//	npc.imageCount = sf::Vector2u(4, 4);
//	npc.currentImage.x = 0;
//	
//	//npc.npcStepTime = 0.14f;
//	//npc.actor.setTexture(&playerTexture);
//	//npc.actor.setTexture(&texNpc1);
//	npc.uvRect.width = texNpc1.getSize().x / float(npc.imageCount.x);
//	npc.uvRect.height = texNpc1.getSize().y / float(npc.imageCount.y);
//	npc.actor.setTextureRect(npc.uvRect);

	
}
void Game::spawnEnemy()
{



}

void Game::pollEvents()
{
	while (this->window->pollEvent(this->ev)) {
		ImGui::SFML::ProcessEvent(ev);
		
		switch (this->ev.type)
		{
			
			//Takes text for chat client. Small delete key deletes current text (once player presses it then moves location by one pixel, it will delete).
		case sf::Event::TextEntered:
			if (ev.text.unicode < 128)
			{
				chat.playerInput += ev.text.unicode;
				chat.playerText.setString(chat.playerInput);
				chat.playerText.setPosition(spChar.getPosition().x, spChar.getPosition().y - 150);
			}

			/*case sf::Event::Closed:
				this->window->close();*/

			break;
		case sf::Event::KeyPressed:

			if (this->ev.key.code == sf::Keyboard::Delete)
				chat.playerInput = "";
			/*if (this->ev.key.code == sf::Keyboard::Escape)
				this->window->close();*/
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
				//std::cout << "\n x: " << spChar.getPosition().x << ", y: " << spChar.getPosition().y << "\n";
				//DEBUG COMMENTS std::cout << "Direct location: " << location.x << ", " << location.y << std::endl;
				
				//###Need to send packet less often.### Maybe on a delta time check.
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

				spChar.move(0, 12);
				player.actor.move(0, 12);
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
				
				spChar.move(-12, 0);
				player.actor.move(-12, 0);
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
				player.actor.move(0, -12);
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

				spChar.move(0, -12);
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
				spChar.move(12, 0);
				player.actor.move(12, 0);

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
			if (this->ev.key.code == sf::Keyboard::F2)
			{
				dayTime -= 0.05;
				water.setUniform("dayTime", dayTime);
			}
			if (this->ev.key.code == sf::Keyboard::F3)
			{
				dayTime += 0.05;
				water.setUniform("dayTime", dayTime);
			}
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
			
			if (this->ev.key.code == sf::Keyboard::LControl)
			{
				if (npcMove)
					npcMove = false;
				else
					npcMove = true;
			}

			break;
			

		case sf::Event::KeyReleased:
		{
			still = true;
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
			//std::cout << vZoom;
			break;
		}

		
		case sf::Event::MouseMoved:
		{
		//
			
		
			break;
		}
		case sf::Event::MouseButtonPressed:
			
			bool pathColor = false;
			int nNodeSize = 100;
			sf::Vector2i XY = sf::Mouse::getPosition(*window);
			//int nSelectedNodeY = sf::Mouse::getPosition(window) ;

			
			sf::Vector2f worldPos = window->mapPixelToCoords(XY, view);


			
			if (ev.type == sf::Event::MouseButtonPressed) // 
			{

				//routefind.nodeEnd->x = (worldPos.x / 100);
				// routefind.nodeEnd->y =( worldPos.y / 100);
				 std::cout << "\nNew node end: " << routefind.nodeEnd->x << ", " << routefind.nodeEnd->y << "\n";
				{
					if (ev.mouseButton.button == sf::Mouse::Left)
					{

						if (!pathColor)
							pathColor = true;
						else
							pathColor = false;
						routefind.nodeStart = &routefind.nodes[(int(worldPos.y) / 100) * routefind.nMapWidth + (int(worldPos.x) / 100)];
						std::cout << "\nNew Node Start is: " << routefind.nodeStart->x << ", " << routefind.nodeStart->y << "\n";
						std::cout << "Original end node is: " << routefind.nodeEnd->x << ", " << routefind.nodeEnd->y << "\n";
					
						pathUpdate(worldPos.x, worldPos.y);
						

						for (auto people : humanity.people) //vector of NPCS
						{
							if (people.actor.getGlobalBounds().contains(worldPos))
							{


								circle.setPosition(people.actor.getPosition().x, people.actor.getPosition().y + 220);
								circleID = people.ID;
								std::cout << "\nYou have selected Human: " << people.ID << std::endl;
								break;
							}
						}

						/*	for (auto npc : humanity.people)
						{
							if (npc.actor.getGlobalBounds().contains(this->mousePosView))
							{
								npcsPath.clear();
								for (auto &a : npc.path)
								{
								//This code used to draw a grid for each players location // where they are walking to next.
									sf::RectangleShape shape;
									shape.setSize(sf::Vector2f(100 / 7, 100 / 7));
									shape.setPosition(sf::Vector2f(a.x * 100, a.y * 100));

									shape.setFillColor(sf::Color::White);
									npcsPath.push_back(shape);

									std::cout << "\nDrawn Node results at : " << a.x << ", " << a.y * 100 << " |  ";
									break; //end loop once drawn the one clicked, dont check other 999 potential npcs!
								}
							}
						}*/
						//Documentation > Below > Worldpos.y (down (220) / 100 will give 22 positions, (21 with 0)
						//						> mulitplied by the (width plus the x coordinate / 100). 
						routefind.nodeStart = &routefind.nodes[(int(worldPos.y) / 100) * routefind.nMapWidth + (int(worldPos.x) / 100)];

						//	routefind.nodes		 [(int(worldPos.y) / 100) * routefind.nMapWidth + (int(worldPos.x) / 100)].bObstacle = true;
						npc.path.clear();
						npc.currentCount = 0;
						routefind.solve_AStar();
						std::cout << "\n\n Size of path count in NPC animation update function: " << npc.pathCount;
						std::cout << "\nPath (starting at the end) is: ";
						if (npcMove) {
							//	humanity.aStarPath.nodeStart = &humanity.aStarPath.nodes[(int(worldPos.y) / 100) * routefind.nMapWidth + (int(worldPos.x) / 100)];
							//	humanity.aStarPath.solve_AStar();
							//	humanity.aStarPath.OnUserUpdate(0.2f);
								//humanity.people[0].path = humanity.aStarPath.OnUserUpdate(0.2f);
						}
					}
						if (ev.mouseButton.button == sf::Mouse::Right)
						{


							//npc.path.clear();
							if (pathColor)
								pathColor = true;
							else
								pathColor = false;
						
							routefind.nodeEnd = &routefind.nodes[(int(worldPos.y) / 100) * routefind.nMapWidth + (int(worldPos.x) / 100)];
							npc.path.clear();
							npc.currentCount = 0;
							routefind.solve_AStar();
							std::cout << "\nNew end node is: " << routefind.nodeEnd->x << ", " << routefind.nodeEnd->y << "";
							std::cout << "\nOriginal Start node is: " << routefind.nodeStart->x << ", " << routefind.nodeStart->y << "\n";

							std::cout << "\n\n Size of path count in NPC animation update function: " << npc.pathCount;

							std::cout << "\nPath (starting at the end) is: ";
							npc.path.clear();
							npc.currentCount = 0;
							routefind.solve_AStar();
							if (npcMove) {

							//	humanity.aStarPath.nodeEnd = &humanity.aStarPath.nodes[(int(worldPos.y) / 100) * routefind.nMapWidth + (int(worldPos.x) / 100)];
							//	humanity.aStarPath.solve_AStar();
							//	humanity.aStarPath.OnUserUpdate(0.2f);
							//	humanity.people[0].path = humanity.aStarPath.OnUserUpdate(0.2f);
							}
						}
					//	else
					//	{
							
							//npcs.clear(); // the blue line path for path find results.
							//routefind.nodes[(int(worldPos.y) / 100) * routefind.nMapWidth + (int(worldPos.x) / 100)].bObstacle = true;
							//routefind.solve_AStar();
						//}

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
	circle.setPosition(humanity.people[circleID].actor.getPosition().x+20, humanity.people[circleID].actor.getPosition().y +200);
	/*
	
		@return void
		Updates the enemy spawn timer and spawns enemies when the total amount of enemies is smaller than the maximum.
		Moves the enemies downwards.
		Removes the enemies at the edge of the screen.
	*/

	////Updating the timer for enemy spawning
	//if (this->enemies.size() < this->maxEnemies) {
	//	if (this->enemySpawnTimer >= this->enemySpawnTimerMax)
	//	{
	//		//Spawn the enemy and reset the timer
	//		this->spawnEnemy();
	//		this->enemySpawnTimer = 0.f;

	//	}
	//	else
	//		this->enemySpawnTimer += 1.f;
	//}

	////Move the enemies and updating
	//for (int i = 0; i < this->enemies.size(); i++) {
	//
	//	bool deleted = false;
	//	this->enemies[i].move(0.f, 3.f);

	//	if (this->enemies[i].getPosition().y > this->window->getSize().y)
	//	{
	//		this->enemies.erase(this->enemies.begin() + i);
	//		this->health -= 1;
	//		std::cout << "Health: " << health << std::endl;}
	//}

	////Check if clicked upon
	//if (sf::Mouse::isButtonPressed(sf::Mouse::Left)) {
	//	
	//	if (this->mouseHeld == false)
	//	{
	//		this->mouseHeld = true;
	//		bool deleted = false;
	//		for (size_t i = 0; i < this->enemies.size() && deleted == false; i++)
	//		{
	//			if (this->enemies[i].getGlobalBounds().contains(this->mousePosView))
	//			{
	//				deleted = true;
	//				this->enemies.erase(this->enemies.begin() + i);

	//				//gain points
	//				points += 10;
	//				std::cout << "Points: " << points << std::endl;
	//			}
	//		}
	//	}
	//}
	//else
	//	this->mouseHeld = false;

}



void Game::renderRain()
{

	if (part == 1) {
		for (int i = 0; i < 300; i++) {
			randomh = 1 + rand() % (15);
			randomsp[i] = 15 + rand() % (28);
			randomx = 2700 + rand() % (2200);
			randomr = 0 + rand() % (45);
			randomg = 30 + rand() % (35);
			randomb = 210 + rand() % (45);
		rectangle[i].setSize(sf::Vector2f(4, randomh));
			rectangle[i].setPosition(randomx, -7);
			rectangle[i].setFillColor(sf::Color(randomr, randomg, randomb));



		}
	}
	for (int i = 0; i < 300; i++)
	{
		this->window->draw(rectangle[i]);
	}

		for (int i = 0; i < 700; i++) {
		if (rectangle[i].getPosition().y > 2000){
			randomx = 2700 + rand() % (2500);
			rectangle[i].setPosition(randomx, -5);
			randomsp[i] = 15 + rand() % (28);
			randomr = 0 + rand() % (45);
			randomg = 30 + rand() % (35);
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
	//for (auto& e : this->enemies)
//	{
//		this->window->draw(e);
	
//	}
//	if (spChar.getPosition().x > 1450)
//		spChar.setPosition(10, 1050);
//else
//	spChar.move(2, 0);
}

void Game::renderInterface(sf::RenderTarget& target)
{
	uTime = timeShader.restart().asSeconds();
/*	Rec.setSize(sf::Vector2f(40.f, 80.f));
	Rec.setFillColor(sf::Color::Red);
	Rec.setPosition(1090, 1560);
	window->draw(Rec);
	*/std::stringstream ss;
	
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

	

	this->window->clear(); //Clear old frame
	
	//Draw game objects
	window->draw(CharBG,&water);
	this->renderEnemies();

	this->renderInterface(*this->window);
	if (bRain)
	{
		this->renderRain();
	}
	
	//window->draw(spChar);
	
	//window->draw(actorMain);
	this->renderPlayers();

	//routefind.solve_AStar();delta
	// 
	//npc.path = routefind.OnUserUpdate(0.0f);
	//npc.pathCount = routefind.path.size();
	dayTime += 0.55;
	water.setUniform("dayTime", dayTime);
	
	float deltaTime;
	dayTime -= 0.55;
	water.setUniform("dayTime", dayTime);
	deltaTime = npcClock.restart().asSeconds();
	npcDelta = npcDeltaClock.restart().asSeconds();
	dogTimeHold = deltaTime;
	droneTimeHold = deltaTime;
	scooterTimeHold = deltaTime;
	//npcTimeHold = npcClock.restart().asSeconds();
	//std::cout << npcDeltaClock.getElapsedTime().asSeconds() << "\n";
	//npc.Update(0, deltaTime, faceRight, faceDown, faceUp, still);
	//std::thread cThread(&Game::checkCollide, this);
	// Main thread waits for the new thread th to stop execution and as a result, its own execution gets blocked
	//cThread.join();
	this->checkCollide();
	for (auto& person : humanity.people)
	{
		
		//deltaTime = npcClock.restart().asSeconds();

		//person.npcWalkSwitch = 0.2;
		
		npcTimeHold+= npcDelta;
	//	if(npcTimeHold >= npcDeltaSwitch){
			switch (person.eFacing) {
			case Animation::East:
				person.actor.move(1, 0);
			//	std::cout << float(std::lerp(person.path[person.currentCount].x, person.path[(person.currentCount + 1)].x, 0.1f)) << "\n ";
			person.lerpCount++;// person.path = person.pathSearch.OnUserUpdate(0.2f);
			//	person.actor.setTextureRect(person.uvRect);
			npcTimeHold -= npcDelta;
			//person.actor.setTextureRect(person.uvRect); 
			//this->window->draw(person.actor,&water);
			break;
			
			case Animation::West:
			person.actor.move(-1, 0);
			//	std::cout << float(std::lerp(person.path[person.currentCount].x, person.path[(person.currentCount + 1)].x, 0.1f)) << "\n ";
			person.lerpCount++; //person.path = person.pathSearch.OnUserUpdate(0.2f);
		//	person.actor.setTextureRect(person.uvRect); 
			npcTimeHold -= npcDelta;
			//person.actor.setTextureRect(person.uvRect); 
			//this->window->draw(person.actor, &water);
			break;
			
			case Animation::North:
			person.actor.move(0, -1);
		//	std::cout << float(std::lerp(person.path[person.currentCount].x, person.path[(person.currentCount + 1)].x, 0.1f)) << "\n ";
			person.lerpCount++;
			//person.path = person.pathSearch.OnUserUpdate(0.2f);
			npcTimeHold -= npcDelta;
			//this->window->draw(person.actor, &water);
		//	person.actor.setTextureRect(person.uvRect);
			break;
			
			case Animation::South:
			person.actor.move(0, 1);
			person.lerpCount++;
			//person.path = person.pathSearch.OnUserUpdate(0.2f);
			npcTimeHold -= npcDelta;
			//person.actor.setTextureRect(person.uvRect);
			break;
			
			
			}

		//}
			humanity.createBounds();
		person.UpdateNpc(0, npcDelta);
	//person.actor.setTextureRect(person.uvRect);
		person.actor.setTextureRect(person.uvRect);
	}

	

	

	for (auto& dog : dogGR.people)
	{

	//deltaTime = npcClock.restart().asSeconds();

		//person.npcWalkSwitch = 0.2;

		dogTimeHold += npcDelta;
		//	if(npcTimeHold >= npcDeltaSwitch){
		switch (dog.eFacing) {
		case Animation::East:
			dog.actor.move(1, 0);
			//	std::cout << float(std::lerp(person.path[person.currentCount].x, person.path[(person.currentCount + 1)].x, 0.1f)) << "\n ";
			dog.lerpCount++;// person.path = person.pathSearch.OnUserUpdate(0.2f);
			//	person.actor.setTextureRect(person.uvRect);
			dogTimeHold = 0;
			//person.actor.setTextureRect(person.uvRect); 
			//this->window->draw(person.actor,&water);
			break;

		case Animation::West:
			dog.actor.move(-1, 0);
			//	std::cout << float(std::lerp(person.path[person.currentCount].x, person.path[(person.currentCount + 1)].x, 0.1f)) << "\n ";
			dog.lerpCount++; //person.path = person.pathSearch.OnUserUpdate(0.2f);
		//	person.actor.setTextureRect(person.uvRect); 
			dogTimeHold = 0;
			//person.actor.setTextureRect(person.uvRect); 
			//this->window->draw(person.actor, &water);
			break;

		case Animation::North:
			dog.actor.move(0, -1);
			//	std::cout << float(std::lerp(person.path[person.currentCount].x, person.path[(person.currentCount + 1)].x, 0.1f)) << "\n ";
			dog.lerpCount++;
			//person.path = person.pathSearch.OnUserUpdate(0.2f);
			dogTimeHold = 0;
			//this->window->draw(person.actor, &water);
		//	person.actor.setTextureRect(person.uvRect);
			break;

		case Animation::South:
			dog.actor.move(0, 1);
			dog.lerpCount++;
			//person.path = person.pathSearch.OnUserUpdate(0.2f);
			dogTimeHold = 0;
			//person.actor.setTextureRect(person.uvRect);
			break;

			}

		//}
		dog.UpdateNpc(0, deltaTime);
		dog.actor.setTextureRect(dog.uvRect);

	}


	for (auto& dog : scooters.people)
	{

		//deltaTime = npcClock.restart().asSeconds();

			//person.npcWalkSwitch = 0.2;

		dogTimeHold += npcDelta;
		//	if(npcTimeHold >= npcDeltaSwitch){
		switch (dog.eFacing) {
		case Animation::East:
			dog.actor.move(3, 0);
			//	std::cout << float(std::lerp(person.path[person.currentCount].x, person.path[(person.currentCount + 1)].x, 0.1f)) << "\n ";
			dog.lerpCount++;// person.path = person.pathSearch.OnUserUpdate(0.2f);
			//	person.actor.setTextureRect(person.uvRect);
			dogTimeHold = 0;
			//person.actor.setTextureRect(person.uvRect); 
			//this->window->draw(person.actor,&water);
			break;

		case Animation::West:
			dog.actor.move(-3, 0);
			//	std::cout << float(std::lerp(person.path[person.currentCount].x, person.path[(person.currentCount + 1)].x, 0.1f)) << "\n ";
			dog.lerpCount++; //person.path = person.pathSearch.OnUserUpdate(0.2f);
		//	person.actor.setTextureRect(person.uvRect); 
			dogTimeHold = 0;
			//person.actor.setTextureRect(person.uvRect); 
			//this->window->draw(person.actor, &water);
			break;

		case Animation::North:
			dog.actor.move(0, -3);
			//	std::cout << float(std::lerp(person.path[person.currentCount].x, person.path[(person.currentCount + 1)].x, 0.1f)) << "\n ";
			dog.lerpCount++;
			//person.path = person.pathSearch.OnUserUpdate(0.2f);
			dogTimeHold = 0;
			//this->window->draw(person.actor, &water);
		//	person.actor.setTextureRect(person.uvRect);
			break;

		case Animation::South:
			dog.actor.move(0, 3);
			dog.lerpCount++;
			//person.path = person.pathSearch.OnUserUpdate(0.2f);
			dogTimeHold = 0;
			//person.actor.setTextureRect(person.uvRect);
			break;

		}

		//}
		dog.UpdateTransport(0, deltaTime);
		dog.actor.setTextureRect(dog.uvRect);

	}


	for (auto& dog : drones.people)
	{

		//deltaTime = npcClock.restart().asSeconds();

			//person.npcWalkSwitch = 0.2;

		dogTimeHold += npcDelta;
		//	if(npcTimeHold >= npcDeltaSwitch){
		switch (dog.eFacing) {
		case Animation::East:
			dog.actor.move(1, 0);
			//	std::cout << float(std::lerp(person.path[person.currentCount].x, person.path[(person.currentCount + 1)].x, 0.1f)) << "\n ";
			dog.lerpCount++;// person.path = person.pathSearch.OnUserUpdate(0.2f);
			//	person.actor.setTextureRect(person.uvRect);
			dogTimeHold = 0;
			//person.actor.setTextureRect(person.uvRect); 
			//this->window->draw(person.actor,&water);
			break;

		case Animation::West:
			dog.actor.move(-1, 0);
			//	std::cout << float(std::lerp(person.path[person.currentCount].x, person.path[(person.currentCount + 1)].x, 0.1f)) << "\n ";
			dog.lerpCount++; //person.path = person.pathSearch.OnUserUpdate(0.2f);
		//	person.actor.setTextureRect(person.uvRect); 
			dogTimeHold = 0;
			//person.actor.setTextureRect(person.uvRect); 
			//this->window->draw(person.actor, &water);
			break;

		case Animation::North:
			dog.actor.move(0, -1);
			//	std::cout << float(std::lerp(person.path[person.currentCount].x, person.path[(person.currentCount + 1)].x, 0.1f)) << "\n ";
			dog.lerpCount++;
			//person.path = person.pathSearch.OnUserUpdate(0.2f);
			dogTimeHold = 0;
			//this->window->draw(person.actor, &water);
		//	person.actor.setTextureRect(person.uvRect);
			break;

		case Animation::South:
			dog.actor.move(0, 1);
			dog.lerpCount++;
			//person.path = person.pathSearch.OnUserUpdate(0.2f);
			dogTimeHold = 0;
			//person.actor.setTextureRect(person.uvRect);
			break;

		}

		//}
		dog.UpdateNpc(0, deltaTime);
		dog.actor.setTextureRect(dog.uvRect);

	}
	if(bDrones)
	drones.drawPeople(dayTime, uTime, droneTimeHold);
	if(bScooters)
	scooters.drawPeople(dayTime, uTime, scooterTimeHold);
	if(bDogs)
	dogGR.drawPeople(dayTime, uTime, dogTimeHold);
	if(bHumans)
	humanity.drawPeople(dayTime, uTime, npcDelta);

	fShaderClock = shaderClock.getElapsedTime().asSeconds();
	sFountain.setUniform("iTime", fShaderClock);
	window->draw(sprFountain, &sFountain);
	this->window->draw(player.actor, &water);
	//for (auto a : routefind.path)
	//{
	//	sf::RectangleShape shape;
	//	shape.setSize(sf::Vector2f(100 / 7, 100 / 7));
	//	shape.setPosition(sf::Vector2f(a.x * 100, a.y * 100));
	//	
	//		shape.setFillColor(sf::Color::Blue);
	//	npcs.push_back(shape);

	//	//std::cout << "\nDrawn Node results at : " << a.x << ", " << a.y * 100 << " |  ";
	//}

//	for (auto a : humanity.people[0].path)
	//{
	//	sf::RectangleShape shape;
	//	shape.setSize(sf::Vector2f(100 / 7, 100 / 7));
	//	shape.setPosition(sf::Vector2f(a.x * 100, a.y * 100));

	//	shape.setFillColor(sf::Color::Blue);
	//	npcs.push_back(shape);

	//	//std::cout << "\nDrawn Node results at : " << a.x << ", " << a.y * 100 << " |  ";
	//}
	//npc.actor.setTextureRect(npc.uvRect);
	
	//if (npc.currentCount >= npc.path.size()-1) {
	//	routefind.nodeStartBuffer = routefind.nodeStart;
	//	routefind.nodeEndBuffer = routefind.nodeEnd;
	//	routefind.nodeEnd = routefind.nodeStartBuffer;
	//	routefind.nodeStart = routefind.nodeEndBuffer;
	//	npc.currentCount = 0;
	//}
//for(auto people : humanity.people)
//	if (people.currentCount >= people.path.size() - 2) {
//		//std::cout << people.currentCount << "#### : ####" << people.pathSearch.path.size() << "\n";
//		people.pathSearch.nodeStartBuffer = routefind.nodeStart;
//		people.pathSearch.nodeEndBuffer = routefind.nodeEnd;
//		people.pathSearch.nodeEnd = routefind.nodeStartBuffer;
//		people.pathSearch.nodeStart = routefind.nodeEndBuffer;
//		people.Update(0, deltaTime, false, false, false, false);
//		people.path = people.pathSearch.OnUserUpdate(0.2f);
//		people.currentCount = 0;
//	}
//	this->window->draw(npc.actor, &water);
	//this->window->draw(playerTwo.actor);
	this->window->draw(userText);
	window->draw(chat.playerText);
	window->draw(circle);
	//Testing NPC animation.
	//window->draw(sprNpc1);
	if (grid)
	{
		this->window->draw(npc.pathSearch.vaGrid);
		this->window->draw(npc.pathSearch.vaLine);
	
	}

	if (bPathKey)
		for (auto dot : vPathVisualAid)
		{
			this->window->draw(dot);
		}

		
	if (gridPath)
	{
		for (auto &npc : npcs)
			this->window->draw(npc);

	}
	{
		for (auto &npc : npcsPath)
		this->window->draw(npc);
	//	for(auto &npcs : humanity.people)
	//	this->window->draw(npcs.actor, &water);

	}
	ImGui::SFML::Update(*window, clockImGui.restart());
	static std::string strengh = "Strength";
	ImGui::Begin("Nottingham Game Simulation");
	ImGui::Checkbox("Rain", &bRain);
	ImGui::Checkbox("Path Key [Yellow]", &bPathKey);
	ImGui::Checkbox("Scooters", &bScooters);
	ImGui::Checkbox("Drones", &bDrones);
	ImGui::Checkbox("Dogs", &bDogs);
	ImGui::Checkbox("Humans", &bHumans);
	if (ImGui::Button("Reset Path File"))
	{
		pathReset();
	}
	if (ImGui::Button("Display Path File"))
	{

		
		//display contents of vector of vector of chars (path file)
		for (auto i : vPathCollide) {
			for (auto j : i)
			{
				std::cout << j << ", ";
			}
			std::cout << std::endl;
		}
	}
	if (ImGui::Button("Path Save"))
	{
		pathSave();
	}
	ImGui::Checkbox("Reset PathFinding", &resetPath); //{std::cout <<resetPath };
	ImGui::Checkbox("Grid", &grid);
	ImGui::SliderFloat("Sun Light ", &dayTime, 0.0f, 1.0f);
	ImGui::Text("Experience (Based on scooter contact\nBut will later be based on missions\nand fighting etc.):");
	ImGui::Text(exper.c_str());
	{
		
	}

	ImGui::End();
	ImGui::SFML::Render(*window);
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
	username = "Ryan";
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

void Game::pathReset()
{
	pathDataW.open("pathdata.txt");
	if (pathDataW.is_open())
	{

		for (int i = 0; i < 75; i++)
		{
			for (int j = 0; j < 130; j++)

			{

				pathDataW << 0;
								
				//routefind.nodes[j * 100 + i].bObstacle = false;
				//std::cout << i << " " << j << std::endl;
			}
			pathDataW << "\n";
			}
		pathDataW.close();

	}

	for (auto &vector : vPathCollide)
	{
		for (auto &digit : vector)
		{
		
			digit = '0';
		}
	}
}

void Game::pathUpdate(int x, int y)
{
	
	std::cout << int(x/100) << ", " << int(y/100);
	x = int(x / 100);
	y = int(y / 100);
	pathDataW.open("pathdata.txt");
	if (pathDataW.is_open())
	{
		if (vPathCollide[x][y] == '1')
		{
			int loca = 0;
			for (auto& dot : vPathVisualAid)
			{
				if (dot.getPosition().x == x * 100 && dot.getPosition().y == y * 100)
				{
					vPathVisualAid.erase(vPathVisualAid.begin() + loca);
					//dot.setFillColor(sf::Color::Red);

				}
				loca++;
			}

			vPathCollide[x][y] = '0';
		}
		else
		{
			sf::RectangleShape dot;
			dot.setPosition(sf::Vector2f(x * 100, y * 100));
			dot.setSize(sf::Vector2f(10, 10));
			dot.setFillColor(sf::Color::Yellow);
			vPathVisualAid.push_back(dot);
			vPathCollide[x][y] = '1';
			
		}
		//pathDataW << "This is a line.\n";
		//pathDataW << "This is another line.\n";
		pathDataW.close();
	}
	
	pathdata.open("pathdata.txt");
	std::string line;
	std::cout << std::endl;
	while (std::getline(pathdata, line))
	{
	//	std::cout << line << std::endl;
	}

}



void Game::pathSave()
{
	pathDataW.open("pathdata.txt");
	if (pathDataW.is_open())
	{
		std::string line;
		for (int i = 0; i < 75; i++)
		{
			for (int j = 0; j < 130; j++)

			{

				pathDataW << vPathCollide[i][j];

				//routefind.nodes[j * 100 + i].bObstacle = false;
				//std::cout << i << " " << j << std::endl;
			}
			pathDataW << "\n";
		}
		pathDataW.close();

	}
}