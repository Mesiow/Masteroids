#include "Multiplayer.h"
#include <iostream>

Multiplayer::Multiplayer()
{
	initialize();
}

Multiplayer::~Multiplayer()
{
	for (size_t i = 0; i < MAX_CONNECTIONS; i++) {
		if (_connects[i]) {
			if (_peers[i]) delete _peers[i];
			if (_asteroids[i]) delete _asteroids[i];
		}
	}
}

void Multiplayer::render(sf::RenderTarget& target)
{
	for (size_t i = 0; i < MAX_CONNECTIONS; i++) {
		if (_connects[i]) {
			if (_peers[i]) _peers[i]->render(target);
			if (_asteroids[i]) _asteroids[i]->render(target);
		}
	}	
}

void Multiplayer::update(Client_t id, float dt)
{
	if (playersAreReady() && !_running) {
		std::cout << "Running\n";
		_running = true;
	}

	if (_running) {
		//Update locally
		if (_connects[id]) {
			_peers[id]->update(dt);
			_asteroids[id]->update(dt);
		}


		//Update all peer bullets
		for (size_t i = 0; i < MAX_CONNECTIONS; ++i) {
			if (_connects[i]) {
				if (_peers[i]) {
					if (id != i) //update peer bullets, make sure we are not updating our own bullets again locally
						_peers[i]->updateBullets(dt);
				}
			}
		}
	}
}

void Multiplayer::handleInput(Client_t id, float dt)
{
	if(_connects[id])
		_peers[id]->handleInput(dt);
}

void Multiplayer::handleEvents(Client_t id, sf::Event& ev)
{
	if (_connects[id])
		_peers[id]->handleEvents(ev);
}

void Multiplayer::initialize()
{
	zeroMem();
	_host.port = 7777;
}

void Multiplayer::updatePeer(Client_t id, PeerState state)
{
	if (_connects[id]) {
		_peers[id]->setPosition(state.x, state.y);
		_peers[id]->setRotation(state.rot);
	}
}

void Multiplayer::updateAsteroid(Client_t id, uint8_t asteroidId, AsteroidState state)
{
	if (_connects[id]) {
		_asteroids[id]->getAsteroids()[asteroidId].setPosition(state.x, state.y);
		_asteroids[id]->getAsteroids()[asteroidId].setRotation(state.rot);
	}
}

void Multiplayer::spawnPeerBullet(Client_t id, BulletState state)
{
	if (_connects[id]) {
		_peers[id]->shoot(state.x, state.y, state.dx, state.dy);
	}
}

void Multiplayer::spawnPeerAsteroid(Client_t id, AsteroidState state)
{
	/*if (_connects[id]) {
		_asteroids[id]->getAsteroids()[id]
		Asteroid asteroid(64.0f, sf::Vector2f(state.x, state.y), sf::Vector2f(state.dx, state.dy));
		_asteroids[id]->add(asteroid);
		std::cout << "spawned at " <<state.x <<", "<< state.y << "\n";
	}*/
}

void Multiplayer::addPlayer(PeerEndPoint endPoint, Client_t id)
{
	sf::Color playerColor;
	if (id == 0) playerColor = sf::Color::Green;
	if (id == 1) playerColor = sf::Color::Yellow;

	_peerEndPoints[id] = endPoint;
	_connects[id] = true;
	_peers[id] = new Player(playerColor);
	_asteroids[id] = new AsteroidManager();
	if (id == 0) _asteroids[id]->spawn(10, 10); //If host, spawn asteroid

	std::cout << "Id: " << (int)id << std::endl;
}

Client_t Multiplayer::addPlayer(PeerEndPoint endPoint)
{
	int slot = emptySlot();
	if (slot != -1) {
		Client_t newPeerId = slot;
		addPlayer(endPoint, newPeerId);
		return newPeerId;
	}
	else {
		std::cerr << "No Slots left\n";
		return -1;
	}
}

void Multiplayer::setHost(PeerEndPoint host)
{
	_host.address = host.address;
	_host.port = host.port;
}

void Multiplayer::setSimRunning(Client_t id, bool running)
{
	_simRunning[id] = running;
}

PeerEndPoint Multiplayer::getHost() const
{
	return _host;
}

PeerEndPoint Multiplayer::getPeer(Client_t id) const
{
	return _peerEndPoints[id];
}

bool Multiplayer::isPeerConnected(Client_t id) const
{
	return _connects[id];
}

void Multiplayer::handleSpawn(Client_t id, float x, float y)
{
	_peers[id]->setPosition(x, y);
}

void Multiplayer::handleAsteroidSpawn(Client_t id, float x, float y)
{
	_asteroids[id]->spawn(x, y);
}

void Multiplayer::zeroMem()
{
	std::fill(_peerEndPoints.begin(), _peerEndPoints.end(), PeerEndPoint());
	std::fill(_peers.begin(), _peers.end(), nullptr);
	std::fill(_connects.begin(), _connects.end(), false);
	std::fill(_simRunning.begin(), _simRunning.end(), false);
}

int Multiplayer::emptySlot()
{
	for (size_t i = 0; i < MAX_CONNECTIONS; i++) {
		if (!_connects[i])
			return i;
	}
	return -1;
}

bool Multiplayer::playersAreReady()
{
	int count = 0;
	for (size_t i = 0; i < MAX_CONNECTIONS; ++i) {
		if (_simRunning[i]) count++;
	}
	return count >= MAX_CONNECTIONS;
}
