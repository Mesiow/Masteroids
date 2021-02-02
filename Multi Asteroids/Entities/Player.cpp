#include "Player.h"
#include <Game/ResourceManager/ResourceManager.h>
#include <iostream>

Player::Player(const sf::Color &pcolor)
{
	_playerColor = pcolor;
	build();
}

Player::~Player()
{
}

void Player::render(sf::RenderTarget& target)
{
	renderPlayer(target);
	for (auto& b : _bullets) b.render(target);
}

void Player::render(sf::RenderTarget& target, sf::RenderStates states) const
{
	//target.draw(_thrustSprite, states);
	draw(target, _sprite.getTransform());
}


void Player::update(float dt)
{
	_sprite.move(_velocity);
	_sprite.rotate(_angle);
	_origin.setPosition(_sprite.getPosition());
	_position = _sprite.getPosition();
	
	updateBullets(dt);
}

void Player::updateBullets(float dt)
{
	for (auto& b : _bullets) b.update(dt);
	/*
		Remove out of bounds bullets
	*/
	if (_bullets.size() > 0) {
		_bullets.erase(std::remove_if(_bullets.begin(), _bullets.end(),
			[&](const Bullet& b) { return b.isOutOfBounds(); }), _bullets.end());
	}
}

void Player::handleInput(float dt)
{
	calculateDirection();

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::W)) {
		calculateVelocity(dt);
	}
	/*
		Rotate ship
	*/
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::A)) {
		calculateRotation(Rotate::Left, dt);
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::D)) {
		calculateRotation(Rotate::Right, dt);
	}
}

void Player::handleEvents(sf::Event& ev)
{
	switch (ev.type) {
		case sf::Event::KeyPressed: {
			if (ev.key.code == sf::Keyboard::Space && _shootTimer.getElapsedTime().asSeconds() > 0.3f) {
				_shootTimer.restart();
				isShooting = true;
				shoot();
			}
		}break;
	}
}

void Player::reset()
{
	isDead = false;
	_sprite.setPosition(SCREEN_WIDTH / 2.0f, SCREEN_HEIGHT / 2.0f);
	_sprite.setRotation(0);
	_velocity = sf::Vector2f(0, 0);
	_angle = 0.0f;
}

void Player::setPosition(float x, float y)
{
	_position = sf::Vector2f(x, y);
	_sprite.setPosition(x, y);
}

void Player::setRotation(float angle)
{
	_sprite.setRotation(angle);
}

void Player::removeBullet(int index)
{
	_bullets.erase(_bullets.begin() + index);
}

void Player::shoot(float x, float y, float dx, float dy)
{
	Bullet bullet(normalize(sf::Vector2f(dx, dy)));
	bullet.spawn(x, y);
	_bullets.push_back(bullet);

	//_shoot.play();
}

void Player::build()
{
	//Player
	_sprite.setRadius(15);
	_sprite.setScale(1.0f, 1.5f);
	_sprite.setPointCount(3);
	_sprite.setOrigin(_sprite.getRadius(), _sprite.getRadius());
	_sprite.setPosition(SCREEN_WIDTH / 2.0f, SCREEN_HEIGHT / 2.0f);
	_sprite.setFillColor(sf::Color::Transparent);
	_sprite.setOutlineColor(_playerColor);
	_sprite.setOutlineThickness(2.0f);

	_mirroredPlayer = _sprite;
	_mirroredPlayer.setOutlineThickness(1.0f);
	_mirroredPlayer.setOutlineColor(sf::Color::Green);

	_origin.setFillColor(sf::Color::Red);
	_origin.setRadius(5.0f);
	_origin.setOrigin(_origin.getRadius(), _origin.getRadius());
	_origin.setPosition(_sprite.getPosition());

	/*_thrustSprite.setTexture(ResourceManager::getTexture("thrust"));
	_thrustSprite.setOrigin(
		sf::Vector2f(_thrustSprite.getLocalBounds().width / 2.0f,
			_thrustSprite.getLocalBounds().height / 2.0f)
	);*/

	_position = _sprite.getPosition();
	_velocity = sf::Vector2f(0, 0);
	_speed = 10.0f;
	_acceleration = 1.0f;
	_angle = 0.0f;
	_rotationSpeed = 10.0f;

	_shoot.setBuffer(ResourceManager::getSoundBuffer("shoot"));
	_shoot.setVolume(65.0f);
}

void Player::shoot()
{
	Bullet bullet(normalize(_direction));
	bullet.spawn(_sprite.getPosition().x, _sprite.getPosition().y);
	_bullets.push_back(bullet);

	_shoot.play();
}

void Player::calculateDirection()
{
	_direction.x = sin(M_PI * _sprite.getRotation() / 180.0f);
	_direction.y = -cos(M_PI * _sprite.getRotation() / 180.0f);
	_direction = normalize(_direction);
}

void Player::calculateVelocity(float dt)
{
	_velocity.x += _direction.x * ((_speed * _acceleration)) * dt;
	_velocity.y += _direction.y * ((_speed * _acceleration)) * dt;

	 //Keep player speed within speed range
	_velocity.x = std::clamp(_velocity.x, -_speed, _speed);
	_velocity.y = std::clamp(_velocity.y, -_speed, _speed);
}

void Player::calculateRotation(Rotate dir, float dt)
{
	if (dir == Rotate::Left) {
		_angle -= _rotationSpeed * dt;
	}
	else if (dir == Rotate::Right) {
		_angle += _rotationSpeed * dt;
	}
	
	/*
		Keep rotation speed of angle below 10
	*/
	if (_angle >= _rotationSpeed)
		_angle = _rotationSpeed;
}

void Player::renderPlayer(sf::RenderTarget& target)
{
	/*
		Render and wrap player around the screen when needed
	*/
	//Sprite bounding box and position
	sf::FloatRect spriteRect = _sprite.getGlobalBounds();
	sf::Vector2f spritePos = _sprite.getPosition();

	_mirroredPlayer = _sprite;
	_mirroredPlayer.setOutlineColor(sf::Color::Green);

	//Init to same position as sprite
	sf::Vector2f mirrorPos;
	mirrorPos = spritePos;

	Util::wrapCoords(spritePos.x, spritePos.y, spritePos.x, spritePos.y);
	_sprite.setPosition(spritePos.x, spritePos.y);

	target.draw(_sprite);
}

void Player::onDraw(sf::RenderTarget& target, const sf::Transform& transform) const
{
	target.draw(_thrustSprite, transform);
}

void Player::drawSpriteAt(sf::RenderTarget& target, sf::CircleShape& sprite, float x, float y)
{
	sprite.setPosition(x, y);
	target.draw(sprite);
}