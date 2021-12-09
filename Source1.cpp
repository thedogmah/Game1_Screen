
#include <SFML/Graphics.hpp>
#include <SFML/Network.hpp>
#include <SFML/Window.hpp>
#include <SFML/System.hpp>
#include <SFML/Config.hpp>

#include <iostream>
#include <vector>
#include <string>

void ThreadFunction() {
	for (int i = 0; i < 200000; i++)
		std::cout << "Thread function " << i << std::endl;
}

int main() {

	sf::IpAddress ip = sf::IpAddress(sf::IpAddress::LocalHost);
	std::string publicIP = ip.toString();
	sf::TcpSocket socket;
	bool done = false;
	std::string id;
	std::string text = "";

	//sf::Thread Ryan(&ThreadFunction);
	//Ryan.launch();
	//for (int i = 0; i < 200000; i++)
	//	std::cout << "MAIN function " << i*10 << std::endl;

	std::cout << "Enter online id: ";
	std::cin >> id;

	ip = sf::IpAddress(publicIP);
	socket.connect(ip, 2000);

	sf::RenderWindow Window(sf::VideoMode(800, 600, 32), "Chat");
	
	std::vector<sf::Text> chat;

	sf::Packet packet;
	packet << id;
	socket.send(packet);
	socket.setBlocking(false);

	Window.setTitle(id);

	sf::Font font;
	font.loadFromFile("Arial.ttf");
	while (Window.isOpen())
	{
		sf::Event event;
		while (Window.pollEvent(event))
		{

			switch (event.type)
			{
			case sf::Event::Closed:
				Window.close();
				break;
			case sf::Event::KeyPressed:
				if (event.key.code == sf::Keyboard::Escape)
					Window.close();
				else if (event.key.code == sf::Keyboard::Return)
				{
					sf::Packet packet;
					packet << id + ": " + text;
					socket.send(packet);
					sf::Text displayText(text, font, 20);
					displayText.setFillColor(sf::Color::Red);
					chat.push_back(displayText);
					text = "";
				}
				break;
			case sf::Event::TextEntered:
				text += event.text.unicode;
				break;
			}

		}
		
		sf::Packet packet;
		socket.receive(packet);

		std::string temptext;
		if (packet >> temptext)
		{
			sf::Text displayText(temptext, font, 20);
			displayText.setFillColor(sf::Color::Blue);
			chat.push_back(displayText);
		}
	
		int i = 0;
		for (i; i < chat.size(); i++)
		{
			chat[i].setPosition(0, i * 20);
			Window.draw(chat[i]);

		}

		sf::Text drawText(text, font, 20);
		drawText.setFillColor(sf::Color::White);
		drawText.setPosition(0, i * 20);
		Window.draw(drawText);

		Window.display();
		Window.clear();

	}
	return 0;

}