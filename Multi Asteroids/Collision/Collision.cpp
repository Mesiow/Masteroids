#include "Collision.h"
#include "../Entities/Player.h"

bool Collision::hasCollided(const Bullet& bullet, const Asteroid& asteroid)
{
    sf::Vector2f b = bullet.getPosition();
    sf::Vector2f a = asteroid.getPosition();
    float radius = asteroid.getRadius();
    return (sqrt(pow(a.x - b.x, 2) + pow((a.y - b.y), 2)) < radius);
}

bool Collision::hasCollided(const Player& player, const Asteroid& asteroid)
{
    sf::Vector2f p = player.getPosition();
    sf::Vector2f a = asteroid.getPosition();
    float radius = asteroid.getRadius();
    return (sqrt(pow(a.x - p.x, 2) + pow((a.y - p.y), 2)) < radius);
}
