#include "socialEngine.h"
#include "Animation.h"
#include "Game.h"
#include "Population.h"
#include <math.h>
socialEngine::socialEngine(){

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
	title = "";
}

void socialEngine::interactParty(std::vector<Animation*> npc)
{

	//check party has started and create time and other variables.
	if (vInteraction.size() > 0 && partyStarted == false)
	{
	partyStarted = true;
	partyTime = socialPartyTimer.restart().asSeconds();
	}
	this->game->zoomfactor = 0.7;
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
	this->game->zoomfactor = 1;
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
