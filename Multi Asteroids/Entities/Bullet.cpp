#include "Bullet.h"
#include <iostream>

Bullet::Bullet(const sf::Vector2f& direction)
{
	_bullet.setRadius(3.0f);
	_bullet.setFillColor(sf::Color::Red);
	_bullet.setOrigin(_bullet.getRadius(), _bullet.getRadius());

	_direction = direction;
	_velocity = sf::Vector2f(0, 0);
	_speed = 200.0f;
}

void Bullet::render(sf::RenderTarget& target)
{
	target.draw(_bullet);
}

void Bullet::update(float dt)
{
	calculateVelocity(dt);

	_bullet.move(_velocity);
	_position = _bullet.getPosition();
}

void Bullet::spawn(float x, float y)
{
	_position.x = x; _position.y = y;
	_bullet.setPosition(_position);
}

bool Bullet::isOutOfBounds() const
{
	return ((_position.x > SCREEN_WIDTH || _position.x < 0) 
		|| (_position.y > SCREEN_HEIGHT || _position.y < 0));
}

sf::Vector2f Bullet::getPosition() const
{
	return _bullet.getPosition();
}

void Bullet::calculateVelocity(float dt)
{
	/*
		Give a constant velocity
	*/
	_velocity = (_direction * _speed) * dt;
}
