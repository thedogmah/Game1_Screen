#include <iostream>
#include "Network.h"
#include "Game.h"
 void sfmlconnect() {	/*std::cout << "Enter (s) for Server, Enter (c) for client:" << std::endl;
	std::cin >> connectionType;

	if (connectionType == 's')
	{

		sf::TcpListener listener;
		listener.listen(2000);
		listener.accept(socket);
		text += "Server";
		mode = 's';
	}

	else if (connectionType == 'c')
	{
		socket.connect(ip, 2000);
		text += "Client";
		mode = 'r';
	}

	socket.send(text.c_str(), text.length() + 1);
	socket.receive(buffer, sizeof(buffer), received);
	std::cout << buffer << std::endl;

	bool done = true;

	while (!done)
	{
		if (mode == 's')
		{
			std::getline(std::cin, text);
			if (text == "x") {
			done = true;
		}
			socket.send(text.c_str(), text.length() + 1);
			mode = 'r';
		}

		else if (mode == 'r')
		{
			socket.receive(buffer, sizeof(buffer), received);
			if (received > 0)
			{
				std::cout << "Received: " << buffer << std::endl;
				mode = 's';
			}
		}
	}*/
	sf::IpAddress ip = sf::IpAddress::getLocalAddress();
	sf::TcpSocket socket;
	char connectionType;

	std::cout << "Enter unique player name: ";
	std:: cin >> unique_player_name;
	std::cout << "\nGot ya, " << unique_player_name;
std::cout << " s for server, c for client: ";
	std::cin >> connectionType;

	if (connectionType == 's')
	{
		sf::TcpListener listener;
		listener.listen(2000);
		listener.accept(socket);
	}

	else {
		socket.connect(ip, 2000);
		sf::Sprite actor;
		Characters.push_back(actor);
		
	}
	sf::RectangleShape rect1, rect2;

	rect1.setSize(sf::Vector2f(20, 20));
	rect2.setSize(sf::Vector2f(20, 20));

	rect1.setFillColor(sf::Color::Green);
	rect2.setFillColor(sf::Color::Red);


	sf::RenderWindow Window2(sf::VideoMode(800, 600, 32), "Packets");


	sf::Vector2f prevPosition, p2Position;

	socket.setBlocking(false);

	while (Window2.isOpen())
	{
		sf::Event Event;
		while (Window2.pollEvent(Event)) {
			if (Event.type == sf::Event::Closed || Event.key.code == sf::Keyboard::Escape)
				Window2.close();
		}

		sf::Packet packet;

		if (prevPosition == rect1.getPosition()) {

			packet << unique_player_name ;
			socket.send(packet);
		}

		
		socket.receive(packet);
		if (packet >> unique_player_name )
		{
			unique_players.push_back(unique_player_name);
			for (auto a : unique_players) 
			{
				//std::cout << unique_players << std::endl;
				std::cout << a;
			}
		}
		Window2.draw(rect1);
		Window2.draw(rect2);
		Window2.display();
	}
}
	