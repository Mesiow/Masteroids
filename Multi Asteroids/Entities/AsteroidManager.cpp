#include "AsteroidManager.h"
#include "../Net/Constants.h"
#include <iostream>

AsteroidManager::AsteroidManager()
{
	_asteroids.clear();
}

void AsteroidManager::render(sf::RenderTarget& target)
{
	for (auto& a : _asteroids) a.render(target);
}

void AsteroidManager::update(float& dt)
{
	for (auto& a : _asteroids) a.update(dt);
}

void AsteroidManager::setSeed(unsigned int seed)
{
	_seed = seed;
	thor::setRandomSeed(_seed);
}

void AsteroidManager::add(Asteroid& asteroid)
{
	_asteroids.push_back(asteroid);
}

void AsteroidManager::remove(int index)
{
	_asteroids.erase(_asteroids.begin() + index);
}

void AsteroidManager::spawn(float xoffset, float yoffset)
{
	_asteroids.push_back(Asteroid(64.0f,
		xoffset, yoffset
	));
}

void AsteroidManager::spawnNewRound(int round, float playerAngle)
{
	int count = 1 * round;
	for (size_t i = 0; i < count; ++i) {
		spawn(
			30.0f * sinf(playerAngle - M_PI / 2.0f),
			30.0f * cosf(playerAngle - M_PI / 2.0f)
		);
	}
}

void AsteroidManager::spawnNewRound(int round)
{
	int count = 1 * round;
	for (size_t i = 0; i < count; ++i) {
		float xStart = thor::random(10.0f, 15.0f);
		float xEnd = thor::random(SCREEN_WIDTH - 15.0f, SCREEN_WIDTH - 10.0f);

		float yStart = thor::random(10.0f, 15.0f);
		float yEnd = thor::random(SCREEN_WIDTH - 15.0f, SCREEN_WIDTH - 10.0f);

		bool spawnWhereX, spawnWhereY;
		spawnWhereX = thor::random(0, 1);
		spawnWhereY = thor::random(0, 1);
		
		if (spawnWhereX && spawnWhereY) {
			spawn(xEnd, yEnd);
		}
		else {
			spawn(xStart, yStart);
		}
	}
}

std::vector<Asteroid> AsteroidManager::split(const Asteroid& asteroid)
{
	//Spawn new asteroid based on parent
	int count = thor::random(2, 3);
	float newRadius = asteroid.getRadius() / 2.0f;
	std::vector<Asteroid> asteroids;

	for (size_t i = 0; i < count; ++i) {
		Asteroid child(newRadius, asteroid.getPosition().x, asteroid.getPosition().y);
		asteroids.push_back(child);
	}
	return asteroids;
}
