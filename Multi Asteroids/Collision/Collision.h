#pragma once
#include "../Entities/Bullet.h"
#include "../Entities/Asteroid.h"

class Player;

namespace Collision {
	/*
		Check if a bullet has collided with an asteroid
	*/
	bool hasCollided(const Bullet& bullet, const Asteroid& asteroid);
	bool hasCollided(const Player& player, const Asteroid& asteroid);
}