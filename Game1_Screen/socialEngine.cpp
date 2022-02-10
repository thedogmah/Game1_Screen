#include "socialEngine.h"
#include "Animation.h"
#include "Game.h"
#include "Population.h"
socialEngine::socialEngine(){
	//title = "";
//	astar.OnUserCreate();

	//vInteraction = new Animation();
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
			npc->stopOverride = true;

			npc->currentImage.x = 3;			
			npc->currentImage.y = 3;
			
			npc->hasControl = false;
			//npc->stopAnimate = true;
			//npc->stopMove = true;
			
			//selectedClone.actor = npc->actor;
		
			//npc->pathSearch.OnUserCreate();

			//astar.OnUserCreate();
			npc->path.clear();
			npc->currentCount = 0;
			//npc->pathSearch.nodeEnd = &npc->pathSearch.nodes[5];
		//	npc->pathSearch.nodeStart = &npc->pathSearch.nodes[20];
			npc->pathSearch.nodeStart = &npc->pathSearch.nodes[int(game->player.actor.getPosition().y /100)  * npc->pathSearch.nMapWidth +(int(game->player.actor.getPosition().x) /100) ];
		//	std::cout << int(game->player.actor.getPosition().y) * npc->pathSearch.nMapWidth + (int(game->player.actor.getPosition().x));
			npc->pathSearch.nodeEnd = &npc->pathSearch.nodes[int(npc->actor.getPosition().y / 100) * npc->pathSearch.nMapWidth + (int(npc->actor.getPosition().x) / 100)];//
			npc->pathSearch.solve_AStar();
			npc->path = npc->pathSearch.OnUserUpdate(0.2f);
	//

			npc->currentImage.x = rand() % 4;

			}
		else {
		
			
			npc->stopOverride = false;

			npc->stopAnimate = false;
			npc->stopMove = false;

		}
		ImGui::Text("Called Over");
	}
	


	ImGui::End();
	title = "";
}

void socialEngine::interactParty(std::vector<Animation*> npc)
{
	std::string text{};
	ImGui::Begin("Your Squad", &bShowInteract);
	if (ImGui::Button("End Interaction")) {
		disbandParty(vInteraction);
	}

	ImGui::Text("Here's a list of everyone vibing.");
	
	for (auto peeps : npc)
	{ 
		Animation avatar;
	
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
}
