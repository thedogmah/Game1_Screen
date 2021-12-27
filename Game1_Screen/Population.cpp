#include "Population.h"

Population::Population()
{
}

Population::Population(int cpeopleAmount, int x, int y, sf::RenderWindow &window)
{
	this->peopleAmount = 50;
	this->coordBounds.x = x;
	this->coordBounds.y = y;
	this->populate();
	}

bool Population::populate()
{
	for (int x = 0; x < peopleAmount/*(this->peopleAmount)*/; x++) {
		//Put this in a loop or create some map for all textures for when using different characters, not from the same file*
		Human.ID = x;
		int start = 780 +rand() %  (600);
		int end = 780 +rand() %  (600);
		imgHuman.loadFromFile("WomanWalking4.png");
		imgHuman.createMaskFromColor(sf::Color::Black);
		if (!texHuman.loadFromImage(imgHuman))
		{
			std::cout << "Human not loaded";
		}
		Human.actor.setTexture(&texHuman);
		Human.actor.setSize(sf::Vector2f(97, 200));
		Human.actor.setPosition(500.0f, 500.0f);
		Human.pathSearch.OnUserCreate();
		Human.pathSearch.nodeStart = &Human.pathSearch.nodes[start];
		Human.pathSearch.nodeEnd = &Human.pathSearch.nodes[end];//
		
		Human.pathSearch.solve_AStar();
		Human.imageCount = sf::Vector2u(4, 4);
		Human.path = Human.pathSearch.OnUserUpdate(0.0f);
		Human.uvRect.width = texHuman.getSize().x / float(Human.imageCount.x);
		Human.uvRect.height = texHuman.getSize().y / float(Human.imageCount.y);

		people.push_back(Human);
	}
	return true;
}

bool Population::drawPeople()
{
	//class should make use of the parameters passed to constructor to draw that amount.
	for (auto &person : people)
	{

		person.actor.setSize(sf::Vector2f(97, 200));
		person.eFacing = Animation::eDirectionFacing::East;
		//
		person.currentImage.x = 0;
		person.npcStepTime = 0.14f;
		
	person.actor.setTexture(&texHuman);
		
		populationDeltaTime = populationClock.restart().asSeconds();
		//populationDeltaTime = populationClock.restart().asSeconds();
		
		person.uvRect.width = texHuman.getSize().x / float(person.imageCount.x);
		person.uvRect.height = texHuman.getSize().y / float(person.imageCount.y);
		person.Update(0, populationDeltaTime, false, false, false, false);
	//	std::cout << person.currentCount << " : " << person.pathSearch.path.size() << "\n";
		person.actor.setTextureRect(person.uvRect);
	//	std::cout << person.path.size() << "\n";
	//	std::cout << person.npcTotalTime << "| total < step > " << person.npcStepTime<< "\n";
		window->draw(person.actor);
		if (person.currentCount >= person.path.size() - 1) {
			person.pathSearch.nodeStartBuffer = person.pathSearch.nodeStart;
			person.pathSearch.nodeEndBuffer = person.pathSearch.nodeEnd;
			person.pathSearch.nodeEnd = person.pathSearch.nodeStartBuffer;
			person.pathSearch.nodeStart = person.pathSearch.nodeEndBuffer;
			person.currentCount = 0;
			person.pathSearch.solve_AStar();
			person.path = person.pathSearch.OnUserUpdate(0.2f);
		
		}
	}
	return true;
}
