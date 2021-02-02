#pragma once
#include <SFML/Graphics.hpp>
#include "../Net/Constants.h"

class Bullet {
public:
	Bullet(const sf::Vector2f &direction);
	void render(sf::RenderTarget& target);
	void update(float dt);
	void spawn(float x, float y);

	bool isOutOfBounds()const;
	sf::Vector2f getPosition()const;

private:
	void calculateVelocity(float dt);

private:
	sf::CircleShape _bullet;
	sf::Vector2f _velocity;
	sf::Vector2f _direction;
	sf::Vector2f _position;
	float _speed;
};