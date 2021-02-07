#include "MPState.h"
#include <Game/Game.h>
#include <Game/ResourceManager/ResourceManager.h>

MPState::MPState(Game* game, bool peerHost)
	:State(game)
{
	_starfield.setStarCount(500);
	_starfield.populate();

	_peer = new Peer(peerHost);
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

