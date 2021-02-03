#pragma once

#include "Asteroid.h"
#include <vector>

class AsteroidManager {
public:
	AsteroidManager();
	void render(sf::RenderTarget& target);
	void update(float& dt);

	void add(Asteroid& asteroid);
	void remove(int index);
	void spawn(float xoffset, float yoffset);
	/*
		Takes in player angle to make sure asteroids spawn at a distance
	*/
	void spawnNewRound(int round, float playerAngle);

	size_t getSize()const { return _asteroids.size(); }
	const std::vector<Asteroid>& getAsteroids()const { return _asteroids; }
	std::vector<Asteroid>& getAsteroids() { return _asteroids; }
	std::vector<Asteroid> split(const Asteroid& asteroid);
	
private:
	std::vector<Asteroid> _asteroids;
};