#include "AsteroidManager.h"

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
