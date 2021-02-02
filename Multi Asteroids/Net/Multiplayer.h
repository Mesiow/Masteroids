#pragma once
#include <Game/Net/Net.h>
#include "Constants.h"
#include "Packets.h"
#include "../Entities/Player.h"
#include <algorithm>
#include <array>

struct PeerEndPoint {
	sf::IpAddress address;
	uint16_t port;
};

struct PeerState {
	float x, y;
	float rot;
};

struct BulletState {
	float x, y, dx, dy; //position and direction of bullet when it fired
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


	/*
		Update peer with new state data
	*/
	void updatePeer(Client_t id, PeerState state);
	/*
		Update peer bullets with new bullet data
	*/
	void updatePeerBullet(Client_t id, BulletState state);

	void addPlayer(PeerEndPoint endPoint, Client_t id);
	/*
		Function used for players connecting to already running host.
		Returns new client id slot for this player
	*/
	Client_t addPlayer(PeerEndPoint endPoint);

	void setHost(PeerEndPoint host);
	/*
		Set if the simulation is running for a client
	*/
	void setSimRunning(Client_t id, bool running);

	PeerEndPoint getHost()const;
	PeerEndPoint getPeer(Client_t id)const;
	bool isPeerConnected(Client_t id)const;
	bool isSimRunning(Client_t id)const { return _simRunning[id]; }
	const Player& getPlayer(Client_t id)const { return *_peers[id]; }
	Player& getPlayer(Client_t id) { return *_peers[id]; }

private:
	void initialize();
	void handleSpawn(Client_t peerId);

	void zeroMem();
	int emptySlot();

private:
	PeerEndPoint _host;
	std::array<PeerEndPoint, MAX_CONNECTIONS> _peerEndPoints;
	std::array<Player*, MAX_CONNECTIONS> _peers;
	std::array<bool, MAX_CONNECTIONS> _connects;
	std::array<bool, MAX_CONNECTIONS> _simRunning;
};