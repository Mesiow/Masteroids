#pragma once
#include <Thor/Math.hpp>
#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/Graphics/CircleShape.hpp>
#include "../Net/Constants.h"

struct Star {
	sf::CircleShape star;
};

class Starfield {
public:
	Starfield();
	~Starfield();
	void render(sf::RenderTarget& target, sf::RenderStates states);
	void setStarCount(int count);
	void populate();

private:
	void addStar(Star &star);

private:
	int _starCount;
	std::vector<Star> _stars;
};