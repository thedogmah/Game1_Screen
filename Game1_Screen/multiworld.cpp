#include "multiworld.h"
#include <iostream>
#include <SFML/Network.hpp>
#include <string>

void multi() {
	std::cout << "Server Running" << std::endl;

	mListener.listen(2000);
	mSelector.add(mListener);

	while (!done)
	{
		if (mSelector.wait())
		{

			if (mSelector.isReady(mListener)) {
				sf::TcpSocket* socket = new sf::TcpSocket;
				mListener.accept(*socket);
				sf::Packet packet;
				std::string id;

				if (socket->receive(packet) == sf::Socket::Done)
					packet >> id;

				std::cout << id << " has connected to the chat room" << std::endl;
				clients.push_back(socket);
				mSelector.add(*socket);
			}
			else
			{
				for (int i = 0; i < clients.size(); i++)
				{
					if (mSelector.isReady(*clients[i]))
					{
						sf::Packet packet, sendPacket;
						if (clients[i]->receive(packet) == sf::Socket::Done)
						{
							std::string text;
							packet >> text;
							sendPacket << text;
							for (int j = 0; j < clients.size(); j++)
							{
								if (i != j)
								{
									clients[j]->send(sendPacket);

								}
							}


						}

					}
				}
			}
		}
	}
	for (std::vector<sf::TcpSocket*>::iterator it = clients.begin(); it != clients.end(); it++)
		delete* it;
}