#pragma once
#include <Game/Net/Net.h>
#include "Constants.h"
#include "Packets.h"
#include "../Entities/Player.h"
#include "../Entities/AsteroidManager.h"
#include "../Collision/Collision.h"
#include <algorithm>
#include <array>

//Events of each peer
enum class eEvent : uint8_t {
	None,
	Shoot,
	Dead,
	AsteroidSpawn,
	AsteroidHit
};

struct PeerState {
	float x, y;
	float rot;
};

struct BulletState {
	float x, y, dx, dy; //position and direction of bullet when it fired
};

struct AsteroidState {
	float x, y, dx, dy;
	float rot;
};

/*
	Class that takes care of connecting/storing peers, entities and sending data to peers
	in a P2P architecture and runs the multiplayer simulation
*/
class Multiplayer {
public:
	Multiplayer();
	~Multiplayer();
	void render(sf::RenderTarget& target);
	void update(Client_t id, float dt);
	void handleInput(Client_t id, float dt);
	void handleEvents(Client_t id, sf::Event& ev);


	void handleCollisions(Client_t id, eEvent& event, std::vector<int> &bullets_To_Remove, std::vector<int> &asteroids_To_Remove);
	/*
		Update peer with new state data
	*/
	void updatePeer(Client_t id, PeerState state);
	/*
		Sync asteroid
	*/
	void updateAsteroid(Client_t id, uint8_t asteroidId, AsteroidState state);
	/*
		Spawn peer bullets with new bullet data
	*/
	void spawnPeerBullet(Client_t id, BulletState state);

	void handleSpawn(Client_t id, float x, float y);
	void handleAsteroidSpawn(Client_t id, float x, float y);

	void addPlayer(const EndPoint &endPoint, Client_t id);
	/*
		Function used for players connecting to already running host.
		Returns new client id slot for this player
	*/
	Client_t addPlayer(const EndPoint &endPoint);

	void setHost(const EndPoint& host);
	void setSeed(int seed);
	/*
		Set if the simulation is running for a client
	*/
	void setSimRunning(Client_t id, bool running);

	EndPoint getHost()const;
	EndPoint getPeer(Client_t id)const;

	bool isPeerConnected(Client_t id)const;
	bool isSimRunning(Client_t id)const { return _simRunning[id]; }


	const Player& getPlayer(Client_t id)const { return *_peers[id]; }
	const std::vector<Asteroid>& getAsteroids(Client_t id)const { return _asteroids[id]->getAsteroids(); }
	AsteroidManager* getAsteroidManager(Client_t id) { return _asteroids[id]; }
	Player& getPlayer(Client_t id) { return *_peers[id]; }
	int getSeed()const { return _seed; }

private:
	void initialize();
	void zeroMem();
	int emptySlot();

	bool playersAreReady();

private:
	EndPoint _host;
	std::array<EndPoint, MAX_CONNECTIONS> _peerEndPoints;
	std::array<Player*, MAX_CONNECTIONS> _peers;
	std::array<AsteroidManager*, MAX_CONNECTIONS> _asteroids;
	std::array<bool, MAX_CONNECTIONS> _connects;
	std::array<bool, MAX_CONNECTIONS> _simRunning;

	int _seed;
	int _round = 1;

	float _interpCounter = 0.0f;
	float _elapsedTime = 0.0f;

	bool _running = false;
};