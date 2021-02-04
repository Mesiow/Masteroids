#pragma once
#include <Game/State/State.h>
#include <Game/Utility/FPSCounter.h>
#include "../Entities/AsteroidManager.h"
#include "../Entities/Player.h"
#include "../Entities/Starfield.h"
#include "../Collision/Collision.h"
#include <Thor/Math.hpp>

/*
	Single player state instance of the game
*/
class SPState : public State {
public:
	SPState(Game* game);
	~SPState();

	void handleEvents(sf::Event& ev, sf::RenderWindow& window)override;
	void handleInput(float dt)override;
	void update(float dt)override;
	void render(sf::RenderWindow& window)override;

private:
	void initText();
	void updateText();
	/*
		Splits single asteroid into a smaller versions, based on parent radius and position
	*/
	void detectAndHandleCollision();
	/*
		Reset game state
	*/
	void reset();
	void checkGameOver(sf::RenderTarget &target);

private:
	Starfield _starfield;
	FPSCounter _counter;

	/*
		Game objects
	*/
	Player *_player;
	AsteroidManager _asteroidManager;
	
	sf::Text _scoreText;
	sf::Text _livesText;
	sf::Text _gameOverText;

	int _score = 0;
	int _round = 1;
	int _lives = 3;
	bool _gameOver = false;

	sf::Clock _gameOverWait;
};