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
	_elapsedTime = dt;

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

void Multiplayer::handleCollisions(Client_t id, eEvent& event)
{
	//Handle Collisions
	auto player = _peers[id];

	auto& pbullets = _peers[id]->getBullets();
	std::vector<int> bulletsToRemove; //vector of bullet indexes to remove
	std::vector<int> asteroidsToRemove;
	std::vector<Asteroid> newAsteroids; //new asteroid vector to append new asteroids that split

	auto asteroidManager = getAsteroidManager(id);
	auto& asteroids = getAsteroids(id);
	size_t numAsteroids = getAsteroids(id).size();

	if (pbullets.size() > 0 && numAsteroids > 0) {
		for (size_t i = 0; i < numAsteroids; ++i) {
			for (size_t j = 0; j < pbullets.size(); ++j) {
				auto& bullet = pbullets[j];
				auto& asteroid = asteroids[i];

				/*
					Bullet collision with asteroid
				*/
				if (Collision::hasCollided(bullet, asteroid)) {
					event = eEvent::AsteroidHit;
					/*
						store bullet index to remove for outside of this loop
					*/
					bulletsToRemove.push_back(j);
					asteroidsToRemove.push_back(i);
					std::cout << "Bullet hit asteroid\n";

					//_score++;

					/*
						Only split asteroids if they have a radius greater than 4
					*/
					if (asteroid.getRadius() > 8) {
						//newAsteroids = _asteroidManager.split(asteroid);
					}
				}
			}
		}
	}

	//if (numAsteroids > 0) {
	//	/*
	//		Check player collision with an asteroid
	//	*/
	//	for (auto& a : asteroids) {
	//		if (Collision::hasCollided(*_player, a)) {
	//			_player->isDead = true;
	//		}
	//	}
	//}

	//If any bullets to remove, remove them
	if (bulletsToRemove.size() > 0) {
		for (size_t i = 0; i < bulletsToRemove.size(); i++) {
			player->removeBullet(bulletsToRemove[i]);
		}
	}

	////If any new asteroids, append to original vector and remove old asteroid
	if (asteroidsToRemove.size() > 0) {
		for (size_t i = 0; i < asteroidsToRemove.size(); i++) {
			asteroidManager->remove(asteroidsToRemove[i]);
		}
	}

	//	if (newAsteroids.size() > 0) {
	//		for (auto& a : newAsteroids)
	//			_asteroidManager.add(a);
	//	}
	//}
}

void Multiplayer::initialize()
{
	zeroMem();
	_host.port = 7777;

	_seed = thor::random(1, 100);
}

void Multiplayer::updatePeer(Client_t id, PeerState state)
{
	if (_connects[id]) {
		/*_interpCounter += _elapsedTime;
		_interpCounter = std::clamp(_interpCounter, 0.0f, 1.0f);

		sf::Vector2f oldPosition = _peers[id]->getPosition();
		std::cout << "Old Pos: " << oldPosition.x << ", " << oldPosition.y << std::endl;
		sf::Vector2f targetPosition = sf::Vector2f(state.x, state.y);
		std::cout << "New Pos: " << targetPosition.x << ", " << targetPosition.y << std::endl;

		float xpos = lerp(oldPosition.x, targetPosition.x, _interpCounter);
		float ypos = lerp(oldPosition.y, targetPosition.y, _interpCounter);*/

		_peers[id]->setPosition(state.x, state.y);
		_peers[id]->setRotation(state.rot);

		//_interpCounter = 0;
	}
}

void Multiplayer::updateAsteroid(Client_t id, uint8_t asteroidId, AsteroidState state)
{
	if (_connects[id]) {
		if (_asteroids[id]->getAsteroids().size() > 0) {
			_asteroids[id]->getAsteroids()[asteroidId].setPosition(state.x, state.y);
			_asteroids[id]->getAsteroids()[asteroidId].setDirection(state.dx, state.dy);
			_asteroids[id]->getAsteroids()[asteroidId].setRotation(state.rot);
		}
	}
}

void Multiplayer::spawnPeerBullet(Client_t id, BulletState state)
{
	if (_connects[id]) {
		_peers[id]->shoot(state.x, state.y, state.dx, state.dy);
	}
}

void Multiplayer::addPlayer(const EndPoint& endPoint, Client_t id)
{
	sf::Color playerColor;
	if (id == 0) playerColor = sf::Color::Green;
	if (id == 1) playerColor = sf::Color::Yellow;

	_peerEndPoints[id] = endPoint;
	_connects[id] = true;
	_peers[id] = new Player(playerColor);

	_asteroids[id] = new AsteroidManager();
	_asteroids[id]->setSeed(_seed);
	std::cout << "Seed: " << _seed << std::endl;


	std::cout << "Id: " << (int)id << std::endl;
}

Client_t Multiplayer::addPlayer(const EndPoint& endPoint)
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

void Multiplayer::setHost(const EndPoint& host)
{
	_host.address = host.address;
	_host.port = host.port;
}

void Multiplayer::setSeed(int seed)
{
	_seed = seed;
}

void Multiplayer::setSimRunning(Client_t id, bool running)
{
	_simRunning[id] = running;
}

EndPoint Multiplayer::getHost() const
{
	return _host;
}

EndPoint Multiplayer::getPeer(Client_t id) const
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
	if (_connects[id]) {
		_asteroids[id]->spawnNewRound(1);
		/*Asteroid asteroid(64.0f, x, y);
		_asteroids[id]->add(asteroid);*/
	}
}

void Multiplayer::zeroMem()
{
	std::fill(_peerEndPoints.begin(), _peerEndPoints.end(), EndPoint());
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
