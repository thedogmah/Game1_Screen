#include "Population.h"

Population::Population()
{
//	std::shared_ptr<sf::Texture> texture;
	//texture->loadFromFile("WomanWalking4_2.png")
	imgHuman.loadFromFile("WomanWalking4_2.png");


	
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
	water.loadFromFile("sWater.txt", sf::Shader::Fragment);

	water.setUniform("u_resolution", sf::Vector2f(1710.0, 1070.0));

	water.setUniform("u_mouse", sf::Vector2f(sf::Mouse::getPosition()));

		

		//water.loadFromFiI'mfaorle("sWater.txt", sf::Shader::Fragment);

		imgHuman.createMaskFromColor(sf::Color::Black);
		if (!texHuman.loadFromImage(imgHuman))
		{
			std::cout << "Human not loaded";
		}

		Human.actor.setTexture(&texHuman);
		Human.actor.setSize(sf::Vector2f(97, 200));
		Human.actor.setPosition(4500.0f, 1000.0f);
		Human.pathSearch.OnUserCreate();
		Human.imageCount = sf::Vector2u(4, 4);

		Human.uvRect.width = texHuman.getSize().x / float(Human.imageCount.x);
		Human.uvRect.height = texHuman.getSize().y / float(Human.imageCount.y);
		for (int x = 0; x < peopleAmount/*(this->peopleAmount)*/; x++) {

			//

			//Put this in a loop or create some map for all textures for when using different characters, not from the same file*
			Human.ID = x;
			int start = 780 + rand() % (2750);
			int end = 880 + rand() % (2700);
			//imgHuman.loadFromFile("WomanWalking4.png");

			Human.pathSearch.nodeStart = &Human.pathSearch.nodes[start];
			Human.pathSearch.nodeEnd = &Human.pathSearch.nodes[end];//

			Human.pathSearch.solve_AStar();
			Human.path = Human.pathSearch.OnUserUpdate(0.2f);

			//std::cout << Human.imageCount.x << ", y image count is " << Human.imageCount.y;
			people.push_back(Human);
		}
	
	return true;
}

bool Population::drawPeople(float dayTime, float uTime, float deltaTime)
{
	dayTime += 0.5;
	water.setUniform("u_time", uTime);
	water.setUniform("dayTime", dayTime);

	
	//class should make use of the parameters passed to constructor to draw that amount.
	for (auto &person : this->people)
	{
	//	person.actor.setSize(sf::Vector2f(97, 200));
	//	person.eFacing = Animation::eDirectionFacing::East;
		//
		//person.currentImage.x = 0;
		//person.npcStepTime = 0.14f;
		
	//	person.actor.setTexture(&texHuman);
		
		
		//populationDeltaTime = populationClock.restart().asSeconds();
		//person.path = person.pathSearch.OnUserUpdate(0.2f);
		person.UpdateNpc(0, deltaTime);
	//	std::cout << person.currentCount << " : " << person.pathSearch.path.size() << "\n";
		//person.actor.setTextureRect(person.uvRect);
	//	std::cout << person.path.size() << "\n";
	//	std::cout << person.npcTotalTime << "| total < step > " << person.npcStepTime<< "\n";

		if (this->rt.contains(person.actor.getPosition().x, person.actor.getPosition().y) )
		window->draw(person.actor, &water);
		if (person.currentCount >= person.path.size() - 1) {
			std::reverse(person.path.begin(), person.path.end());
			
			/*person.pathSearch.nodeStartBuffer = person.pathSearch.nodeStart;
			person.pathSearch.nodeEndBuffer = person.pathSearch.nodeEnd;
			person.pathSearch.nodeEnd = person.pathSearch.nodeStartBuffer;
			person.pathSearch.nodeStart = person.pathSearch.nodeEndBuffer;*/
			person.currentCount = 0;
			/*person.pathSearch.solve_AStar();
			person.path.clear();
			person.path = person.pathSearch.OnUserUpdate(0.2f);*/
		
		}
	}
	return true;
}

void Population::createBounds()
{

	this->rt.left = this->window->getView().getCenter().x - this->window->getSize().x / 2.f - 100;
	this->rt.top = this->window->getView().getCenter().y - this->window->getSize().y / 2.f - 250;
	this->rt.width = this->window->getSize().x + 100;
	this->rt.height = this->window->getSize().y + 250;

}