#pragma once
#include "Multiplayer.h"
#include "Constants.h"
#include "Packets.h"

//TODO: Get Asteroids networked and synced

//Events of each peer
enum class eEvent : uint8_t {
	None,
	Shoot,
	Dead,
	AsteroidSpawn
};

class Peer : public Net {
public:
	/*
		takes in whether this peer acts as the host or not
	*/
	Peer(bool host = false); 
	~Peer();

	void handleInput(float dt);
	void handleEvents(sf::Event& ev);
	void update(float dt);
	void render(sf::RenderTarget& target);

	bool isConnected()const { return _connected; }

	const Player& getPlayer()const { return _multi.getPlayer(_id); }

private:
	void initSocket();
	void initialize();
	void sendPacket(sf::Packet& packet, PeerEndPoint endPoint);

	/*
		peer will broadcast to the host port to find a hosted game
	*/
	void broadcast(); 
	void waitForConnections();

	void receivePackets()override;
	void updateState();
	void sendState();

	void sendPositionData(const PeerEndPoint& endPoint);
	void sendBulletData(const PeerEndPoint &endPoint);
	void sendAsteroidData(const PeerEndPoint& endPoint);

	void handleConnectionRequest(const sf::IpAddress& address, uint16_t port);
	void handleConnectionResponse(sf::Packet &packet, const sf::IpAddress& address, uint16_t port);
	void handleBroadcast(const sf::IpAddress& address, uint16_t port);
	void handleBroadcastResponse(const sf::IpAddress& address, uint16_t port);

	/* Handle Game Data Packets */
	void handlePeerState(sf::Packet& packet);
	void handlePeerBullet(sf::Packet& packet);
	void handlePeerAsteroid(sf::Packet& packet);

	///////
	bool connect();

private:
	Multiplayer _multi;
	Input_t _input;
	Client_t _id;
	eEvent _event;

	bool _isHost = false;
	bool _connected = false;
	bool _simRunning = false;
	bool _foundHost = false;

	int _frameCount = 0;

	/*
		Timer used to determine when to send asteroid state updates for syncing
	*/
	sf::Clock _syncTimer;
};