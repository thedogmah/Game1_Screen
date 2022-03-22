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
#include "clientside.h"
namespace social {
	class DialogueNode;
	class DialogueTree;
	class DialogueOption;
}

class Game;
class DialogueTree;
class socialEngine
{

	friend class Game;
public:
	std::string serverUsername{};
	socialEngine(social::DialogueTree* dialogue);
	bool bShowInteract = false;
	bool bShowTradeRequest = false;
	bool bShowServerInteract = false;
	bool socialTakeClicks = false;
	// bool bSocialClicked = false;
	std::optional<sf::Vector2f>optMouseLocation;
	//menu resources
	sf::Vector2f interactPlayerPosition;
	sf::Text InteractMenu[6];
	sf::Font interactFont;
	//aStar astar{};
	Game* game;
	clientside* client;
	social::DialogueTree* dialoguetreePtr;
	void interact(Animation* npc);
	void interactParty(std::vector<Animation*> npc);
	void disbandParty(std::vector<Animation*> npc);
	void worldChat(std::map<std::string, std::string> clientmap);
	void partyVisuals(std::vector<Animation*> npc);
	void drawGrid();
	void drawSocial();
	void moveNPCs(std::vector<Animation*> npc);
	void showTradeRequest();
	void checkWindows();
	void serverClientTrade(std::string username);
	void serverTradeRequest(std::string username);
	void serverInteract(sf::Vector2f position);

	void collisionDetect(); 
	void socialReset();
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

namespace social {
	class DialogueNode;
	

	class DialogueOption {

	public:

		DialogueOption(std::string Text, int ReturnCode, DialogueNode* parentNode, DialogueNode* NextNode);
		std::string text;
		int returncode;
		DialogueNode* nextNode;
		DialogueNode* parentNode;
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

		//visuals
		sf::Text nodetext;
		sf::Font fontname;
		sf::RectangleShape chatbox;
		
		std::vector<sf::Text> vecText;

		//int intCurrentNode{};
		int intOption{};
		sf::Text optiontext;
		int performDialogue(sf::RenderWindow* dwindow, Animation* npc);
		void dialogueSelect(social::DialogueNode* currentnode, int option);
		//socialEngine* socialengine;
		
		DialogueNode* currentnode;
		DialogueNode* resetnode; //this address will always hold some form of node0, where as currentnode may ocassionaly hold nullptr (to end the interaction)
		bool end = false;
		std::vector<DialogueNode*> dialogueNodes;


		
	};

	inline DialogueTree::DialogueTree() {
		fontname.loadFromFile("Arial.ttf");
		
		nodetext.setFont(fontname);
		chatbox.setFillColor(sf::Color(0, 0, 0, 200));
		chatbox.setSize(sf::Vector2f(120, 200));
	}

	inline void DialogueTree::init() {
		
		DialogueNode* node0 = new DialogueNode("Hi, are you wanting directions?");
		DialogueNode* node1 = new DialogueNode("Come back if I can help you.");
		DialogueNode* node2 = new DialogueNode("I can help you get to St Marys or The Castle");
		DialogueNode* node3 = new DialogueNode("For the castle, follow the Tram past starbucks then its signed!");
		DialogueNode* node4 = new DialogueNode("For St Marys, follow the tram up the hill then its signed.");
		DialogueNode* node5 = new DialogueNode("Not exactly what I meant!");
		//	DialogueNode* node4 = new DialogueNode;
		
		//initialise starting nodeSt 
		currentnode = node0;
		resetnode = node0;
			//Node 0
		node0->dialogueOptions.push_back(DialogueOption("No, sorry.", 0, node0, node1));
		node0->dialogueOptions.push_back(DialogueOption("Yes!", 0, node0, node2));
		node0->dialogueOptions.push_back(DialogueOption("I don't know what to do with my life", 0, node0, node5));

		dialogueNodes.push_back(node0);

		//Node 1
		node1->dialogueOptions.push_back(DialogueOption("Got it!", 0, node1, nullptr));
		dialogueNodes.push_back(node1);

		// Node 2
		node2->dialogueOptions.push_back(DialogueOption("The Castle", 0, node2, node3));
		node2->dialogueOptions.push_back(DialogueOption("St Marys", 0, node2, node4));
		node2->dialogueOptions.push_back(DialogueOption("Back up a sec...", 0, node2, node0));
		dialogueNodes.push_back(node2);

		//Node 3
		node3->dialogueOptions.push_back(DialogueOption("Got it", 1, node3, nullptr));
		dialogueNodes.push_back(node3);

		//Node 4
		node4->dialogueOptions.push_back(DialogueOption("Thanks", 1, node4, nullptr));
		dialogueNodes.push_back(node4);

		//Node 5
		node5->dialogueOptions.push_back(DialogueOption("...Helpful", 1, node5, nullptr));
		dialogueNodes.push_back(node5);
	}

	inline void DialogueTree::destroyTree() {

	}


	inline int DialogueTree::performDialogue(sf::RenderWindow* dwindow, Animation* npc) {
		
		
		if (dialogueNodes.empty() || currentnode == nullptr)
		{
			

			currentnode = resetnode;
			intOption = 0;
			end = true;
			return -1;
		
		}
		sf::Vector2f position = npc->actor.getPosition();
		DialogueNode* currentNode = currentnode;
		int width;
		
		dwindow->draw(chatbox);
		nodetext.setString(currentNode->text);
		nodetext.setPosition(npc->actor.getPosition().x - 100 , npc->actor.getPosition().y - 50);
		nodetext.setCharacterSize(16);
		nodetext.setFillColor(sf::Color::White);
		dwindow->draw(nodetext);
		optiontext.setPosition(nodetext.getPosition());
		optiontext.setCharacterSize(15);
		width = nodetext.getLocalBounds().width;
		width = width + 60;

		//clear the options vector before checking collide in the game.cpp functions
		vecText.clear();
		
		for (int i = 0; i < currentNode->dialogueOptions.size(); i++)
		{
			optiontext.setFont(fontname);
			
			optiontext.setFont(fontname);
			optiontext.setString(currentNode->dialogueOptions[i].text);
			optiontext.setPosition(optiontext.getPosition().x,optiontext.getPosition().y + 25);
			optiontext.setFillColor(sf::Color::Green);
			dwindow->draw(optiontext);
			vecText.push_back(optiontext);
		if (optiontext.getLocalBounds().width > width)
			width = nodetext.getLocalBounds().width;
		}
		chatbox.setSize(sf::Vector2f(width, chatbox.getSize().y));
		chatbox.setPosition(position);
		chatbox.move(-100, -50);
		chatbox.setSize(sf::Vector2f(width, 50 + (25 * currentNode->dialogueOptions.size())) );
		


		return 1;
	}

	inline void DialogueTree::dialogueSelect(social::DialogueNode* parentnode, int option) {
		intOption = option;
		currentnode = currentnode->dialogueOptions[option].nextNode;
		
	}
	inline DialogueOption::DialogueOption(std::string Text, int ReturnCode, DialogueNode* parentnode, DialogueNode* NextNode) {

		text = Text;
		returncode = ReturnCode;
		nextNode = NextNode;
		parentNode = parentnode;
	}


	inline DialogueNode::DialogueNode(std::string Text) {

		text = Text;
	}


}