#include "MPState.h"
#include <Game/Game.h>
#include <Game/ResourceManager/ResourceManager.h>

MPState::MPState(Game* game)
	:State(game)
{
	_starfield.setStarCount(500);
	_starfield.populate();

	char c;
	std::cout << "1. Host\n";
	std::cout << "2. Peer\n";
	std::cin >> c;
	if (c == '1') {
		_peer = new Peer(true);
	}
	else if (c == '2') {
		_peer = new Peer();
	}
}

MPState::~MPState()
{
	delete _peer;
}

void MPState::handleEvents(sf::Event& ev, sf::RenderWindow& window)
{
	_peer->handleEvents(ev);
}

void MPState::handleInput(float dt)
{
	_peer->handleInput(dt);
}

void MPState::update(float dt)
{
	_counter.update();

	_peer->update(dt);
}

void MPState::render(sf::RenderWindow& window)
{
	_starfield.render(window, sf::RenderStates::Default);

	_counter.render(window);

	_peer->render(window);
}
