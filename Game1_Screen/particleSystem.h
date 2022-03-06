#include <iostream>
#include <SFML/Graphics.hpp>
class ParticleSystem : public sf::Drawable, public sf::Transformable
{
public:

    ParticleSystem(unsigned int count) :
        m_particles(count),
        m_vertices(sf::Triangles, count),
        m_lifetime(sf::seconds(1.f)),
        m_emitter(0.f, 0.f)
       
    {
    }

    void setEmitter(sf::Vector2f position)
    {
        m_emitter = position;
    }

    void update(sf::Time elapsed)
    {
        for (std::size_t i = 0; i < m_particles.size(); i+=3)
        {
            // update the particle lifetime
            Particle& p = m_particles[i];
            p.lifetime -= elapsed;
         //   std::cout << p.lifetime.asSeconds() << " <> " << elapsed.asSeconds() << " ";
            // if the particle is dead, respawn it
            if (p.lifetime <= sf::Time::Zero)
                resetParticle(i);

            // update the position of the corresponding vertex
            m_vertices[i].position += p.velocity * (elapsed.asSeconds()/4);
          //  p.velocity.x += 5;
            m_vertices[i+1].position = sf::Vector2f(m_vertices[i].position.x + 12 * std::cos(i - 1), m_vertices[i].position.y +12 * std::sin(i - 1));;
          //  m_vertices[i + 1].position = sf::Vector2f(m_vertices[i].position.x + 30 * std::cos(i - 1), m_vertices[i].position.y + 30 * std::sin(i - 1));;
           // p.velocity.y += 5;

            m_vertices[i+2].position = sf::Vector2f(m_vertices[i].position.x + 13 * std::cos(i + 5), m_vertices[i].position.y + 13 * std::sin(i + 5));
         //   p.velocity.x -= 5;
            

            //m_vertices[i+3].position += p.velocity * elapsed.asSeconds();


            // update the alpha (transparency) of the particle according to its lifetime
            float ratio = p.lifetime.asSeconds() / m_lifetime.asSeconds();
           // m_vertices[i].color.a = static_cast<sf::Uint8>(ratio * 130);
            m_vertices[i].color.g = static_cast<sf::Uint8>((ratio *5.5)* 255);
            //m_vertices[i].color.r = static_cast<sf::Uint8>((ratio * 5.5) * 255);
            m_vertices[i+2].color.a = static_cast<sf::Uint8>(ratio * 255);
          //  m_vertices[i+2].color.g = static_cast<sf::Uint8>((ratio * 5.5) * 255);
           m_vertices[i+1].color.a = static_cast<sf::Uint8>(ratio * 50);
            //m_vertices[i+1].color.g = static_cast<sf::Uint8>((ratio * 5.5) * 255);
          //  m_vertices[i+1].color.r = static_cast<sf::Uint8>((ratio * 5.5) * 255);
            //  std::cout << ratio << "\n";
        }
    }

private:

    virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const
    {
        // apply the transform
        states.transform *= getTransform();

        // our particles don't use a texture
        states.texture = NULL;

        // draw the vertex array
        target.draw(m_vertices, states);
    }

private:

    struct Particle
    {
        sf::Vector2f velocity;
        sf::Time lifetime;
    };

    void resetParticle(std::size_t index)
    {
        // give a random velocity and lifetime to the particle
        float angle = (std::rand() % 360) * 3.14f / 180.f;
        float speed = (std::rand() % 12) + 11.f;
        m_particles[index].velocity = sf::Vector2f(std::cos(angle) * speed, std::sin(angle) * speed);
        m_particles[index].lifetime = sf::milliseconds((std::rand() % 900) + 500);
        m_vertices[index].color = sf::Color::Yellow;
        m_vertices[index + 1].color = col;
        m_vertices[index+2].color = sf::Color::Red;
        // reset the position of the corresponding vertex
        m_vertices[index].position = m_emitter;
    }

    std::vector<Particle> m_particles;
    sf::VertexArray m_vertices;
    sf::Time m_lifetime;
    sf::Vector2f m_emitter;
    sf::Color col{255, 140, 0};
};