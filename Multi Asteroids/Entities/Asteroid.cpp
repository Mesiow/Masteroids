#include "Asteroid.h"
#include <iostream>
Asteroid::Asteroid(float radius, float x, float y)
{
	_radius = radius;
	_position = sf::Vector2f(x, y);
	build();
}

void Asteroid::render(sf::RenderTarget& target)
{
	target.draw(_shape);
}

void Asteroid::update(float& dt)
{
	_velocity = (_direction * _speed) * dt;
	_angle +=_rotationSpeed * dt;
	if (_angle >= _rotationSpeed)
		_angle = _rotationSpeed;

	_shape.move(_velocity);
	_shape.rotate(_angle);

	sf::Vector2f position = _shape.getPosition();
	Util::wrapCoords(position.x, position.y, position.x, position.y);
	_shape.setPosition(position);
}

void Asteroid::setPosition(float x, float y)
{
	_shape.setPosition(x, y);
}

float Asteroid::getRadius() const
{
	return _radius;
}

sf::Vector2f Asteroid::getPosition() const
{
	return _shape.getPosition();
}

void Asteroid::build()
{
	 int verts = 20;
	_shape.setPointCount(verts);
	//Create vertices for jagged asteroid polygon
	for (int i = 0; i < verts; i++) {
		//value between 0.0f and 1.0f to plot point between
		float radius = (float)rand() / (float)RAND_MAX * 0.4f + 0.8f;

		//Plot point and scale by actual radius
		sf::Vector2f point;
		point.x = (radius * sinf(((float)i / (float)verts) * 6.28318f)) * _radius;
		point.y = (radius * cosf(((float)i / (float)verts) * 6.28318f)) * _radius;
		_shape.setPoint(i, point);
	}
	_shape.setOrigin(_radius - _radius, _radius - _radius);
	_shape.setFillColor(sf::Color::Transparent);
	_shape.setOutlineThickness(1.0f);
	_shape.setOutlineColor(sf::Color::White);
	_shape.setPosition(_position);

	_velocity = sf::Vector2f(0, 0);
	_angle = 0;
	_rotationSpeed = 0.5f;

	//Random direction
	float rx = thor::random(0.0f, 360.0f);
	float ry = thor::random(0.0f, 360.0f);
	_direction.x = sin(M_PI * rx / 180.0f);
	_direction.y = -cos(M_PI * ry / 180.0f);
	_direction = normalize(_direction);
	_speed = thor::random(15.0f, 150.0f);
}
