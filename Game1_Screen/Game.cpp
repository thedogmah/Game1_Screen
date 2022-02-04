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
#include "quadtree.h"
	
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
	screenSize.x = 1100;
	screenSize.y = 1100;
	settings.antialiasingLevel = 8.0;
	this->window = nullptr;
	//resizing Players across server.
	client.sPlayers.setScale(0.21, 0.21);
	// fill rectangle colours for rain
	/*for (int i = 0; i < 500; i++)
	{
		this->rectangle[i].setFillColor(sf::Color(235, 149, 13));
	}*/

	this->points = 0;
	this->enemySpawnTimerMax = 1.f;
	this->enemySpawnTimer = this->enemySpawnTimerMax;
	this->maxEnemies = 0;
	this->mouseHeld = false;
	this->health = 15;
	this->endGame = false;

	//Initialise quadtree rectangle boundary
	boundary.h = 3500;
	boundary.w = 3500;
	boundary.x = 1000;
	boundary.y = 1000;
	Point p(10, 10);
	qt.boundary = boundary;
	qt.capacity = 2;
	for (int i = 0; i < 100; i++) {
		p.x = 1 + rand() % (2500);
		p.y = 1 + rand() % (2000);
		qt.insert(p);
	}

	//Initialise path collide vector
	pathDataW.open("pathdata.txt");
	if (pathDataW.is_open())
	{
		std::string line;
		std::cout << std::endl;
		while (std::getline(pathDataW, line))
		{
			//	std::cout << line;
			for (auto &digit : line)
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

	//load trambus

	if (!imgTramBus.loadFromFile("trambus.png"))
		std::cout << "trambus.png failed to load\n";
	imgTramBus.createMaskFromColor(sf::Color::White, 255);
	texTramBus.loadFromImage(imgTramBus);
	recTramBus.setTexture(&texTramBus);
	//sprTramBus.setTexture(texTramBus);
	recTramBus.setPosition(7000., 4375);
	recTramBus.setSize(sf::Vector2f(imgTramBus.getSize()));
	recTramBus2.setScale(sf::Vector2f(-1, 1));

	//tram 2
	if (!imgTramBus2.loadFromFile("trambus.png"))
		std::cout << "trambus.png failed to load\n";
	imgTramBus2.createMaskFromColor(sf::Color::White, 255);
	texTramBus2.loadFromImage(imgTramBus);
	recTramBus2.setTexture(&texTramBus);
	//sprTramBus.setTexture(texTramBus);
	recTramBus2.setPosition(-2000., 4275);
	recTramBus2.setSize(sf::Vector2f(imgTramBus.getSize()));

	if (!imgTrees.loadFromFile("treesBare.png"))
		std::cout << "Trees Bare PNG not loaded";
	imgTrees.createMaskFromColor(sf::Color::White, 255);
	texTrees.loadFromImage(imgTrees);
	recTrees.setTexture(&texTrees);
	recTrees.setSize(sf::Vector2f(imgTrees.getSize()));
	recTrees.setPosition(6000, 6700);
	
	if (!imgTreeAsset.loadFromFile("treebare.png"))
		std::cout << "Not loaded (tree asset)" << '\n';
	imgTreeAsset.createMaskFromColor(sf::Color::White, 255);

	texTreeAsset.loadFromImage(imgTreeAsset);
	rectTree.setTexture(&texTreeAsset);
	rectTree.setSize(sf::Vector2f(imgTreeAsset.getSize()));
	sprTreeAsset.setTexture(texTreeAsset);
	//Read treeAsset file into vector
	assetDataW.open("treedata.txt");
	if (assetDataW.is_open())
	{
		int loop = 0;
		std::cout << "Tree file open\n";
		while (std::getline(assetDataW, sAssetTree))
		{

			std::stringstream oss(sAssetTree);
			while (oss >> asset)

			{
				//oss >> asset;


				std::cout << "Substring: " << asset << ", ";
				//sTreeAsset.x = std::stof(asset);//	std::cout << line << std::endl;}
				if (loop % 2 == 0) {
					sTreeAsset.ID = loop;
					sTreeAsset.x = std::stof(asset);
					std::cout << sTreeAsset.x << '\n';
				}
				else
				{
					sTreeAsset.y = std::stof(asset);
					std::cout << sTreeAsset.y << '\n';
				}
				loop++;
				if (loop % 2 == 0) {
					sTreeAsset.ID = loop / 2;
					vStructAssets.push_back(sTreeAsset);
					
				}
			}
			//if (loop % 2 == 0)



		}
		assetDataW.close();
	}
	




}


void Game::initWindow()
{
	
	this->videoMode.height = 1530;
	this->videoMode.width = 1500;
//	this->videoMode.getDesktopMode;
	this->window = new sf::RenderWindow(this->videoMode, "The City", sf::Style::Default, settings);
	ImGui::SFML::Init(*window);
	ImGuiIO& io = ImGui::GetIO();
	io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
	this->window->setFramerateLimit(100);

	view.setCenter(3700, 1260);
	view.zoom(zoomfactor);
	view.setViewport(sf::FloatRect(0, 0, 1, 1));

	followView.zoom(0.85);
	followView.setViewport(sf::FloatRect(0, 0, 1, 1));


	flyingView.setCenter(5000, 5000);
	flyingView.setViewport(sf::FloatRect(0.7, 0, 1, 0.4));
	flyingView.zoom(1.3);
	this->window->setView(view);
	if (!humanityMaleGreen.imgHuman.loadFromFile("protagonistgreen.png"))

		std::cout << "Green Protagonist not loaded";
	humanityMaleGreen.peopleAmount = 150;
	
	 
		
		
		
	humanityMaleGreen.populate();
//	humanityMaleGreen.texHuman.loadFromImage(humanityMaleGreen.imgHuman);
	humanityMaleGreen.window = window;
	humanityMaleGreen.createBounds();


	if (!humanityMaleSandyJacket.imgHuman.loadFromFile("protagonistsandy.png"))

		std::cout << "Green Protagonist not loaded";
	humanityMaleSandyJacket.peopleAmount = 220;

		humanityMaleSandyJacket.populate();
//	humanityMaleGreen.texHuman.loadFromImage(humanityMaleGreen.imgHuman);
	humanityMaleSandyJacket.window = window;
	humanityMaleSandyJacket.createBounds();


	if (!humanityMaleWhiteJacket.imgHuman.loadFromFile("protagonistwhitejacket.png"))

		std::cout << "Green Protagonist not loaded";
	sf::Color customBlue(255, 0, 0);
	humanityMaleWhiteJacket.imgHuman.createMaskFromColor(sf::Color(255,0,0));
	humanityMaleWhiteJacket.peopleAmount = 240;
	
	humanityMaleWhiteJacket.populate();
	//	humanityMaleGreen.texHuman.loadFromImage(humanityMaleGreen.imgHuman);
	humanityMaleWhiteJacket.window = window;
	humanityMaleWhiteJacket.createBounds();


	humanity.peopleAmount =156;
	humanity.populate();
	
	humanity.window = window;
	humanity.createBounds();
	dogGR.peopleAmount = 120;
	dogGR.populate();
	dogGR.window = window;

	drones.peopleAmount = 40;
	drones.populate();
	drones.window = window;

	scooters.peopleAmount = 275;
	scooters.populate();
	scooters.window = window;


	if (!scootersManSandyJacket.imgHuman.loadFromFile("scooterManSandyJacket.png"))
		std::cout << "Sandy Jacket Male Scooter Not Loaded. Ouch." << '\n';
	scootersManSandyJacket.peopleAmount = 180;
	scootersManSandyJacket.populate();
	scootersManSandyJacket.window = window;
	

	//snug grey coat woman population
	if (!humanityWomanSnugGrey.imgHuman.loadFromFile("WomanSnugGrey.png"))
		std::cout << "Snug Grey Woman Asset not loaded - not a bad thing really, have you seen that jacket?";
	humanityWomanSnugGrey.peopleAmount = 90;
	humanityWomanSnugGrey.populate();
	humanityWomanSnugGrey.window = window;
	humanityWomanSnugGrey.createBounds();
	//snug black coat woman population
	if (!humanityWomanSnugBlack.imgHuman.loadFromFile("WomanSnugBlack.png"))
		std::cout << "Snug Grey Woman Asset not loaded - not a bad thing really, have you seen that jacket?";
	humanityWomanSnugBlack.peopleAmount = 90;
	humanityWomanSnugBlack.populate();
	humanityWomanSnugBlack.window = window;
	humanityWomanSnugBlack.createBounds();
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

	for (auto& humans : humanityMaleGreen.people)
	{
		if (recTramBus.getGlobalBounds().contains(humans.actor.getPosition().x, humans.actor.getPosition().y + 100) || recTramBus2.getGlobalBounds().contains(humans.actor.getPosition().x, humans.actor.getPosition().y + 100)) {

			humans.stopAnimate = true;
			humans.stopMove = true;
			humans.currentImage.x = 3;
			humans.currentImage.y = 3;
			humans.UpdateNpc(3, 0.2);
		}
		else {
			humans.stopMove = false;
			humans.stopAnimate = false;
		}
	}
	player.vVibesText.clear();
	if (player.bVibeInstinctSwitch) {
		for (auto& humans : humanityMaleGreen.people) {
			if (recTramBus.getGlobalBounds().contains(humans.actor.getPosition().x, humans.actor.getPosition().y + 100) || recTramBus2.getGlobalBounds().contains(humans.actor.getPosition().x, humans.actor.getPosition().y + 100)) {
				VibeText.setCharacterSize(14);
				VibeText.setString(sf::String(npcVibe(humans)));
				VibeText.setPosition(humans.actor.getPosition().x, humans.actor.getPosition().y - 18);
				VibeText.setFont(fontUI);
				player.vibeCatch(VibeText.getString());
				player.vVibesText.push_back(VibeText);
				//window->draw(VibeText);
				//	std::cout << "Vibe instinct collide with Male Green\n";
				player.profile.powerVibeInstinct -= player.profile.powerVibeInstinctTolerance;
			}
		}
	}

	for (auto& humans : humanityMaleSandyJacket.people)
	{
		if (recTramBus.getGlobalBounds().contains(humans.actor.getPosition().x, humans.actor.getPosition().y + 100) || recTramBus2.getGlobalBounds().contains(humans.actor.getPosition().x, humans.actor.getPosition().y + 100)) {
			humans.currentImage.x = 3;
			humans.currentImage.y = 3;
			humans.UpdateNpc(3, 0.2);

			humans.stopMove = true;
			humans.stopAnimate = true;



		}
		else {
			humans.stopMove = false;
			humans.stopAnimate = false;
		}
	}
	if (player.bVibeInstinctSwitch) {
		for (auto& humans : humanityMaleSandyJacket.people) {

			if (recTramBus.getGlobalBounds().contains(humans.actor.getPosition().x, humans.actor.getPosition().y + 100) || recTramBus2.getGlobalBounds().contains(humans.actor.getPosition().x, humans.actor.getPosition().y + 100)) {
				VibeText.setCharacterSize(14);
				VibeText.setString(sf::String(npcVibe(humans)));
				VibeText.setPosition(humans.actor.getPosition().x, humans.actor.getPosition().y - 18);
				VibeText.setFont(fontUI);
				player.vibeCatch(VibeText.getString());
				player.vVibesText.push_back(VibeText);
				//	window->draw(VibeText);
					//	std::cout << "Vibe instinct collide with Sandy Jacket Male Green\n";
			}
		}
	}
	for (auto& humans : humanityMaleWhiteJacket.people)
	{
		if (recTramBus.getGlobalBounds().contains(humans.actor.getPosition().x, humans.actor.getPosition().y + 100) || recTramBus2.getGlobalBounds().contains(humans.actor.getPosition().x, humans.actor.getPosition().y + 100)) {
			humans.currentImage.x = 3;
			humans.currentImage.y = 3;
			humans.UpdateNpc(3, 0.2);

			humans.stopMove = true;
			humans.stopAnimate = true;

		}
		else {
			humans.stopMove = false;
			humans.stopAnimate = false;
		}
		if (player.bVibeInstinctSwitch) {
			for (auto& humans : humanityMaleWhiteJacket.people)

				if (player.vibeInstinct.getGlobalBounds().contains(humans.actor.getPosition().x, humans.actor.getPosition().y))
				{
					VibeText.setCharacterSize(14);
					VibeText.setString(sf::String(npcVibe(humans)));
					VibeText.setPosition(humans.actor.getPosition().x, humans.actor.getPosition().y - 18);
					VibeText.setFont(fontUI);
					//window->draw(VibeText);
					player.vibeCatch(VibeText.getString());
					player.vVibesText.push_back(VibeText);
					//	std::cout << "Vibe instinct collide with White Jacket Male Green\n";
				}
		}
	}
	for (auto& humans : humanityWomanSnugBlack.people)
	{
		if (recTramBus.getGlobalBounds().contains(humans.actor.getPosition().x, humans.actor.getPosition().y + 100) || recTramBus2.getGlobalBounds().contains(humans.actor.getPosition().x, humans.actor.getPosition().y + 100)) {
			humans.stopMove = true;
			humans.stopAnimate = true;
			humans.UpdateNpc(3, 0.2);
			humans.currentImage.x = 3;
			humans.currentImage.y = 3;
			
		}
		else {
			humans.stopMove = false;
			humans.stopAnimate = false;
		}
	}	if (player.bVibeInstinctSwitch) {
		for (auto& humans : humanityWomanSnugBlack.people) {

			if (player.vibeInstinct.getGlobalBounds().contains(humans.actor.getPosition().x, humans.actor.getPosition().y))
			{
				VibeText.setCharacterSize(14);
				VibeText.setString(sf::String(npcVibe(humans)));
				VibeText.setPosition(humans.actor.getPosition().x, humans.actor.getPosition().y - 18);
				VibeText.setFont(fontUI);
				player.vibeCatch(VibeText.getString());
				player.vVibesText.push_back(VibeText);
				//window->draw(VibeText);
				//	std::cout << "Vibe instinct collide with Woman, snug black Jacket\n";
			}
		}
	}
	for (auto& humans : humanityWomanSnugGrey.people)
	{
		if (recTramBus.getGlobalBounds().contains(humans.actor.getPosition().x, humans.actor.getPosition().y + 100) || recTramBus2.getGlobalBounds().contains(humans.actor.getPosition().x, humans.actor.getPosition().y + 100)) {
			humans.stopMove = true;
			humans.stopAnimate = true;
			humans.UpdateNpc(3, 0.2);
			humans.currentImage.x = 3;
			humans.currentImage.y = 3;
		//	humans.UpdateNpc(3, 0.2);
		}
		else {
			humans.stopMove = false;
			humans.stopAnimate = false;
		}
	}	if (player.bVibeInstinctSwitch) {
		for (auto& humans : humanityWomanSnugGrey.people) {
			if (player.vibeInstinct.getGlobalBounds().contains(humans.actor.getPosition().x, humans.actor.getPosition().y))
			{
				VibeText.setCharacterSize(14);
				VibeText.setString(sf::String(npcVibe(humans)));
				VibeText.setPosition(humans.actor.getPosition().x, humans.actor.getPosition().y - 18);
				VibeText.setFont(fontUI);
				player.vibeCatch(VibeText.getString());
				player.vVibesText.push_back(VibeText);
				//window->draw(VibeText);
				//	std::cout << "Vibe instinct collide with Woman, Snug Gret Jacket\n";
			}
		}
	}

	for (auto& humans : humanity.people)
	{
		if (recTramBus.getGlobalBounds().contains(humans.actor.getPosition().x, humans.actor.getPosition().y + 100) || recTramBus2.getGlobalBounds().contains(humans.actor.getPosition().x, humans.actor.getPosition().y + 100)) {
			humans.stopMove = true;
			humans.stopAnimate = true;
			humans.currentImage.x = 3;
			humans.currentImage.y = 3;
			humans.UpdateNpc(3, 0.2);
		}
		else {
			humans.stopMove = false;
			humans.stopAnimate = false;
		}
	}
	if (player.bVibeInstinctSwitch) {
		for (auto& humans : humanity.people) {
			if (player.vibeInstinct.getGlobalBounds().contains(humans.actor.getPosition().x, humans.actor.getPosition().y))
			{
				VibeText.setCharacterSize(14);
				VibeText.setString(sf::String(npcVibe(humans)));
				VibeText.setPosition(humans.actor.getPosition().x, humans.actor.getPosition().y - 18);
				VibeText.setFont(fontUI);
				player.vibeCatch(VibeText.getString());
				player.vVibesText.push_back(VibeText);
				//window->draw(VibeText);
				//	std::cout << "Vibe instinct collide with Woman, Snug Gret Jacket\n";
			}

		}
	}
		for (auto& humans : scooters.people) {
			if (recTramBus.getGlobalBounds().contains(humans.actor.getPosition().x, humans.actor.getPosition().y + 100) || recTramBus2.getGlobalBounds().contains(humans.actor.getPosition().x, humans.actor.getPosition().y + 100))
			{
				humans.stopMove = true;
				humans.stopAnimate = true;
				humans.currentImage.x = 3;
				humans.currentImage.y = 3;
				humans.UpdateNpc(3, 0.2);
			}
			else {
				humans.stopMove = false;
				humans.stopAnimate = false;
			}

			for (auto& humans : scootersManSandyJacket.people) {
				if (recTramBus.getGlobalBounds().contains(humans.actor.getPosition().x, humans.actor.getPosition().y + 100) || recTramBus2.getGlobalBounds().contains(humans.actor.getPosition().x, humans.actor.getPosition().y + 100))
				{
					humans.stopMove = true;
					humans.stopAnimate = true;
					humans.currentImage.x = 3;
					humans.currentImage.y = 3;
					humans.UpdateNpc(3, 0.2);
				}
				else {
					humans.stopMove = false;
					humans.stopAnimate = false;
				}
			}
			//for (auto& npc : scooters.people)
			//{
			//	//	npc.pathSearch.solve_AStar();
			//		//npc2.pathSearch.solve_AStar();
			//		//for (auto &npc2 : humanity.people) {
			//		//	sf::Vector2i XY = npc2.getLocality();
			//		//	std::cout << "npc2 position: " << npc2.actor.getPosition().x << ", " << npc2.actor.getPosition().y << "\n";
			//		//	sf::Vector2f worldPos = window->mapPixelToCoords(XY, view);
			//			//std::cout << "converted world pos" << worldPos.x << ", " << worldPos.y << "\n";

			//	if (npc.actor.getGlobalBounds().intersects(player.actor.getGlobalBounds()))//&& npc.ID != npc2.ID && npc.pathSearch.nodeEnd != nullptr)
			//	{
			//		experience += 1;
			//		exper = std::to_string(experience);
			//		return;
			//		//std::cout << experience << " : " << exper;
			//		//npc.pathSearch.nodes[0].bObstacle = true;

			//			//npc.pathSearch.solve_AStar();

			//		//	std::cout << "node is | " << npc.pathSearch.nodes[static_cast<int>((worldPos.y) / 100 * npc.pathSearch.nMapWidth + (worldPos.x) / 100) - 1].x << ", " << npc.pathSearch.nodes[static_cast<int>((worldPos.y) / 100 * npc.pathSearch.nMapWidth + (worldPos.x) / 100) - 1].y << " | ";
			//			//std::cout << "node is | " << npc.pathSearch.nodes[static_cast<int>((worldPos.y) / 100 * npc.pathSearch.nMapWidth + (worldPos.x) / 100) - 1].x << npc.pathSearch.nodes[static_cast<int>((worldPos.y) / 100 * npc.pathSearch.nMapWidth + (worldPos.x) / 100) - 1].y << " | ";

			//			//npc.pathSearch.nodes[static_cast<int>((worldPos.y) / 100 * npc.pathSearch.nMapWidth + (worldPos.x) / 100)].bObstacle = true;
			//			//npc.pathSearch.solve_AStar();
			//			//npc2.pathSearch.solve_AStar();
			//			//npc.path = npc.pathSearch.OnUserUpdate(0.2f);
			//			//npc2.path = npc2.pathSearch.OnUserUpdate(0.2f);

			//	}

			//}
		}
	}


std::string Game::npcMarried(Animation person)
{
	switch (person.married)
	{
	case 0:
		return "Single";
	case 1:
		return "Married";
	case 2:
		return "Polyamory";
	case 3:
		return "Undisclosed";
	}
}

std::string Game::npcReligion(Animation person)
{
	switch (person.religion)
	{
	case 0:
		return "Christian";
	case 1:
		return "Athiest";
	case 2:
		return "Agnostic";
	case 3:
		return "Muslim";
	case 4:
		return "Hindu";
	case 5:
		return "Sikh";
	case 6:
		return "Judaism";
	case 7:
		return "Buddhism";
	case 8:
		return "Mormon";
	case 9:
		return "Jehovah Witness";
	case 10:
		return "Catholic";
	case 11:
		return "Christian";
	case 12:
		return "Catholic";
	case 13:
		return "Christian";
	case 14:
		return "Spirituality";
	}
}

std::string Game::npcVibe(Animation person)
{
	switch (person.vibe)
	{
	case 0:
		return "Wholesome";
	case 1:
		return "Pure";
	case 2:
		return "Creepster";
	case 3:
		return "Angry";
	case 4:
		return "Warm";
	case 5:
		return "Confused";
	case 6:
		return "Flirty";
	case 7:
		return "Joyful";
	case 8:
		return "Sad";
	case 9:
		return "Defensive";
	case 10:
		return "Intimidating";
	case 11:
		return "Friendly";
	case 12:
		return "Peaceful";
	case 13:
		return "Chill";
	case 14:
		return "Quiet";
	case 15:
		return "Positive";
	}

}
std::string Game::npcCareer(Animation person)
{
	switch (person.career)
	{
	case 0:
		return "Manager";

	case 1:
		return "Project Manager";
	case 2:
		return "Teacher";
	case 3:
		return "Director";
	case 4:
		return "Accountant";
	case 5:
		return "Consultant";
	case 6:
		return "Administrator";
	case 7:
		return "Solicitor";
	case 8:
		return "Account Manager";
	case 9:
		return "PA";
	case 10:
		return "Office Manager";
	case 11:
		return "Analyst";
	case 12:
		return "Engineer";
	case 13:
		return "Sales Manager";
	case 14:
		return "Doctor";
	case 15:
		return "Software Engineer";
	case 16:
		return "Business Analyst";
	case 17:
		return "Managing Director";
	case 18:
		return "Personal Assistant";
	case 19:
		return "Marketing Manager";
	case 20:
		return "Secretary";
	case 21:
		return "Graphic Designer";
	case 22:
		return "Lecturer";
	case 23:
		return "Architect";
	case 24:
		return "HR Manager";
	case 25:
		return "Buyer";
	case 26:
		return "Driver";
	case 27:
		return "Plumber";
	case 28:
		return "Researcher";
	case 29:
		return "Journalist";
	case 30:
		return "Graduate";

	}
}
std::string Game::npcSexuality(Animation person)
{
	switch (person.sexuality)
	{
	case 0:
		return "Straight";
	case 1:
		return "Bisexual";
	case 2:
		return "Homosexual";
	case 3:
		return "Curious";
	case 4:
		return "Asexual";
	case 5:
		return "Straight";
	case 6:
		return "Bisexual";
	case 7:
		return "Undisclosed";
	case 8:
		return "Straight";
	case 9:
		return "Straight";
	case 10:
		return "Straight";
	case 11:
		return "Straight";
	case 12:
		return "Straight";
	case 13:
		return "Bisexual";
	}
}
void Game::initClient(sf::TcpSocket* rsocket)
{
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

void Game::npcLookingGlass(Animation npc)
{
	//std::cout << "start of looking glass imgui\n";
	ImGui::PushStyleColor(ImGuiCol_WindowBg, (ImVec4)ImColor::ImColor(50, 110, 110, 105));
	//ImGui::SFML::Update(*window, clockImGui.restart());
	ImGui::Begin("Character Looking Glass");
	
	ImGui::PopStyleColor(1);
	ImGui::SameLine(163);
	ImGui::Text("Human Number ");
	ImGui::SameLine(98);
	std::string label = std::to_string(static_cast<int>(npc.ID));
	ImGui::Text(label.c_str());
//	ImGui::SameLine(171);
	
	sf::Sprite character;
	character.setTexture(*npc.actor.getTexture());
	npc.uvRect.left = 0;
	npc.uvRect.width = npc.actor.getSize().x * 2;
	character.setTextureRect(npc.uvRect);
	ImGui::Image(character);
	ImGui::Text("Their Vibe is ");
	ImGui::SameLine(107);
	label = npcVibe(npc);
	ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0.9, 0.9, 0.5, 1.0));
	ImGui::Text(label.c_str());
	ImGui::PopStyleColor(1);
	ImGui::Text("Their career is ");
	ImGui::SameLine(121);
	label = npcCareer(npc);
	ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0.9, 0.9, 0.5, 1.0));
	ImGui::Text(label.c_str());
	ImGui::PopStyleColor(1);
	
	//std::cout << "mid looking glass imgui\n";
	ImGui::Text("Their influencer reach is ");
	ImGui::SameLine(189);
	label = std::to_string(npc.influencer);
	ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0.9, 0.9, 0.5, 1.0));
	ImGui::Text(label.c_str());
	ImGui::PopStyleColor(1);

	ImGui::Text("Their personal wealth is ");
	ImGui::SameLine(182);
	label = std::to_string(npc.wealth);
	ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0.9, 0.9, 0.5, 1.0));
	ImGui::Text(label.c_str());
	ImGui::PopStyleColor(1);

	ImGui::Text("Their IQ is ");
	ImGui::SameLine(90);
	label = std::to_string(npc.intelligence);
	ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0.9, 0.9, 0.5, 1.0));
	ImGui::Text(label.c_str());
	ImGui::PopStyleColor(1);

	ImGui::Text("Their sexuality is ");
	ImGui::SameLine(141);
	label = npcSexuality(npc);
	ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0.9, 0.9, 0.5, 1.0));
	ImGui::Text(label.c_str());
	ImGui::PopStyleColor(1);
//	std::cout << "midend of imgui looking glass\n";
	ImGui::Text("Their marital status is ");
	ImGui::SameLine(177);
	label = npcMarried(npc);
	ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0.9, 0.9, 0.5, 1.0));
	ImGui::Text(label.c_str());
	ImGui::PopStyleColor(1);

	ImGui::Text("Their mind health is ");
	ImGui::SameLine(158);
	label = std::to_string(npc.mindHealth);
	ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0.9, 0.9, 0.5, 1.0));
	ImGui::Text(label.c_str());
	ImGui::PopStyleColor(1);

	ImGui::Text("Their body health is ");
	ImGui::SameLine(158);
	label = std::to_string(npc.bodyHealth);
	ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0.9, 0.9, 0.5, 1.0));
	ImGui::Text(label.c_str());
	ImGui::PopStyleColor(1);

	ImGui::Text("Their soul health is ");
	ImGui::SameLine(158);
	label = std::to_string(npc.soulHealth);
	ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0.9, 0.9, 0.5, 1.0));
	ImGui::Text(label.c_str());
	ImGui::PopStyleColor(1);

	ImGui::Text("Their tendancies are");
	ImGui::Text("Crime:");
	ImGui::SameLine(55);
	label = std::to_string(npc.crime);
	ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0.9, 0.9, 0.5, 1.0));
	ImGui::Text(label.c_str());
	ImGui::PopStyleColor(1);
	ImGui::Text("Depression:");
	ImGui::SameLine(90);
	label = std::to_string(npc.depression);
	ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0.9, 0.9, 0.5, 1.0));
	ImGui::Text(label.c_str());
ImGui::PopStyleColor(1);
	ImGui::Text("Anxiety:");
	ImGui::SameLine(69);
	label = std::to_string(npc.anxiety);
	ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0.9, 0.9, 0.5, 1.0));
	ImGui::Text(label.c_str());
	ImGui::PopStyleColor(1);

	ImGui::Text("Their instinct level is ");
	ImGui::SameLine(177);
	label = std::to_string(npc.instinct);
	ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0.9, 0.9, 0.5, 1.0));
	ImGui::Text(label.c_str());
	ImGui::PopStyleColor(1);
//	std::cout << "midendend of looking lass (instinct)\n";
	ImGui::Text("Their belief system is ");
	ImGui::SameLine(170);
	label = npcReligion(npc);
	ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0.9, 0.9, 0.5, 1.0));
	ImGui::Text(label.c_str());
	ImGui::PopStyleColor(1);
	ImGui::Text("Their spirituality level is ");
	ImGui::SameLine(204);
	label = std::to_string(npc.spirituality);
	ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0.9, 0.9, 0.5, 1.0));
	ImGui::Text(label.c_str());
	ImGui::PopStyleColor(1);
	/*ImGui::Text("There Vibe is ");
	ImGui::SameLine(107);
	label = npcVibe(npc);
	ImGui::Text(label.c_str());*/
	//ImGui::PopStyleColor(17);
	//std::cout << "end of looking glassn";
	ImGui::End();
	//std::cout << "very end of looking glass functions\n";
	
	//	"\nTheir career is: " << npcCareer(people) << ".\nTheir influencer reach is : " << people.influencer << "\nTheir personal wealth is: " << people.wealth << "\n\nTheir IQ is: " << people.intelligence << ".\nTheir sexuality is: " << npcSexuality(people) <<
	//	"\nTheir marital status is: " << npcMarried(people) << ".\n\nTheir mind health is: " << people.mindHealth << ".\nTheir body health is: " << people.bodyHealth <<
	//	"\nTheir soul health is: " << people.soulHealth << "\n\nThere tendancies are:\nCrime: " << people.crime << "\nDepression: " << people.depression << "\nAnxiety: " << people.anxiety << "\n\nTheir instinct level is: " << people.instinct << "\nTheir belief system is: " << npcReligion(people) << "\nTheir spirituality is: " << people.spirituality << "\n\n\n\n\n\n\n";


}

void Game::initConnection()
{
		bool connect = false;
	//while (!connect)
		if (client.socket.connect(client.ip, 2000) == sf::Socket::Done)
		{
			connect = true;
			std::cout << "Connection made\n ";
			std::cout << "Connected to server \n " << client.socket.getRemoteAddress();

		}
	//	else
			std::cout << "probing for server\n / Connection needed to be established to run = false (edit InitConnection fuinction in game.cpp to change) " << sf::IpAddress::LocalHost << "3000\n";
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
			{//stops follow npc
				bNpcFollow = false;
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

			if (this->ev.type == sf::Event::JoystickMoved)
			{
				if (this->ev.joystickMove.axis == sf::Joystick::X)
				{
					std::cout << "X axis moved!" << std::endl;
					std::cout << "joystick id: " << ev.joystickMove.joystickId << std::endl;
					std::cout << "new position: " << ev.joystickMove.position << std::endl;
				}
				if (this->ev.joystickMove.axis == sf::Joystick::Y || this->ev.joystickMove.axis == sf::Joystick::R)
				{
					std::cout << "X axis moved!" << std::endl;
					std::cout << "joystick id: " << ev.joystickMove.joystickId << std::endl;
					std::cout << "new position: " << ev.joystickMove.position << std::endl;
				}
			}
			if (this->ev.key.code == sf::Keyboard::Left)
			{
				bNpcFollow = false;
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
				bNpcFollow = false;
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
				//reset drone view /cancel out
				//this->window->setView(view);
				bDroneFollow = false;
				//stops following npc if seleced to follow
				bNpcFollow = false;
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
			vZoom -= 0.02 * ev.mouseWheel.delta;
			//std::cout << vZoom <<std::endl; //uncomment to display zoom level to console
			view.zoom(vZoom);
			window->setView(view);
			//std::cout << vZoom;
			break;
		}


		case sf::Event::MouseMoved:
		{
			//
			sf::Vector2i XY = sf::Mouse::getPosition(*window);
			//int nSelectedNodeY = sf::Mouse::getPosition(window) ;


			sf::Vector2f worldPos = window->mapPixelToCoords(XY, view);
			//for (auto &people : humanity.people) //vector of NPCS
			//{
			//	if (people.actor.getGlobalBounds().contains(worldPos))//This ismousePos))
			//	{


			//		circle.setPosition(people.actor.getPosition().x, people.actor.getPosition().y + 220);
			//		circleID = people.ID;
			//		std::cout << "\nThis is Human number: " << people.ID << '\n' << "There IQ is: " << people.intelligence << ".\nThere sexuality is: " << people.sexuality <<
			//			"There marital status is: " << people.married << ".\nThere mind health is: " << people.mindHealth << ".\nThere body health is: " << people.bodyHealth;
			//		/*Human.intelligence = 1 + rand() % (160); 
			//		Human.sexuality = 0 + rand() % (2);
			//		Human.married = 0 + rand() % (1);
			//		Human.mindHealth = 1 + rand() % (100);
			//		Human.bodyHealth = 1 + rand() % (100);
			//		Human.soulHealth = 1 + rand() % (100);
			//		Human.influencer = 1 +*/
			//		break;
			//	}
			//}

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
				// std::cout << "\nNew node end: " << routefind.nodeEnd->x << ", " << routefind.nodeEnd->y << "\n";
				{
					if (ev.mouseButton.button == sf::Mouse::Left)
					{

						//	Point p(worldPos.x, worldPos.y);
						//	qt.insert(p);
						if (!pathColor)
							pathColor = true;
						else
							pathColor = false;
						routefind.nodeStart = &routefind.nodes[(int(worldPos.y) / 100) * routefind.nMapWidth + (int(worldPos.x) / 100)];
						//std::cout << "\nNew Node Start is: " << routefind.nodeStart->x << ", " << routefind.nodeStart->y << "\n";
						//std::cout << "Original end node is: " << routefind.nodeEnd->x << ", " << routefind.nodeEnd->y << "\n";
					//	bool found = false;


						for (auto& people : humanity.people) //vector of NPCS
						{
							if (people.actor.getGlobalBounds().contains(worldPos))//This ismousePos))
							{
								//SETTING VIEW PORT FOR NPC FOLLOW
								circle.setPosition(people.actor.getPosition().x, people.actor.getPosition().y + 220);
								circleID = people.ID;
								popCircleLocator = &people;
								/*	system("cls");
									std::cout << "This is Human number: " << people.ID << ".\nTheir vibe is: " << npcVibe(people) << "\nTheir career is: " << npcCareer(people) << ".\nTheir influencer reach is : " << people.influencer  << "\nTheir personal wealth is: " << people.wealth << "\n\nTheir IQ is: " << people.intelligence << ".\nTheir sexuality is: " << npcSexuality(people) <<
										"\nTheir marital status is: " << npcMarried(people) << ".\n\nTheir mind health is: " << people.mindHealth << ".\nTheir body health is: " << people.bodyHealth <<
										"\nTheir soul health is: " << people.soulHealth << "\n\nThere tendancies are:\nCrime: " << people.crime <<  "\nDepression: " <<people.depression << "\nAnxiety: " << people.anxiety << "\n\nTheir instinct level is: " << people.instinct << "\nTheir belief system is: " << npcReligion(people) << "\nTheir spirituality is: " <<people.spirituality << "\n\n\n\n\n\n\n";
								*/	vNPCLookingGlass.clear();
								vNPCLookingGlass.push_back(people);
								/*Human.intelligence = 1 + rand() % (160);
								Human.sexuality = 0 + rand() % (2);
								Human.married = 0 + rand() % (1);
								Human.mindHealth = 1 + rand() % (100);
								Human.bodyHealth = 1 + rand() % (100);
								Human.soulHealth = 1 + rand() % (100);
								Human.influencer = 1 +*/
								//found = true;
								followView.setCenter(people.actor.getPosition().x, people.actor.getPosition().y);

								this->window->setView(followView);
								vNpcFollowPointer = &people;
								bNpcFollow = true;
								return;





							}
						}

						for (auto& people : humanityMaleGreen.people) //vector of NPCS
						{
							if (people.actor.getGlobalBounds().contains(worldPos))//This ismousePos))
							{

								if (people.stopAnimate == true)
								{//people.StopNpc();
									people.stopAnimate = false;
									people.stopMove = false;
									//	people.currentImage.x = 3;
									//	people.currentImage.y = 0;
									//	// people.UpdateNpc(0, 0.2f);
									//	if (people.eFacing == people.East)
									//	{
									//		people.uvRect.left = people.currentImage.x * people.uvRect.width;
									//		people.uvRect.width = abs(people.uvRect.width);
									//	}
									//	else //facing West
									//	{
									//		people.uvRect.left = (people.currentImage.x + 1) * abs(people.uvRect.width);
									//		people.uvRect.width = -abs(people.uvRect.width);
									//	}

										//people.actor.setTextureRect(people.uvRect);
										//this->window->draw(people.actor, &water);
									//}
								}
								else
									people.stopAnimate = true;

								//system("cls");
								//people.pathSearch.path.clear();
								//people.pathSearch.OnUserCreate();
								//people.currentCount = 0;
								//people.pathSearch.nodeStart = &people.pathSearch.nodes[(int(player.actor.getPosition().y) ) * people.pathSearch.nMapWidth + (int(player.actor.getPosition().x) )];
							//	people.pathSearch.nodeEnd = &people.pathSearch.nodes[(int(player.actor.getPosition().y) ) * people.pathSearch.nMapWidth + (int(player.actor.getPosition().x) )];
						//		std::cout << people.pathSearch.nodes[(int(player.actor.getPosition().y) / 100) * people.pathSearch.nMapWidth + (int(player.actor.getPosition().x) / 100)].x << ", " << people.pathSearch.nodes[(int(player.actor.getPosition().y) / 100) * people.pathSearch.nMapWidth + (int(player.actor.getPosition().x) / 100)].y;

							//	people.pathSearch.solve_AStar();

								//people.pathSearch.nodes.
							//	people.pathSearch.path = people.pathSearch.OnUserUpdate(0.2f);

								circle.setPosition(people.actor.getPosition().x, people.actor.getPosition().y + 220);
								circleID = people.ID;
								popCircleLocator = &people;
								std::cout << "This is Human number: " << people.ID << ".\nTheir vibe is: " << npcVibe(people) << "\nTheir career is: " << npcCareer(people) << ".\nTheir influencer reach is : " << people.influencer << "\nTheir personal wealth is: " << people.wealth << "\n\nTheir IQ is: " << people.intelligence << ".\nTheir sexuality is: " << npcSexuality(people) <<
									"\nTheir marital status is: " << npcMarried(people) << ".\n\nTheir mind health is: " << people.mindHealth << ".\nTheir body health is: " << people.bodyHealth <<
									"\nTheir soul health is: " << people.soulHealth << "\n\nThere tendancies are:\nCrime: " << people.crime << "\nDepression: " << people.depression << "\nAnxiety: " << people.anxiety << "\n\nTheir instinct level is: " << people.instinct << "\nTheir belief system is: " << npcReligion(people) << "\nTheir spirituality is: " << people.spirituality << "\n\n\n\n\n\n\n";
								/*Human.intelligence = 1 + rand() % (160);
								Human.sexuality = 0 + rand() % (2);
								Human.married = 0 + rand() % (1);
								Human.mindHealth = 1 + rand() % (100);
								Human.bodyHealth = 1 + rand() % (100);
								Human.soulHealth = 1 + rand() % (100);
								Human.influencer = 1 +*/
								//found = true;
								vNPCLookingGlass.clear();
								vNPCLookingGlass.push_back(people);
								followView.setCenter(people.actor.getPosition().x, people.actor.getPosition().y);

								this->window->setView(followView);
								vNpcFollowPointer = &people;
								bNpcFollow = true;
								return;
							}
						}


						for (auto& people : drones.people) //vector of NPCS
						{
							if (people.actor.getGlobalBounds().contains(worldPos))//This ismousePos))
							{



								circle.setPosition(people.actor.getPosition().x, people.actor.getPosition().y + 220);
								circleID = people.ID;
								popCircleLocator = &people;
								/*	system("cls");
									std::cout << "This is Human number: " << people.ID << ".\nTheir vibe is: " << npcVibe(people) << "\nTheir career is: " << npcCareer(people) << ".\nTheir influencer reach is : " << people.influencer  << "\nTheir personal wealth is: " << people.wealth << "\n\nTheir IQ is: " << people.intelligence << ".\nTheir sexuality is: " << npcSexuality(people) <<
										"\nTheir marital status is: " << npcMarried(people) << ".\n\nTheir mind health is: " << people.mindHealth << ".\nTheir body health is: " << people.bodyHealth <<
										"\nTheir soul health is: " << people.soulHealth << "\n\nThere tendancies are:\nCrime: " << people.crime <<  "\nDepression: " <<people.depression << "\nAnxiety: " << people.anxiety << "\n\nTheir instinct level is: " << people.instinct << "\nTheir belief system is: " << npcReligion(people) << "\nTheir spirituality is: " <<people.spirituality << "\n\n\n\n\n\n\n";
								*/	vNPCLookingGlass.clear();
								vNPCLookingGlass.push_back(people);
								/*Human.intelligence = 1 + rand() % (160);
								Human.sexuality = 0 + rand() % (2);
								Human.married = 0 + rand() % (1);
								Human.mindHealth = 1 + rand() % (100);
								Human.bodyHealth = 1 + rand() % (100);
								Human.soulHealth = 1 + rand() % (100);
								Human.influencer = 1 +*/
								//found = true;


								vNPCLookingGlass.clear();
								vNPCLookingGlass.push_back(people);
								flyingView.setCenter(people.actor.getPosition().x, people.actor.getPosition().y);

								bufferDroneSwitch = true;//
								droneFollowPointer = &people;
								bNpcFollow = false;
								bDroneFollow = true;



								//this->window->setView(followView);


								return;
							}
						}
								for (auto& people : humanityMaleSandyJacket.people) //vector of NPCS
								{
									if (people.actor.getGlobalBounds().contains(worldPos))//This ismousePos))
									{



										circle.setPosition(people.actor.getPosition().x, people.actor.getPosition().y + 220);
										circleID = people.ID;
										popCircleLocator = &people;
										//system("cls");
										//std::cout << "This is Human number: " << people.ID << ".\nTheir vibe is: " << npcVibe(people) << "\nTheir career is: " << npcCareer(people) << ".\nTheir influencer reach is : " << people.influencer << "\nTheir personal wealth is: " << people.wealth << "\n\nTheir IQ is: " << people.intelligence << ".\nTheir sexuality is: " << npcSexuality(people) <<
										//	"\nTheir marital status is: " << npcMarried(people) << ".\n\nTheir mind health is: " << people.mindHealth << ".\nTheir body health is: " << people.bodyHealth <<
										//	"\nTheir soul health is: " << people.soulHealth << "\n\nThere tendancies are:\nCrime: " << people.crime << "\nDepression: " << people.depression << "\nAnxiety: " << people.anxiety << "\n\nTheir instinct level is: " << people.instinct << "\nTheir belief system is: " << npcReligion(people) << "\nTheir spirituality is: " << people.spirituality << "\n\n\n\n\n\n\n";
										///*Human.intelligence = 1 + rand() % (160);
										/*Human.sexuality = 0 + rand() % (2);
										Human.married = 0 + rand() % (1);
										Human.mindHealth = 1 + rand() % (100);
										Human.bodyHealth = 1 + rand() % (100);
										Human.soulHealth = 1 + rand() % (100);
										Human.influencer = 1 + */
										//	found = true;
										vNPCLookingGlass.clear();
										vNPCLookingGlass.push_back(people);
										followView.setCenter(people.actor.getPosition().x, people.actor.getPosition().y);

										this->window->setView(followView);
										vNpcFollowPointer = &people;
										bNpcFollow = true;
										return;
									}
								}

								for (auto& people : humanityMaleWhiteJacket.people) //vector of NPCS
								{
									if (people.actor.getGlobalBounds().contains(worldPos))//This ismousePos))
									{



										circle.setPosition(people.actor.getPosition().x, people.actor.getPosition().y + 220);
										circleID = people.ID;
										popCircleLocator = &people;
										//system("cls");
										//std::cout << "This is Human number: " << people.ID << ".\nTheir vibe is: " << npcVibe(people) << "\nTheir career is: " << npcCareer(people) << ".\nTheir influencer reach is : " << people.influencer << "\nTheir personal wealth is: " << people.wealth << "\n\nTheir IQ is: " << people.intelligence << ".\nTheir sexuality is: " << npcSexuality(people) <<
										//	"\nTheir marital status is: " << npcMarried(people) << ".\n\nTheir mind health is: " << people.mindHealth << ".\nTheir body health is: " << people.bodyHealth <<
										//	"\nTheir soul health is: " << people.soulHealth << "\n\nThere tendancies are:\nCrime: " << people.crime << "\nDepression: " << people.depression << "\nAnxiety: " << people.anxiety << "\n\nTheir instinct level is: " << people.instinct << "\nTheir belief system is: " << npcReligion(people) << "\nTheir spirituality is: " << people.spirituality << "\n\n\n\n\n\n\n";
										///*Human.intelligence = 1 + rand() % (160);
										/*Human.sexuality = 0 + rand() % (2);
										Human.married = 0 + rand() % (1);
										Human.mindHealth = 1 + rand() % (100);
										Human.bodyHealth = 1 + rand() % (100);
										Human.soulHealth = 1 + rand() % (100);
										Human.influencer = 1 + */
										//	found = true;
										vNPCLookingGlass.clear();
										vNPCLookingGlass.push_back(people);
										followView.setCenter(people.actor.getPosition().x, people.actor.getPosition().y);

										this->window->setView(followView);
										this->window->draw(CharBG, &water);
										vNpcFollowPointer = &people;
										bNpcFollow = true;
										return;
									}
								}

								for (auto& people : scooters.people) //vector of NPCS
								{
									if (people.actor.getGlobalBounds().contains(worldPos))//This ismousePos))
									{



										circle.setPosition(people.actor.getPosition().x, people.actor.getPosition().y + 220);
										circleID = people.ID;
										popCircleLocator = &people;
										//system("cls");
										//std::cout << "This is Human number: " << people.ID << ".\nTheir vibe is: " << npcVibe(people) << "\nTheir career is: " << npcCareer(people) << ".\nTheir influencer reach is : " << people.influencer << "\nTheir personal wealth is: " << people.wealth << "\n\nTheir IQ is: " << people.intelligence << ".\nTheir sexuality is: " << npcSexuality(people) <<
										//	"\nTheir marital status is: " << npcMarried(people) << ".\n\nTheir mind health is: " << people.mindHealth << ".\nTheir body health is: " << people.bodyHealth <<
										//	"\nTheir soul health is: " << people.soulHealth << "\n\nThere tendancies are:\nCrime: " << people.crime << "\nDepression: " << people.depression << "\nAnxiety: " << people.anxiety << "\n\nTheir instinct level is: " << people.instinct << "\nTheir belief system is: " << npcReligion(people) << "\nTheir spirituality is: " << people.spirituality << "\n\n\n\n\n\n\n";
										///*Human.intelligence = 1 + rand() % (160);
										/*Human.sexuality = 0 + rand() % (2);
										Human.married = 0 + rand() % (1);
										Human.mindHealth = 1 + rand() % (100);
										Human.bodyHealth = 1 + rand() % (100);
										Human.soulHealth = 1 + rand() % (100);
										Human.influencer = 1 + */
										//	found = true;
										vNPCLookingGlass.clear();
										vNPCLookingGlass.push_back(people);
										followView.setCenter(people.actor.getPosition().x, people.actor.getPosition().y);

										this->window->setView(followView);
										vNpcFollowPointer = &people;
										bNpcFollow = true;
										return;
									}
								}

								for (auto& people : scootersManSandyJacket.people) //vector of NPCS
								{
									if (people.actor.getGlobalBounds().contains(worldPos))//This ismousePos))
									{



										circle.setPosition(people.actor.getPosition().x, people.actor.getPosition().y + 220);
										circleID = people.ID;
										popCircleLocator = &people;
										//system("cls");
										//std::cout << "This is Human number: " << people.ID << ".\nTheir vibe is: " << npcVibe(people) << "\nTheir career is: " << npcCareer(people) << ".\nTheir influencer reach is : " << people.influencer << "\nTheir personal wealth is: " << people.wealth << "\n\nTheir IQ is: " << people.intelligence << ".\nTheir sexuality is: " << npcSexuality(people) <<
										//	"\nTheir marital status is: " << npcMarried(people) << ".\n\nTheir mind health is: " << people.mindHealth << ".\nTheir body health is: " << people.bodyHealth <<
										//	"\nTheir soul health is: " << people.soulHealth << "\n\nThere tendancies are:\nCrime: " << people.crime << "\nDepression: " << people.depression << "\nAnxiety: " << people.anxiety << "\n\nTheir instinct level is: " << people.instinct << "\nTheir belief system is: " << npcReligion(people) << "\nTheir spirituality is: " << people.spirituality << "\n\n\n\n\n\n\n";
										///*Human.intelligence = 1 + rand() % (160);
										/*Human.sexuality = 0 + rand() % (2);
										Human.married = 0 + rand() % (1);
										Human.mindHealth = 1 + rand() % (100);
										Human.bodyHealth = 1 + rand() % (100);
										Human.soulHealth = 1 + rand() % (100);
										Human.influencer = 1 + */
										//	found = true;
										vNPCLookingGlass.clear();
										vNPCLookingGlass.push_back(people);
										followView.setCenter(people.actor.getPosition().x, people.actor.getPosition().y);

										this->window->setView(followView);
										vNpcFollowPointer = &people;
										bNpcFollow = true;
										return;
									}
								}

								pathUpdate(worldPos.x, worldPos.y);


								//for (auto &people : humanity.people) //vector of NPCS
								//{
								//	if (people.actor.getGlobalBounds().contains(worldPos))//This ismousePos))
								//	{


								//		circle.setPosition(people.actor.getPosition().x, people.actor.getPosition().y + 220);
								//		circleID = people.ID;
								//		std::cout << "\nYou have selected Human: " << people.ID << "\N";
								//		break;
								//	}
								//}

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
								//std::cout << "\n\n Size of path count in NPC animation update function: " << npc.pathCount;
								//std::cout << "\nPath (starting at the end) is: ";
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
	
	//}}


void Game::updateEnemies()
{//
	if (popCircleLocator)
	{
		circle.setPosition(popCircleLocator->actor.getPosition().x + 20, popCircleLocator->actor.getPosition().y + 200);
	//	std::cout << popCircleLocator->actor.getPosition().x + 20 << ", " << popCircleLocator->actor.getPosition().y + 200;
	}
	/*
	* 
	*/
	if (recTramBus.getPosition().x < -4100)
		recTramBus.setPosition(7000., 4375);
	if (recTramBus2.getPosition().x > 10300)
		recTramBus2.setPosition(-3000., 4275);


	/*	@return void
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
//	this->window->clear(); //Clear old frame
	 
		//deal with view affects
	
 //{ /*//change flyingView rotation.
	//	*/if(bufferDroneSwitch)
	//	{
	//		switch (droneFollowPointer->eFacing) {
	//		case Animation::East:
	//			flyingView.rotate(0.2f);
	//			
	//		case Animation::North:
	//			flyingView.rotate(0.2f);
	//			
	//		case Animation::West:
	//			flyingView.rotate(-0.2f);
	//			
	//		case Animation::South:
	//			flyingView.rotate(-0.2f);
	//			
	//		}
			//this->window->setView(flyingView);
		//}
		//this->window->setView(flyingView);
	

			//std::cout << bNpcFollow;
	
	window->draw(CharBG, &water);
	this->renderEnemies();
			
		if (bNpcFollow)

		{
			followView.setCenter(vNpcFollowPointer->actor.getPosition().x, vNpcFollowPointer->actor.getPosition().y);
			this->window->setView(followView);
			//followView.setRotation(180.);
			followView.setViewport(sf::FloatRect(0, 0, 1, 1));
			
		}

		
		
		//Draw game objects
		

		this->renderInterface(*this->window);
		if (bRain)
		{
			this->renderRain();
		}


		else
		{
			//window->setView(view);
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


		//drawers players vibe circle if imgui checked.
		if (player.bvibeInstinctsDraw)
			window->draw(player.vibeInstinct);

		for (auto& person : humanity.people)
		{
			if (!person.stopMove) {
				vRectShapeDataVector.push_back(person.actor);
				//deltaTime = npcClock.restart().asSeconds();

				//person.npcWalkSwitch = 0.2;

				npcTimeHold += npcDelta;
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
			}
			//}
			humanity.createBounds();
			person.UpdateNpc(0, npcDelta);
			//person.actor.setTextureRect(person.uvRect);
			person.actor.setTextureRect(person.uvRect);
		}

		for (auto& person : humanityMaleGreen.people)
		{


			if (person.actor.getRotation() == 0 && person.actor.getRotation() != 22)
			{
				//person.actor.setRotation(8.0f);
			//	person.actor.scale(-1, 1);
			}
			vRectShapeDataVector.push_back(person.actor);
			if (!person.stopMove) {
				//deltaTime = npcClock.restart().asSeconds();

				//person.npcWalkSwitch = 0.2;

				npcTimeHold += npcDelta;
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
				humanityMaleGreen.createBounds();
				person.UpdateNpc(0, npcDelta);
				//person.actor.setTextureRect(person.uvRect);
				person.actor.setTextureRect(person.uvRect);
			}

		}

		//}
		for (auto& person : humanityMaleWhiteJacket.people)
		{
			if (person.actor.getRotation() == 0 && person.actor.getRotation() != 22)
			{
				//	person.actor.setRotation(8.0f);
					//person.actor.scale(-1, -1);
			}
			vRectShapeDataVector.push_back(person.actor);
			//animate them if not in the stop state
			if (!person.stopMove) {

				//deltaTime = npcClock.restart().asSeconds();

				//person.npcWalkSwitch = 0.2;

				npcTimeHold += npcDelta;
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
			}

			//}
			humanityMaleWhiteJacket.createBounds();
			person.UpdateNpc(0, npcDelta);
			//person.actor.setTextureRect(person.uvRect);
			person.actor.setTextureRect(person.uvRect);
		}

		for (auto& person : humanityMaleSandyJacket.people)
		{
			if (person.actor.getRotation() == 0 && person.actor.getRotation() != 22)
			{
				//	person.actor.setRotation(8.0f);
				//	person.actor.scale(-1, 1);
			}
			//animate and move npc if not stopped

			vRectShapeDataVector.push_back(person.actor);
			if (!person.stopMove) {
				//deltaTime = npcClock.restart().asSeconds();

				//person.npcWalkSwitch = 0.2;

				npcTimeHold += npcDelta;
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
				humanityMaleSandyJacket.createBounds();
				person.UpdateNpc(0, npcDelta);
				//person.actor.setTextureRect(person.uvRect);
				person.actor.setTextureRect(person.uvRect);
			}

		}

		for (auto& person : humanityWomanSnugGrey.people)
		{
			vRectShapeDataVector.push_back(person.actor);

			//animate if not stopped
			if (!person.stopMove) {
				//deltaTime = npcClock.restart().asSeconds();

				//person.npcWalkSwitch = 0.2;

				npcTimeHold += npcDelta;
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
				humanityWomanSnugGrey.createBounds();
				person.UpdateNpc(0, npcDelta);
				//person.actor.setTextureRect(person.uvRect);
				person.actor.setTextureRect(person.uvRect);
			}


		}

		for (auto& person : humanityWomanSnugBlack.people)
		{
			vRectShapeDataVector.push_back(person.actor);
			//animate if not stopped
			if (!person.stopMove) {
				//deltaTime = npcClock.restart().asSeconds();

				//person.npcWalkSwitch = 0.2;

				npcTimeHold += npcDelta;
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
				humanityWomanSnugBlack.createBounds();
				person.UpdateNpc(0, npcDelta);
				//person.actor.setTextureRect(person.uvRect);
				person.actor.setTextureRect(person.uvRect);
			}
		}




		Point point;

		for (auto& dot : scooters.people)
		{
			//Point point;
			point.x = dot.actor.getPosition().x + (dot.actor.getSize().x / 2);
			point.y = dot.actor.getPosition().y + (dot.actor.getSize().y / 2);
			qt.insert(point);
		}

		//for (int i = 0; i <= 6; i++)
		//	for (int j = 0; j <= 6; j++)
		//	{
		//		point.x = 1 + rand() % (2500);
		//		point.y = 1 + rand() % (2000);
		//		qt.insert(point);
		//	}
		//for (int i = 0; i <= 4; i++)
		//	
		//	{
		//	qt.points.erase(qt.points.begin());
		//	}

		//std::cout << qt.points.size();

		for (auto& dog : dogGR.people)
		{

			vRectShapeDataVector.push_back(dog.actor);
			if (!dog.stopMove) {
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
		}


		for (auto& dog : scooters.people)
		{
			vRectShapeDataVector.push_back(dog.actor);
			//deltaTime = npcClock.restart().asSeconds();
			if (!dog.stopMove) {
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
		}
		for (auto& dog : scootersManSandyJacket.people)
		{
			vRectShapeDataVector.push_back(dog.actor);
			if (!dog.stopMove) {
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
		}

		for (auto& dog : drones.people)
		{
			vRectShapeDataVector.push_back(dog.actor);
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
		if (bDrones)
			drones.drawPeople(dayTime, uTime, droneTimeHold);
		if (bScooters)
			scooters.drawPeople(dayTime, uTime, scooterTimeHold);
		scootersManSandyJacket.drawPeople(dayTime, uTime, scooterTimeHold);
		if (bDogs)
			dogGR.drawPeople(dayTime, uTime, dogTimeHold);
		if (bHumans)
		{
			humanityMaleWhiteJacket.drawPeople(dayTime, uTime, npcDelta);
			humanityMaleSandyJacket.drawPeople(dayTime, uTime, npcDelta);
			humanityMaleGreen.drawPeople(dayTime, uTime, npcDelta);
			humanity.drawPeople(dayTime, uTime, npcDelta);
			humanityWomanSnugGrey.drawPeople(dayTime, uTime, npcDelta);
			humanityWomanSnugBlack.drawPeople(dayTime, uTime, npcDelta);
		}
		fShaderClock = shaderClock.getElapsedTime().asSeconds();
		sFountain.setUniform("iTime", fShaderClock);
		window->draw(sprFountain, &sFountain);

		//If Vibe Switch is turned on in the GUI, update instinct circle.
		if (player.bVibeInstinctSwitch)
			player.Update();
		vRectShapeDataVector.push_back(player.actor);
		vRectShapeDataVector.push_back(recTramBus);
		vRectShapeDataVector.push_back(recTramBus2);
		//rendering assets will order assets and add to vector.
		renderAssets();
		
		
		if(!bNpcFollow)this->window->setView(view);
		std::sort(vRectShapeDataVector.begin(), vRectShapeDataVector.end(), [](sf::RectangleShape a, sf::RectangleShape b) {return a.getPosition().y + a.getSize().y < b.getPosition().y + b.getSize().y; });
		for (auto& npcs : vRectShapeDataVector)
			this->window->draw(npcs);

	
		
		for (auto& npcs : vRectShapeDataVector)
		{
			
			this->window->draw(npcs);
		}
	//	this->window->setView(view);
	
		//this->window->draw(player.actor, &water);
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
	//	window->draw(sprTramBus);
		recTramBus.move(-8, 0);
		recTramBus2.move(6, 0);
		this->window->draw(userText);
		window->draw(chat.playerText);
		window->draw(circle);
		for (auto& vibe : player.vVibesText)
		{
			window->draw(vibe);
		}

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

		if (bAssetOneKey)
			for (auto dot : vAssetVisualAid)
			{
				this->window->draw(dot);
			}

		if (gridPath)
		{
			for (auto& npc : npcs)
				this->window->draw(npc);

		}
		{
			for (auto& npc : npcsPath)
				this->window->draw(npc);
			//	for(auto &npcs : humanity.people)
			//	this->window->draw(npcs.actor, &water);

		}

		if (bQuadTree)
		{
			qt.show(*window);
		}
		qt.cleanseTree();
		//qt.points.clear();
	//	std::cout << "begin of render pop functions\n";
		ImGui::SFML::Update(*window, clockImGui.restart());
		static std::string strengh = "Strength";
		ImGui::PushStyleColor(ImGuiCol_WindowBg, (ImVec4)ImColor::ImColor(50, 110, 110, 55));
		ImGui::Begin("Nottingham Game Simulation");
		ImGui::Checkbox("Rain", &bRain);

		ImGui::Checkbox("Path Key [Yellow]", &bPathKey);
		ImGui::Checkbox("Tree Asset Key [Green]", &bAssetOneKey);
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
		if (ImGui::Button("Tree Path Save"))
		{
			treeAssetSave();
		}
		ImGui::Checkbox("Reset PathFinding", &resetPath); //{std::cout <<resetPath };
		ImGui::Checkbox("Quad Tree", &bQuadTree);
		ImGui::Checkbox("Grid", &grid);
		ImGui::SliderFloat("Sun Light ", &dayTime, 0.0f, 1.0f);
		ImGui::Text("Experience (Based on scooter contact\nBut will later be based on missions\nand fighting etc.):");
		ImGui::Text(exper.c_str());
		{

		}

		for (auto& spy : vNPCLookingGlass) {
			npcLookingGlass(spy);
			// use similar function for multiple select?
		}

		ImGui::End();
		ImGui::PopStyleColor(1);
		ImGui::PushStyleColor(ImGuiCol_WindowBg, (ImVec4)ImColor::ImColor(50, 110, 110, 55));
		ImGui::Begin("Character Stats");
		ImGui::PopStyleColor(1);


		ImGui::Text("Character Wellness");

		ImGui::PushStyleColor(ImGuiCol_FrameBg, (ImVec4)ImColor::ImColor(50, 250, 110, 175));
		ImGui::ProgressBar(player.profile.soulhealth / 100, ImVec2(150, 15), "Soul Health");
		//ImGui::PushStyleColor(ImGuiCol_FrameBg, (ImVec4)ImColor::ImColor(50, 250, 110, 175));
		ImGui::ProgressBar(player.profile.mindhealth / 100, ImVec2(150, 15), "Mind Health");
		ImGui::PopStyleColor(1);
		ImGui::ProgressBar(player.profile.bodyhealth / 100, ImVec2(150, 15), "Body Health");
		ImGui::PushStyleColor(ImGuiCol_FrameBg, (ImVec4)ImColor::ImColor(150, 140, 250, 175));
		ImGui::Checkbox("Activate Vibe Instincts", &player.bVibeInstinctSwitch);
		ImGui::PopStyleColor(1);
		ImGui::Checkbox("Draw Instincts", &player.bvibeInstinctsDraw);
		ImGui::PushStyleColor(ImGuiCol_FrameBg, (ImVec4)ImColor::ImColor(150, 100, 80, 175));

		ImGui::ProgressBar(player.profile.powerVibeInstinct / 100, ImVec2(150, 15), "Vibe Instinct: ");

		ImGui::PopStyleColor(1);
		ImGui::SameLine(163);
		std::string labelinstinct = std::to_string(static_cast<int>(player.profile.powerVibeInstinct));
		ImGui::Text(labelinstinct.c_str());
		ImGui::SameLine(171); ImGui::Text("  /100");


		ImGui::PushStyleColor(ImGuiCol_FrameBg, (ImVec4)ImColor::ImColor(50, 90, 250, 175));

		ImGui::ProgressBar(player.profile.anxiety / 100, ImVec2(150, 15), "Anxiety");
		ImGui::PopStyleColor(1);
		ImGui::SameLine(163);
		labelinstinct = std::to_string(static_cast<int>(player.profile.anxiety));
		ImGui::Text(labelinstinct.c_str());
		ImGui::SameLine(171); ImGui::Text("  /100");

		ImGui::PushStyleColor(ImGuiCol_FrameBg, (ImVec4)ImColor::ImColor(50, 90, 250, 175));
		ImGui::ProgressBar(player.profile.rest / 100, ImVec2(150, 15), "Rest");
		ImGui::PopStyleColor(1);
		ImGui::SameLine(163);
		labelinstinct = std::to_string(static_cast<int>(player.profile.rest));
		ImGui::Text(labelinstinct.c_str());
		ImGui::SameLine(171); ImGui::Text("  /100");

		ImGui::ProgressBar(player.profile.social / 100, ImVec2(150, 15), "Social");
		ImGui::SameLine(163);
		labelinstinct = std::to_string(static_cast<int>(player.profile.social));
		ImGui::Text(labelinstinct.c_str());
		ImGui::SameLine(171); ImGui::Text("  /100");

		ImGui::ProgressBar(player.profile.hunger / 100, ImVec2(150, 15), "Style");
		ImGui::SameLine(163);
		labelinstinct = std::to_string(static_cast<int>(player.profile.style));
		ImGui::Text(labelinstinct.c_str());
		ImGui::SameLine(171); ImGui::Text("  /100");

		ImGui::ProgressBar(player.profile.style / 100, ImVec2(150, 15), "Hunger");
		ImGui::SameLine(163);
		labelinstinct = std::to_string(static_cast<int>(player.profile.hunger));
		ImGui::Text(labelinstinct.c_str());
		ImGui::SameLine(171); ImGui::Text("  /100");




		//	ImGui::PopStyleColor(1);
			//ImGui::
			//ImGui::Text(exper.c_str());
			//ImGui::DrawRect(sf::FloatRect(sf::Vector2f(3000., 7000.), (sf::Vector2f(300., 700.))), sf::Color::Blue);
		ImGui::End();
		//std::cout << "just before imgui sfml render\n";
		ImGui::SFML::Render(*window);

		if (bDroneFollow) {
			
			flyingView.setViewport(sf::FloatRect(0.8, 0, 1., 0.2));
			flyingView.setCenter(droneFollowPointer->actor.getPosition().x, droneFollowPointer->actor.getPosition().y);
			this->window->setView(flyingView);
			//this->window->draw(CharBG, &water);
			sprTramBus.setPosition(droneFollowPointer->actor.getPosition());
			
			for (auto &asset : vRectShapeDataVector)
			{
				this->window->draw(asset);
			}
			
			
			bufferDroneSwitch = false;
			bDroneFollow = true;
		}	vRectShapeDataVector.clear();
		this->window->draw(recTrees);
		 //Tell app that window is done drawing.
		this->window->display();
		if(!bNpcFollow )this->window->setView(view);
		
		this->window->clear();
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
	if (bPathKey) {
		bAssetOneKey = false;
		//std::cout << int(x/100) << ", " << int(y/100);
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


	if (bAssetOneKey)
	{
	//variable switch to check for finding, and deleting tree		
		bool found = false;
		int del=0;
		bool aidFound = false;
		int aidDel = 0;
		//delete tree from vector based on struct x and y
		for (auto& tree : vStructAssets) {
			
			if (tree.x >= x-25 && tree.x<= x+25 && tree.y >= y - 25 && tree.y <= y + 25)
			{
				found = true;
				std::cout << "found tree at this location. Removing\n";
				for (auto& vAid : vAssetVisualAid) {
					
					if (vAid.getPosition().x >= x - 25 && vAid.getPosition().x <= x +25 && vAid.getPosition().y >= y - 25 && vAid.getPosition().y <= y + 25)
					{
						aidFound = true;
						std::cout << "found Green node. Removing\n";
					}
					if (aidFound == true)break;
					aidDel++;
				}
				if (aidFound)vAssetVisualAid.erase(vAssetVisualAid.begin() + aidDel);
			}
			if (found == true) break;
			del++;
		}

		if (found) {
				vStructAssets.erase(vStructAssets.begin() + del);
			
			return;
		}
		bPathKey = false;
		bAssetOneKey = true;
		//	assetDataW.open("treedata.txt");

		sAsset tree;
		tree.x = x;
		tree.y = y;
		tree.ID = (vStructAssets.size() + 1);
		vStructAssets.push_back(tree);

		
		for (auto& tree : vStructAssets)
		{
			Rec.setPosition(tree.x, tree.y);
			Rec.setFillColor(sf::Color::Green);
			Rec.setSize(sf::Vector2f(25., 25.));
		
			vAssetVisualAid.push_back(Rec);
		}
	}
				/*if (dot.getPosition().x == x * 100 && dot.getPosition().y == y * 100)
				{
					vAssetVisualAid.erase(vPathVisualAid.begin() + loca);
					dot.setFillColor(sf::Color::Red);

				}
				loca++;*/
			

			//vPathCollide[x][y] = '0';


		//	{
		//		sf::RectangleShape dot;
		//		dot.setPosition(sf::Vector2f(x * 100, y * 100));
		//		dot.setSize(sf::Vector2f(10, 10));
		//		dot.setFillColor(sf::Color::Yellow);
		////		vPathVisualAid.push_back(dot);
		////		vPathCollide[x][y] = '1';

		//	}
		//	//pathDataW << "This is a line.\n";
		//	//pathDataW << "This is another line.\n";
			//assetDataW.close();
		


	

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


void Game::treeAssetSave()
{
	assetDataW.open("treedata.txt");
	if (assetDataW.is_open())
	{
		std::string line;
		for (auto &line : vStructAssets)
		{
			
				assetDataW << line.x << " ";
				assetDataW << line.y << " ";
				assetDataW << line.ID << '\n';

								//routefind.nodes[j * 100 + i].bObstacle = false;
				//std::cout << i << " " << j << std::endl;
			}
		assetDataW << "\n";
		}
	assetDataW.close();

	
}

void Game::renderAssets()
{
	
	
	for (auto& tree : vStructAssets)
	{
		rectTree.setPosition(tree.x, tree.y);

		vRectShapeDataVector.push_back(rectTree);
//		sprTreeAsset.setPosition(tree.x, tree.y);
		
		
		
		//this->window->draw(sprTreeAsset);

	}
}
