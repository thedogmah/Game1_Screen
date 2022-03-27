#include "socialEngine.h"
//#include "Animation.h"
#include "Game.h"
#include "Population.h"
#include <math.h>

namespace social {
	class DialogueNode;
	class DialogueTree;
	class DialogueOption;
}

socialEngine::socialEngine(social::DialogueTree* dialogue){

//	initialise images. Mustn't be local since they will go out of scope for sprites (no reference)
	
	bubbleTexture.loadFromFile("sbubble1.png");
	bubbleTexture2.loadFromFile("sbubble2.png");
	bubbleTexture3.loadFromFile("sbubble3.png");
	bubbleTexture4.loadFromFile("sbubble4.png");
	bubbleTexture5.loadFromFile("sbubble5.png");
	bubbleTexture6.loadFromFile("sbubble6.png");
	bubbleTexture7.loadFromFile("sbubble7.png");
	bubbleTexture8.loadFromFile("sbubble8.png");
	bubbleTexture9.loadFromFile("sbubble9.png");
	dialoguetreePtr = dialogue;

	HugText.setCharacterSize(150);
	HugText.setFont(interactFont);
	HugText.setFillColor(sf::Color::Yellow);
	//Initialise text labels for interact menu;
	interactFont.loadFromFile("Arial.ttf");
	InteractMenu[0].setString("Choose interaction");
	InteractMenu[1].setString("Say Hi");
	InteractMenu[2].setString("Trade");
	InteractMenu[3].setString("Private DM");
	InteractMenu[4].setString("Walk Together");
	InteractMenu[5].setString("Send Hug");

	InteractMenu[0].setFont(interactFont);
	InteractMenu[1].setFont(interactFont);
	InteractMenu[2].setFont(interactFont);
	InteractMenu[3].setFont(interactFont);
	InteractMenu[4].setFont(interactFont);
	InteractMenu[5].setFont(interactFont);

	InteractMenu[0].setFillColor(sf::Color::White);
	InteractMenu[1].setFillColor(sf::Color::White);
	InteractMenu[2].setFillColor(sf::Color::White);
	InteractMenu[3].setFillColor(sf::Color::White);
	InteractMenu[4].setFillColor(sf::Color::White);
	InteractMenu[5].setFillColor(sf::Color::White);
}


void socialEngine::interact(Animation* npc)
{

	ImGui::Begin("Interact!", &bShowInteract);
	
	std::string title;
	title = sLabelChoose + std::to_string(npc->ID);
	int pathsofar = npc->currentCount;
	int stepssofar = npc->path.size();
	std::string spath = std::to_string(pathsofar);
	std::string ssteps = std::to_string(stepssofar);
	ImGui::Text(title.c_str());
	ImGui::Text("Steps to you their location/you, and steps so far");
	ImGui::Text(spath.c_str());
	ImGui::Text(ssteps.c_str());
	ImGui::Text("Gathering Size: ");
	ImGui::SameLine(135);
	pathsofar = vInteraction.size();
	ImGui::Text(std::to_string(pathsofar).c_str());


	if (ImGui::Button("Call them over"))
	{
		if (npc->stopOverride == false)
		{
			if (partyAnchor.y == 0.0 && partyAnchor.x == 0.0) {
				partyAnchor = game->player.actor.getPosition();
				partyAnchor.x = partyAnchor.x + game->player.actor.getSize().x / 2;
				partyAnchor.y = partyAnchor.y + game->player.actor.getSize().y / 2;
				//partyGrid.setPosition(npc->actor.getPosition().x, npc->actor.getPosition().y);
				partyGrid.setFillColor(sf::Color::Transparent);
				partyGrid.setOutlineThickness(5);
				partyGrid.setOutlineColor(sf::Color::Green);
				partyGrid.setOrigin(partyGrid.getRadius(), partyGrid.getRadius());
				//	partyGrid.set
					//partyGrid.setSize(sf::Vector2f(100, 100));
					//partyGrid.setOrigin((partyGridi)
			}
			if (!npc->arrived) {
				
				npc->stopOverride = true;
				
				npc->currentImage.x = 3;
				npc->currentImage.y = 3;

				npc->hasControl = false;
				//npc->stopAnimate = true;
				npc->stopMove = true;

				//selectedClone.actor = npc->actor;

				//npc->pathSearch.OnUserCreate();

				//astar.OnUserCreate();
				npc->path.clear();
				npc->currentCount = 0;
				npc->lerpCount = 0;
				//npc->pathSearch.nodeEnd = &npc->pathSearch.nodes[5];
			//	npc->pathSearch.nodeStart = &npc->pathSearch.nodes[20];
				npc->pathSearch.nodeStart = &npc->pathSearch.nodes[int(game->player.actor.getPosition().y / 100) * npc->pathSearch.nMapWidth + (int(game->player.actor.getPosition().x) / 100)];
				//	std::cout << int(game->player.actor.getPosition().y) * npc->pathSearch.nMapWidth + (int(game->player.actor.getPosition().x));
				npc->pathSearch.nodeEnd = &npc->pathSearch.nodes[int(npc->actor.getPosition().y / 100) * npc->pathSearch.nMapWidth + (int(npc->actor.getPosition().x) / 100)];//
				npc->pathSearch.solve_AStar();
				npc->path = npc->pathSearch.OnUserUpdate(0.2f);
				//

				npc->currentImage.x = rand() % 4;

			}
			
		}
		else {
		
			if (!npc->arrived) {
				npc->stopOverride = false;

				npc->stopAnimate = false;
				npc->stopMove = false;
			}
		}
		ImGui::Text("Called Over");
	}
	


	ImGui::End();
	
	
}

void socialEngine::interactParty(std::vector<Animation*> npc)
{

	//check party has started and create time and other variables.
	if (vInteraction.size() > 0 && partyStarted == false)
	{
	partyStarted = true;
	partyTime = socialPartyTimer.restart().asSeconds();
	}
	this->game->zoomfactor = 0.85;
	std::string text{};
	ImGui::Begin("Your Squad", &bShowInteract);
	if (ImGui::Button("End Interaction")) {
		disbandParty(vInteraction);
	}

	ImGui::Text("Here's a list of everyone vibing.");
	Animation avatar;
	for (auto peeps : npc)
	{ 
		
	
		avatar.actor.setTexture(peeps->actor.getTexture());
		
		sf::Sprite character;
		
	//	character.setTexture(*npc.actor.getTexture());
		character.setTexture(*peeps->actor.getTexture());
		avatar.uvRect.left = 0;
		avatar.uvRect.height = 100;
		avatar.uvRect.top = 0;
		avatar.actor.setScale(1, 1);
		avatar.uvRect.width = peeps->actor.getSize().x  ;
		
		character.setTextureRect(peeps->uvRect);
		//people.actor.setScale(0.3, 0.3);
		//people.actor.setTextureRect(people.uvRect);
		
		text = "Member: " + std::to_string(peeps->ID);
		ImGui::Text(text.c_str());
		ImGui::Image(character);
		text = "Profession: ";
		ImGui::Text(text.c_str());
		ImGui::SameLine(170);
		/*text = "Member: " + std::to_string(people.ID);
		ImGui::Text(text.c_str());
		text = "Member: " + std::to_string(people.ID);
		//ImGui::Text(text.c_str());
*/
	}
	ImGui::End();
}

void socialEngine::disbandParty(std::vector<Animation*> npc) {
	dialoguetreePtr->end = true;
	//dialoguetreePtr->intCurrentNode = 0;
	for (auto& person : npc)
		std::cout << person->path.size() << person->stopAnimate << " " << person->stopMove;
	for (auto& person : npc)
	{
		
		int start = 180 + rand() % (4750);
		int end = 180 + rand() % (4700);
		int buffer = int(person->pathSearch.nodeEnd);
		person->path.clear();
		person->currentCount = 0;
		person->lerpCount = 0;
		//person->pathSearch.nodeStart = &person->pathSearch.nodes[buffer];
		person->pathSearch.nodeEnd = &person->pathSearch.nodes[end];//
		
		
		person->pathSearch.solve_AStar();
		person->path = person->pathSearch.OnUserUpdate(0.2f);
		std::reverse(person->path.begin(), person->path.end());
		//person->currentImage.x = rand() % 4;
	//	person->currentImage.y = rand() % 4;
		person->hasControl = true;
		
		//person->lerpCount = 0;
		person->stopAnimate = false;
		person->stopMove = false;
		person->stopOverride = false;
		person->moved = false;
		person->arrived = false;
	//	person.st
	
	}
	for (auto& person : npc)
		std::cout << "\n\nbefore clear"  << person->path.size() << person->stopAnimate << " " << person->stopMove;
	
	vInteraction.clear();

	for (auto& person : npc)
		std::cout << "\n\nCleared" << person->path.size() << person->stopAnimate << " " << person->stopMove;
	this->game->zoomfactor = 1.45;
}

void socialEngine::worldChat(std::map<std::string, std::string> clientmap)
{

	ImGui::Begin("Chat");
	for (auto& line : clientmap) {
		ImGui::Text(line.first.c_str());
		ImGui::SameLine(55);
		ImGui::Text(line.second.c_str());
	}
	ImGui::End();
}

void socialEngine::partyVisuals(std::vector<Animation*> npc)
{
	
		vPartySpeechBubbles.clear();

		
		
		for (auto& const person : npc) {
			sf::Sprite Bubble;
			Bubble.setScale(0.32, 0.32);
			if (person->vibe >= 0 && person->vibe < 2)
			{
				Bubble.setTexture(bubbleTexture);
				
			}
			else if (person->vibe  >1 && person->vibe < 4)
				Bubble.setTexture(bubbleTexture2);

			else if (person->vibe >3 && person->vibe < 6)
				Bubble.setTexture(bubbleTexture3);

			else if (person->vibe > 5 && person->vibe < 8)
				Bubble.setTexture(bubbleTexture4);
			else if (person->vibe > 7 && person->vibe < 9)
				Bubble.setTexture(bubbleTexture5);
			else if (person->vibe > 8 && person->vibe < 10)
				Bubble.setTexture(bubbleTexture6);
			else if (person->vibe > 9 && person->vibe < 12)
				Bubble.setTexture(bubbleTexture7);
			else if (person->vibe > 11 && person->vibe < 13)
				Bubble.setTexture(bubbleTexture7);
			else if (person->vibe > 12 && person->vibe < 14)
				Bubble.setTexture(bubbleTexture8);
			else
			{
				
				Bubble.setTexture(bubbleTexture9);
			}
			//Bubble.setScale(0.4, 0.4);
			int x = rand() % (2);
			int y = rand() & (2);
			
			//Bubble.setScale(0.4, 0.4);
			//Bubble.setScale(0.35, 0.35);
			Bubble.setPosition(person->actor.getPosition().x+x, person->actor.getPosition().y-50-y);
			vPartySpeechBubbles.push_back(Bubble);
		}
}

void socialEngine::drawGrid()
{
	//a multiplier variable for all grid info.
	if (partySizeChange)
	{
		
		float jj = partyGrid.getRadius() + vInteraction.size() * 10;
		float kk = partyGrid.getRadius() + vInteraction.size() * 10;
		partyGrid.setRadius((jj/2));
	}
if (vInteraction.size() > 0)	partyGrid.setPosition(vInteraction[0]->actor.getPosition().x+44, vInteraction[0]->actor.getPosition().y + 199);
	partyGrid.rotate(0.05 * vInteraction.size());
	partyGrid.setOrigin(partyGrid.getRadius(), partyGrid.getRadius());
	partyGrid.setScale(0.7, 1);
}

	


void socialEngine::drawSocial()
{
	if (partyStarted)
		partyTime = socialPartyTimer.getElapsedTime().asSeconds();
	if (partyTime >= 12){
		moveNPCs(vInteraction);
		
			partyTime = 0;
			socialPartyTimer.restart();
		}
		
//	std::cout << partyTime << " Time since last call /party social timer \n";
	game->vSocialShapeDataVector.clear();
	//moveNPCs(vInteraction);
	partyVisuals(vInteraction);
	drawGrid();
	partySizeChange = false;
	game->vSocialShapeDataVector.push_back(partyGrid);
	//this->game->window->draw(partyGrid);
	if (vPartySpeechBubbles.size()>0)
	for (auto const &bubble: vPartySpeechBubbles)
	this->game->window->draw(bubble);
}

void socialEngine::moveNPCs(std::vector<Animation*> npc)
{
	int xspace{};
	int placeline = 0;
	int line = sqrt(npc.size());
	//create array grid and fill npcs/
		for (int i = 1; i < npc.size(); i++)
	{
			xspace = 50 * i;
			npc[i]->actor.setPosition(npc[0]->actor.getPosition().x + (i * 50 ), npc[0]->actor.getPosition().y + placeline * 50  );
			std::cout << "\nitem: " << i << npc[i]->actor.getPosition().x << " and y" << npc[i]->actor.getPosition().y << '\n';
			placeline++;
			std::cout << placeline;
			if (placeline > line)
			{
				line++;
				placeline = 0; 
			}
		}
	
	//legacy code moves NPCs a few spaces
	/*for (auto const& person : npc)
	{
		int smooth = rand() % (2);
		int xx =  rand() % 81+(-40);
		int yy =  rand() % 81+(-40); 
		std::cout << smooth << "  " ;
		if (smooth == 1)
		person->actor.move(xx/2, 0);
		else
			person->actor.move(0 , yy);*/
	//}
}

void socialEngine::showTradeRequest()
{



	//std::cout << "\n\n\nReceived trade request from";// << username;
	ImGui::Begin("title");
	
	ImGui::Text("Select what you would like to trade.");
	//ImGui::Text(title.c_str());
	if (ImGui::Button("Close Trade Interaction")) {
		bShowTradeRequest = false;
	}

	

	ImGui::End();

}

void socialEngine::checkWindows()
{

	if (bShowTradeRequest) {
	
		showTradeRequest();
	}
	if (bShowServerInteract)
		serverInteract(interactPlayerPosition);
	if (bShowHug)
		serverHugReceived("");
}

void socialEngine::serverClientTrade(std::string username)
{
	
	std::cout << "\nStarted Trade request";
	std::string user;
	user = username;
	int header = 4; // trade /connection header
	sf::Packet tradepacket;
	tradepacket << header << user;
	if (client->socket.send(tradepacket) != sf::Socket::Done)
	{
		std::cout << "\nWorld update packet [header 4]not sent";
	}
	else {
		std::cout << "\nTrade requestpacket [header 4] sent. Header is: " << header << '\n';
		//	std::cout << header << " " << username << '\n';
	}
	tradepacket.clear();

}

void socialEngine::serverClientHug(std::string username)
{

	sf::Packet hugPacket;
	int header = 21;
	hugPacket << header << username;
	if (client->socket.send(hugPacket) != sf::Socket::Done)
	{
		std::cout << "\nHug failed to send\n";
	}
	else
	{
	
	}
}

void socialEngine::serverTradeRequest(std::string username)
{
	std::string title;
	title = "Trade request from " + username;

	bShowTradeRequest = true;

	


}

void socialEngine::serverInteract(sf::Vector2f position)
{
	std::string title;
	//title = "Trade request from " + username;
	interactPlayerPosition = position;
	sf::RectangleShape interactBox;

	interactBox.setFillColor(sf::Color(200,200,0,160));
	interactBox.setPosition(interactPlayerPosition.x - 20, interactPlayerPosition.y - 20);
	interactBox.setSize(sf::Vector2f(340, 330));
	game->window->draw(interactBox);
	interactBox.setFillColor(sf::Color(20, 190, 100, 160));
	interactBox.setPosition(interactPlayerPosition);
	interactBox.setSize(sf::Vector2f(300,290));
	game->window->draw(interactBox);

	InteractMenu[0].setPosition(interactBox.getPosition().x + 20, interactBox.getPosition().y + 20);
	InteractMenu[1].setPosition(interactBox.getPosition().x + 20, interactBox.getPosition().y + 45);
	InteractMenu[2].setPosition(interactBox.getPosition().x + 20, interactBox.getPosition().y + 70);
	InteractMenu[3].setPosition(interactBox.getPosition().x + 20, interactBox.getPosition().y + 95);
	InteractMenu[4].setPosition(interactBox.getPosition().x + 20, interactBox.getPosition().y + 120);
	InteractMenu[5].setPosition(interactBox.getPosition().x + 20, interactBox.getPosition().y + 145);
	

	for (auto menu : InteractMenu) {
		game->window->draw(menu);
	
	}
	
	collisionDetect();

	//call (after I create it) collission detection on above menu since the text has global scope within the social class... which may look like

	//if serverInteract == visible
		//if click on any text label
			//initiate relevant server function between two clients
}

void socialEngine::serverHugReceived(std::string username)
{
	if (username.size() > 0) {
		hugSender = username;
	}
	if (!bShowHug)
	{
		hugTimeout = hugDisplay.restart().asSeconds();
		HugText.setPosition(game->player.actor.getPosition().x - HugText.getGlobalBounds().width / 2, game->player.actor.getPosition().y + 150);
		bShowHug = true;
	}
	else {
		
		hugTimeout += hugDisplay.getElapsedTime().asSeconds();
		std::string msg;
		msg = hugSender + " Sent a Hug!";
		HugText.setString(msg);
		HugText.setPosition(game->player.actor.getPosition().x - HugText.getGlobalBounds().width / 2, HugText.getPosition().y - 1);
		game->window->draw(HugText);
		std::cout << hugTimeout;
		HugText.setFillColor(sf::Color(0 + rand() % 255, 0 + rand() % 255, 0 + rand() % 255, 70 + rand() %135));
		if (hugTimeout > 300)
		{
			bShowHug = false;
			hugTimeout = 0;
		}
	}
}

void socialEngine::collisionDetect()
{
	//do collision detect on all active UI's based on bool switches
	sf::Vector2i XY = sf::Mouse::getPosition(*game->window);
	//int nSelectedNodeY = sf::Mouse::getPosition(window) ;


	sf::Vector2f worldPos = game->window->mapPixelToCoords(XY, game->view);
	//collision detection for serverInteract menu
	if (bShowServerInteract)
	{
		for (auto& menu : InteractMenu)
		{
			if (menu.getGlobalBounds().contains(worldPos))
			{
				menu.setFillColor(sf::Color::Yellow);
				//std::cout << "Mouse Over " << menu.getString().toAnsiString();

			}
			else
			{
				menu.setFillColor(sf::Color::White);
			}
		}if (optMouseLocation && socialTakeClicks)
			for (auto& menu : InteractMenu)
			{

				{
					sf::Vector2f worldPoss = game->window->mapPixelToCoords(sf::Vector2i(optMouseLocation.value()), game->view);
					std::cout << "\nOptional mouse location true:\n";

					//Check whether 'Trade has been clcicked by client
					if (menu.getGlobalBounds().contains(worldPoss) && menu.getString() == "Trade")
					{
						serverClientTrade(serverUsername);
						std::cout << "\nClick";
				
						//turn UI off and its ability to receive clicks
						bShowServerInteract = false;
						socialTakeClicks = false;
					}
					//Check whether send hug has been clicked by client
					if (menu.getGlobalBounds().contains(worldPoss) && menu.getString() == "Send Hug")
					{
					//Code for sending hug.
						serverClientHug(serverUsername);
						bShowServerInteract = false;
						socialTakeClicks = false;
					}
				}
			}
		else
			socialTakeClicks = true;
		//HERE
		
			
		
	}
}

void socialEngine::socialReset()
{
	optMouseLocation.reset();
}
