#include "SPState.h"
#include <Game/Game.h>
#include <Game/ResourceManager/ResourceManager.h>

SPState::SPState(Game* game)
	:State(game)
{
	_starfield.setStarCount(500);
	_starfield.populate();

	_player = new Player();

	_asteroidManager.spawnNewRound(_round, _player->getRotation());

	initText();
}

SPState::~SPState()
{
	delete _player;
}

void SPState::handleEvents(sf::Event& ev, sf::RenderWindow& window)
{
	_player->handleEvents(ev);
}

void SPState::handleInput(float dt)
{
	_player->handleInput(dt);
}

void SPState::update(float dt)
{
	//_counter.update();

	_player->update(dt);

	_asteroidManager.update(dt);

	detectAndHandleCollision();

	//all of the asteroids for this round have been destroyed
	if (_asteroidManager.getSize() <= 0) { 
		_round++;
		_asteroidManager.spawnNewRound(_round, _player->getRotation());
	}

	if (_player->isDead) reset();

	updateText();
}

void SPState::render(sf::RenderWindow& window)
{
	_starfield.render(window, sf::RenderStates::Default);

	//_counter.render(window);

	window.draw(_scoreText);
	window.draw(_livesText);
	

	/* Render game entities */

	_player->render(window);

	_asteroidManager.render(window);

	checkGameOver(window);
}

void SPState::initText()
{
	_scoreText.setFont(ResourceManager::getFont("mont"));
	_scoreText.setFillColor(sf::Color::White);
	_scoreText.setString("Score: " + std::to_string(_score));
	_scoreText.setCharacterSize(18);
	_scoreText.setPosition((SCREEN_WIDTH - _scoreText.getGlobalBounds().width) - 10, 20);

	_livesText.setFont(ResourceManager::getFont("mont"));
	_livesText.setFillColor(sf::Color::White);
	_livesText.setPosition(10, 20);
	std::string count = "| | |";
	std::string str = "Lives: " + count;
	_livesText.setString(str);
	_livesText.setCharacterSize(18);


	_gameOverText.setFont(ResourceManager::getFont("mont"));
	_gameOverText.setFillColor(sf::Color::White);
	_gameOverText.setString("Game Over!");
	_gameOverText.setCharacterSize(30);
	_gameOverText.setOrigin(sf::Vector2f(_gameOverText.getLocalBounds().width / 2.0f, _gameOverText.getLocalBounds().height / 2.0f + 45.0f));
	_gameOverText.setPosition(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2);
}

void SPState::updateText()
{
	_scoreText.setString("Score: " + std::to_string(_score));
	switch (_lives) {
		case 2: {
			std::string count = "| |";
			_livesText.setString("Lives: " + count);
		}break;
		case 1: {
			std::string count = "|";
			_livesText.setString("Lives: " + count);
		}break;

		default: break;
	}
}

void SPState::detectAndHandleCollision()
{
	auto& pbullets = _player->getBullets();
	std::vector<int> bulletsToRemove; //vector of bullet indexes to remove
	std::vector<int> asteroidsToRemove;
	std::vector<Asteroid> newAsteroids; //new asteroid vector to append new asteroids that split

	auto& asteroids = _asteroidManager.getAsteroids();
	size_t numAsteroids = _asteroidManager.getSize();

	if (pbullets.size() > 0 && numAsteroids > 0) {
		for (size_t i = 0; i < numAsteroids; ++i) {
			for (size_t j = 0; j < pbullets.size(); ++j) {
				auto& bullet = pbullets[j];
				auto& asteroid = asteroids[i];

				/*
					Bullet collision with asteroid
				*/
				if (Collision::hasCollided(bullet, asteroid)) {
					/*
						store bullet index to remove for outside of this loop
					*/
					bulletsToRemove.push_back(j);
					asteroidsToRemove.push_back(i);

					_score++;

					/*
						Only split asteroids if they have a radius greater than 4
					*/
					if (asteroid.getRadius() > 8) {
						newAsteroids = _asteroidManager.split(asteroid);
					}
				}
			}
		}
	}

		if (numAsteroids > 0) {
			/*
				Check player collision with an asteroid
			*/
			for (auto& a : asteroids) {
				if (Collision::hasCollided(*_player, a)) {
					_player->isDead = true;
				}
			}
		}

		//If any bullets to remove, remove them
		if (bulletsToRemove.size() > 0) {
			for (size_t i = 0; i < bulletsToRemove.size(); i++) {
				_player->removeBullet(bulletsToRemove[i]);
			}
		}

		//If any new asteroids, append to original vector and remove old asteroid
		if (asteroidsToRemove.size() > 0) {
			for (size_t i = 0; i < asteroidsToRemove.size(); i++) {
				_asteroidManager.remove(asteroidsToRemove[i]);
			}
				
			
			if (newAsteroids.size() > 0) {
				for (auto& a : newAsteroids)
					_asteroidManager.add(a);
			}
		}
}

void SPState::reset()
{
	_player->reset();
	_lives--;
	if (_lives == 0) _gameOver = true;
	if (_gameOver) _gameOverWait.restart();
}

void SPState::checkGameOver(sf::RenderTarget& target)
{
	if (_gameOver) {
		target.draw(_gameOverText);
		if (_gameOverWait.getElapsedTime().asSeconds() >= 2.5f) {
			_game->popState();
		}
	}
}
