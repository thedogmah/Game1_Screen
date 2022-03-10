#pragma once
#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <SFML/Audio.hpp>
#include <SFML/Network.hpp>

#include "imgui/imgui.h"
#include "imgui/imgui-SFML.h"
#include <string>
#include <iostream>
#include <sstream>
#include <fstream>
#include <istream>
#include <vector>
//#include "Animation.h"
#include "aStar.h"
#include "Population.h"
//#include "Game.h"

class Game;
class DialogueNode;

class socialEngine
{
public:

	socialEngine();
	bool bShowInteract = false;
	//aStar astar{};
	Game* game;
	void interact(Animation* npc);
	void interactParty(std::vector<Animation*> npc);
	void disbandParty(std::vector<Animation*> npc);
	void worldChat(std::map<std::string, std::string> clientmap);
	void partyVisuals(std::vector<Animation*> npc);
	void drawGrid();
	void drawSocial();
	void moveNPCs(std::vector<Animation*> npc);
	//get the currently selected npc
	Animation* selectedNpc;
	//Animation selectedClone;
	Population worldBase;
	std::string sLabelChoose = "Choose interaction with: ";
	Animation* vInteract;

	//resources for drawing to the world
	//receives pointers to each member in the current social gathering
	//Is use to create text based on its size (text per person) and uses their location, in addition to drawing their mug shot on the appropriate UIs.
	std::vector<Animation*> vInteraction;

	std::vector<sf::Sprite> vPartySpeechBubbles;

	//image resources
	sf::Texture bubbleTexture;
	sf::Texture bubbleTexture2;
	sf::Texture bubbleTexture3;
	sf::Texture bubbleTexture4;
	sf::Texture bubbleTexture5;
	sf::Texture bubbleTexture6;
	sf::Texture bubbleTexture7;
	sf::Texture bubbleTexture8;
	sf::Texture bubbleTexture9;

	//dynamic bool + party dynamics / variables
	bool partySizeChange = false;
	bool partyStarted = false;
	sf::CircleShape partyGrid;
	sf::Vector2f partyAnchor;
	sf::Clock socialPartyTimer;
	float deltaTime;
	float partyTime;

};

	
	class DialogueOption {

	public:

		DialogueOption(std::string Text, int ReturnCode, DialogueNode* NextNode); 
		std::string text;
		int returncode;
		DialogueNode* nextNode;
	};

	class DialogueNode {

	public:
		DialogueNode(std::string text);
		std::string text;
		std::vector <DialogueOption> dialogueOptions;

	};

	class DialogueTree
	{
	public:
		DialogueTree();

		void init();
		void destroyTree();

		int performDialogue();

	private:

		std::vector<DialogueNode*> dialogueNodes;

	};

	DialogueTree::DialogueTree() {
	
	}

	void DialogueTree::init() {
	
		DialogueNode* node0 = new DialogueNode("Hi, are you wanting directions?");
		DialogueNode* node1 = new DialogueNode("Come back if I can help you.");
		DialogueNode* node2 = new DialogueNode("I can help you get to St Marys of The Castle");
		DialogueNode* node3 = new DialogueNode("For the castle, you need to follow the Tram, then...");
	//	DialogueNode* node4 = new DialogueNode;

		//Node 0
		node0->dialogueOptions.push_back(DialogueOption("No, sorry.", 0, node1));
		node0->dialogueOptions.push_back(DialogueOption("Yes!", 0, node2));
		dialogueNodes.push_back(node0);
		
		//Node 1
		node1->dialogueOptions.push_back(DialogueOption("No, sorry.", 0, nullptr));
		dialogueNodes.push_back(node1);
		
		// Node 2
		node2->dialogueOptions.push_back(DialogueOption("The Castle", 0, node3));
		dialogueNodes.push_back(node2);
		
		//Node 3
		node3->dialogueOptions.push_back(DialogueOption("Got it", 1, nullptr));
		dialogueNodes.push_back(node3);
	}

	void DialogueTree::destroyTree() {
	
	}


	int DialogueTree::performDialogue() {

		if (dialogueNodes.empty())
		{		return -1;
		}

		DialogueNode* currentNode = dialogueNodes[0];

		std::cout << currentNode->text << "\n";
	}

	DialogueOption::DialogueOption(std::string Text, int ReturnCode, DialogueNode* NextNode) {
	
		text = Text;
		returncode = ReturnCode;
		nextNode = NextNode;
	}


	DialogueNode::DialogueNode(std::string Text) {

		text = Text;
	}


