#include "Starfield.h"

Starfield::Starfield()
{
	_starCount = 250;
	_stars.clear();
}

Starfield::~Starfield()
{
}

void Starfield::render(sf::RenderTarget& target, sf::RenderStates states)
{
	for (auto& s : _stars) target.draw(s.star);
}

void Starfield::setStarCount(int count)
{
	_starCount = count;
}

void Starfield::populate()
{
	Star star;
	star.star.setFillColor(sf::Color::White);

	for (size_t i = 0; i < _starCount; i++) {
		float posX = thor::random(0.0f, (float)SCREEN_WIDTH);
		float posY = thor::random(0.0f, (float)SCREEN_HEIGHT);
		float size = thor::random(0.1f, 1.0f);
		sf::Vector2f position(posX, posY);
	
		star.star.setPosition(position);
		star.star.setRadius(size);
		addStar(star);
	}
}

void Starfield::addStar(Star &star)
{
	_stars.emplace_back(star);
}
