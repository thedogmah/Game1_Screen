#include "Population.h"

Population::Population()
{
}

Population::Population(int cpeopleAmount, int x, int y, sf::RenderWindow &window)
{
	this->peopleAmount = cpeopleAmount;
	this->coordBounds.x = x;
	this->coordBounds.y = y;
	this->populate();

}

bool Population::populate()
{
//	for (int x = 0; x < (this->peopleAmount); x++) {
		//Put this in a loop or create some map for all textures for when using different characters, not from the same file*
		imgHuman.loadFromFile("WomanWalking4.png");
		imgHuman.createMaskFromColor(sf::Color::Black);
		if (!texHuman.loadFromImage(imgHuman))
		{
			std::cout << "Human not loaded";
		}
		Human.actor.setTexture(&texHuman);
		Human.actor.setSize(sf::Vector2f(97, 200));
		Human.actor.setPosition(500.0f, 500.0f);
		aStarPath.OnUserCreate();
		aStarPath.nodeStart = &aStarPath.nodes[20];
		aStarPath.nodeEnd = &aStarPath.nodes[150];//
		aStarPath.solve_AStar();

		Human.path = aStarPath.OnUserUpdate(0.0f);
		Human.uvRect.width = texHuman.getSize().x / float(Human.imageCount.x);
		Human.uvRect.height = texHuman.getSize().y / float(Human.imageCount.y);

		people.push_back(Human);
	//}
	return true;
}

bool Population::drawPeople()
{
	//class should make use of the parameters passed to constructor to draw that amount.
	for (auto person : people)
	{

		person.actor.setSize(sf::Vector2f(97, 200));
		person.eFacing = Animation::eDirectionFacing::East;
		person.imageCount = sf::Vector2u(4, 4);
		person.currentImage.x = 0;
		person.npcStepTime = 0.14f;
		
		//person.actor.setTexture(&texHuman);
		
	
		populationDeltaTime = populationClock.restart().asSeconds();
		person.path = aStarPath.OnUserUpdate(0.0f);
		person.uvRect.width = texHuman.getSize().x / float(person.imageCount.x);
		person.uvRect.height = texHuman.getSize().y / float(person.imageCount.y);
		//person.Update(0, populationDeltaTime, true, false, false, false);
		
		person.actor.setTextureRect(person.uvRect);
		std::cout << person.path.size() << "\n";
		std::cout << person.npcTotalTime << "| total < step > " << person.npcStepTime<< "\n";
		window->draw(person.actor);
	}
	return true;
}
