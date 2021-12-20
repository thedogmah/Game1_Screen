#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#pragma once
class Chat
{
   public: Chat();
         sf::Font chatFont;
         

   // sf::Event event;
    sf::String playerInput;
    sf::Text playerText;
  
    std::string id; //connect chat text to ip number / character.

};

