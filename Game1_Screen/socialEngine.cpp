#include "socialEngine.h"
#include "Animation.h"
#include "Game.h"
socialEngine::socialEngine(){
	//title = "";
}

void socialEngine::interact(Animation* npc)
{

	ImGui::Begin("Interact!", &bShowInteract);
	std::string title;
	title = sLabelChoose + std::to_string(npc->ID);
	ImGui::Text(title.c_str());

	if (ImGui::Button("Call them over"))
	{

		//		npc->stopAnimate = true;
			//	npc->stopMove = true;
		std::cout << "\nOverride is: " << npc->stopOverride << '\n' << "\nNodes are :" << npc->pathSearch.nodeStart->x << ", "
			<< npc->pathSearch.nodeStart->y << '\n' << npc->pathSearch.nodeEnd->x << ", " << npc->pathSearch.nodeEnd->y << '\n';
		//Give character new destination.
		if (npc->stopOverride == false)
		{
			npc->stopAnimate = true;
			npc->stopMove = true;
			npc->stopOverride = true;
			npc->path.clear();
			npc->pathSearch.OnUserCreate();
			npc->pathSearch.nodeStart = &npc->pathSearch.nodes[(int(game->player.actor.getPosition().y/100)) * npc->pathSearch.nMapWidth + (int(game->player.actor.getPosition().x/100))];
			npc->pathSearch.nodeEnd = &npc->pathSearch.nodes[(int(game->player.actor.getPosition().y/100)) * npc->pathSearch.nMapWidth + (int(game->player.actor.getPosition().x/100))];
			std::cout << "\nOverride is: " << npc->stopOverride << '\n' << "\nNodes are :" << npc->pathSearch.nodeStart->x << ", "
				<< npc->pathSearch.nodeStart->y << '\n' << npc->pathSearch.nodeEnd->x << ", " << npc->pathSearch.nodeEnd->y << '\n' <<npc->currentCount << "<<current count" << npc-> lerpCount <<"lerp count";
			npc->pathSearch.solve_AStar();
			
			npc->path = npc->pathSearch.OnUserUpdate(0.2f);
			npc->currentCount = 0;
			npc->lerpCount = 0;
			std::cout << "\ncurrent count" << npc->currentCount << ", Lerp Count: " << npc->lerpCount << "\nPath size" << npc->path.size();
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
