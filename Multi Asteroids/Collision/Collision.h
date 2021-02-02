#pragma once
#include "../Entities/Bullet.h"
#include "../Entities/Asteroid.h"

class Player;

namespace Collision {
	enum class Type : uint8_t {
		BulletAsteroid = 0, /* Player's bullet hits an asteroid */
		PlayerAsteroid, /* The Player himself collides with an asteroid */
	};
	/*
		Check if a bullet has collided with an asteroid
	*/
	bool hasCollided(const Bullet& bullet, const Asteroid& asteroid);
	bool hasCollided(const Player& player, const Asteroid& asteroid);
}