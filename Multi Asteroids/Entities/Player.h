#pragma once
#include <Game/Math/Math.h>
#include <SFML/Audio.hpp>
#include "Bullet.h"
#include "Util.h"
#include <algorithm>
#include <vector>

class Node
{
public:

	// ... functions to transform the node

	// ... functions to manage the node's children

	void draw(sf::RenderTarget& target, const sf::Transform& parentTransform) const
	{
		// combine the parent transform with the node's one
		sf::Transform combinedTransform = parentTransform * _transform;

		// let the node draw itself
		onDraw(target, combinedTransform);

		// draw its children
		for (std::size_t i = 0; i < _children.size(); ++i)
			_children[i]->draw(target, combinedTransform);
	}

private:

	virtual void onDraw(sf::RenderTarget& target, const sf::Transform& transform) const = 0;

	sf::Transform _transform;
	std::vector<Node*> _children;
};

enum class Rotate : uint8_t {
	Left,
	Right
};


class Player : public Node{
public:
	Player(const sf::Color &pcolor = sf::Color::White);
	~Player();
	void render(sf::RenderTarget& target);
	void render(sf::RenderTarget& target, sf::RenderStates states) const;
	void update(float dt);
	void updateBullets(float dt);
	void handleInput(float dt);
	void handleEvents(sf::Event& ev);

	void reset();
	void setPosition(float x, float y);
	void setRotation(float angle);
	inline sf::Vector2f getPosition()const { return _sprite.getPosition(); }
	inline float getRotation()const { return _sprite.getRotation(); }
	inline sf::Vector2f getDirection()const { return normalize(_direction); }

	void removeBullet(int index);
	std::vector<Bullet>& getBullets() { return _bullets; }

	//Shoot bullet from x,y at direction dx,dy
	//Function should be used by netcode
	void shoot(float x, float y, float dx, float dy);

private:
	void shoot();
	void calculateDirection();
	void calculateVelocity(float dt);
	void calculateRotation(Rotate dir, float dt);
	
	void build();

	void renderPlayer(sf::RenderTarget& target);
	void onDraw(sf::RenderTarget& target, const sf::Transform& transform)const override;
	void drawSpriteAt(sf::RenderTarget &target, sf::CircleShape &sprite, float x, float y);

public:
	bool isDead = false;
	bool isShooting = false;

private:
	//sf::Sprite _sprite;
	sf::Sprite _thrustSprite;
	sf::Vector2f _velocity;
	sf::Vector2f _position;
	sf::Vector2f _direction;
	float _speed;
	float _acceleration;
	float _angle;
	float _rotationSpeed;

	std::vector<Bullet> _bullets;

	sf::CircleShape _origin;
	sf::CircleShape _mirroredPlayer;
	sf::CircleShape _sprite;
	sf::Color _playerColor;

	sf::Sound _shoot;
	sf::Clock _shootTimer;
};