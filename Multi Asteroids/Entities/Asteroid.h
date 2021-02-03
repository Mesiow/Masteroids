#pragma once
#include <SFML/Graphics.hpp>
#include <Game/Math/Math.h>
#include <Thor/Math.hpp>
#include "Util.h"

class Asteroid {
public:
	Asteroid(float radius, const sf::Vector2f& position, const sf::Vector2f& direction);
	Asteroid(float radius, float x, float y);
	void render(sf::RenderTarget& target);
	void update(float& dt);
	
	void setPosition(float x, float y);
	void setRotation(float rot);


	float getRadius()const;
	float getRotation()const;
	sf::Vector2f getPosition()const;
	sf::Vector2f getDirection()const;

private:
	/*
		Builds asteroids' vertices
	*/
	void build();
	/*
		initializes asteroids properties such as random direction/speed
	*/
	void initialize();

private:
	sf::Vector2f _direction;
	sf::Vector2f _velocity;
	sf::Vector2f _position;
	float _rotationSpeed;
	float _speed;
	float _radius;
	float _angle;
	sf::ConvexShape _shape;
};