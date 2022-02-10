#include "socialEngine.h"
#include "Animation.h"
#include "Game.h"
#include "Population.h"
socialEngine::socialEngine(){
	//title = "";
//	astar.OnUserCreate();
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
