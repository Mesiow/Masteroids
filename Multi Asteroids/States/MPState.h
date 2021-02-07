#pragma once
#include <Game/State/State.h>
#include <Game/Utility/FPSCounter.h>
#include "../Net/Peer.h"
#include "../Entities/Starfield.h"


/*
	Multiplayer state instance of the game
*/
class MPState : public State {
public:
	MPState(Game* game, bool peerHost = true);
	~MPState();

	void handleEvents(sf::Event& ev, sf::RenderWindow& window)override;
	void handleInput(float dt)override;
	void update(float dt)override;
	void render(sf::RenderWindow& window)override;

private:
	Peer* _peer = nullptr; //multiplayer instance
	Starfield _starfield;
	FPSCounter _counter;
};