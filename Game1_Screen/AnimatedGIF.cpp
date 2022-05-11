
#pragma once
#include "AnimatedGIF.h"

#define STB_IMAGE_IMPLEMENTATION
#define STBI_ONLY_GIF
#include "stb_image.h"
#include <iostream>
AnimatedGIF::AnimatedGIF(const std::string& filename)
{
    std::cout << "\n" << filename;
    FILE* f = stbi__fopen(filename.c_str(), "rb");
    stbi__context s;
    stbi__start_file(&s, f);

    int* delays;
    int frameCount = 0;
    int _unused = 0;
    std::cout << "\n" << filename;
    void* pixels = stbi__load_gif_main(&s, &delays, &size.x, &size.y, &frameCount, &_unused, STBI_rgb_alpha);

    sf::Image image;
    int step = size.x * size.y * 4;

    totalDuration = 0;
    for (int i = 0; i < frameCount; i++)
    {
        frames.push_back({});
        GifFrame& frame = frames.back();

        image.create(size.x, size.y, (const sf::Uint8*)pixels + step * i);

        frame.texture.loadFromImage(image);
        frame.duration = delays[i] / 1000.f;
        frame.position = totalDuration;
        totalDuration += frame.duration;
    }

    stbi_image_free(pixels);

    frameIndex = 0;
    frameDuration = 0;
    fclose(f);
}

const sf::Vector2i& AnimatedGIF::getSize()
{
    return size;
}

void AnimatedGIF::update(sf::Sprite& sprite, float deltaTime)
{
    frameDuration += deltaTime;
    if (frames.size() == 0) return;
    do
    {
        const auto& frame = frames[frameIndex];
      //uncomment to printout debug info
        //std::cout << "\nDT: " << deltaTime << "  FI: " << frameIndex << " framesize: " << frames.size() << "\n";

        if (frame.duration <= frameDuration)
        {
            frameDuration -= frame.duration;
            frameIndex++;
            if (frameIndex >= frames.size())
            {
                frameIndex = 0;
            }

            if (frame.duration <= 0)
            {
                break;
            }
        }
        else
        {
            break;
        }
    } while (true);

    sprite.setTexture(frames[frameIndex].texture);
}