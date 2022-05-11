#pragma once
#include <SFML/Graphics.hpp>

#include <vector>
#include <string>

class AnimatedGIF
{public:
    AnimatedGIF(const std::string& filename);

    const sf::Vector2i& getSize();

    void update(sf::Sprite& sprite, float deltaTime);


    struct GifFrame
    {
        float position;
        float duration;
        sf::Texture texture;
    };

    sf::Vector2i size;
    int frameIndex = 0;
    float frameDuration = 0;
    float totalDuration;
    std::vector<GifFrame> frames;
};


