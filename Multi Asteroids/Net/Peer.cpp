#include "Peer.h"
#include <iostream>

Peer::Peer(bool host)
{
	_isHost = host;
	initialize();
}

Peer::~Peer()
{
	
}

void Peer::handleInput(float dt)
{
	if (!_connected) return;

	_multi.handleInput(_id, dt);
}

void Peer::handleEvents(sf::Event& ev)
{
	_multi.handleEvents(_id, ev);
}

void Peer::update(float dt)
{
	if (!_connected) return;
	
	_multi.update(_id, dt);

	receivePackets();
	updateState();
	sendState();

	_frameCount++;
}

void Peer::render(sf::RenderTarget& target)
{
	_multi.render(target);
}

void Peer::initSocket()
{
	address = sf::IpAddress::getLocalAddress();
	port = 56743;
	if (_isHost) port = _multi.getHost().port;

	std::cout << "Binding to port " << port << std::endl;
	if (socket.bind(port, address) == sf::Socket::Done) std::cout << "Socket bound Ok\n";
}

void Peer::initialize()
{
	initSocket();
	if (_isHost) {
		_id = 0;
		_multi.setHost({ address, _multi.getHost().port });
		_multi.addPlayer(_multi.getHost(), _id);
		_connected = true;

		waitForConnections();
		socket.setBlocking(false);
	}
	else {
		_id = -1;
		if (!connect()) {
			std::cerr << "Failed to connect to host\n";
			exit(EXIT_FAILURE);
		}
	}
}

void Peer::sendPacket(sf::Packet& packet, PeerEndPoint endPoint)
{
	socket.send(packet, endPoint.address, endPoint.port);
}

void Peer::broadcast()
{
	sf::Packet broadcast;
	broadcast << ePacket::Broadcast;
	socket.send(broadcast, sf::IpAddress::Broadcast, _multi.getHost().port);
}

void Peer::waitForConnections()
{
	while (!_multi.isSimRunning(_id)) {
		receivePackets();
	}
}

void Peer::receivePackets()
{
	//Receive commands and data from other peers..
	sf::Packet packet;
	sf::IpAddress address;
	uint16_t port;
	if(socket.receive(packet, address, port) == sf::Socket::Done) {
		ePacket type;
		packet >> type;
		switch (type) {
			case ePacket::ConnectionRequest: handleConnectionRequest(address, port); break;
			case ePacket::ConnectionResponse: handleConnectionResponse(packet, address, port); break;
			case ePacket::Broadcast: handleBroadcast(address, port); break;
			case ePacket::BroadcastResponse: handleBroadcastResponse(address, port); break;

			/* Handle Incoming Peer Game Data */
			case ePacket::PeerState: handlePeerState(packet); break;
			case ePacket::Bullet: handlePeerBullet(packet); break;
			case ePacket::Asteroid: handlePeerAsteroid(packet); break;
		}
	}

}

void Peer::updateState()
{
	auto& player = _multi.getPlayer(_id);
	if (player.isShooting) {
		_event = eEvent::Shoot;
		player.isShooting = false;
	}
}

void Peer::sendState()
{
	//Send movement data etc.. to other peers

		sf::Packet statePacket;
		sf::Packet bulletPacket;
		if (!_isHost) { //Then we are a peer
			PeerEndPoint peer = _multi.getHost();
			if (_frameCount % 2 == 0) //send data every 2nd frame to avoid congestion
				sendPositionData(peer);

			sendBulletData(peer);
		}
		else {
			//Host
			for (size_t i = 0; i < MAX_CONNECTIONS; i++) {
				PeerEndPoint peer = _multi.getPeer(i);
				//Make sure host does not send data to itself and only to other peers
				if (_multi.getHost().port != peer.port) {
					if (_frameCount % 2 == 0)
						sendPositionData(peer);

					sendBulletData(peer);

					//Send asteroids data
					if (_syncTimer.getElapsedTime().asSeconds() >= 1.0f) {
						std::cout << "Sending asteroid data\n";
						sendAsteroidData(peer);
						_syncTimer.restart();
					}
				}
			}
		}
}

void Peer::sendPositionData(const PeerEndPoint& endPoint)
{
	sf::Packet statePacket;
	auto& player = _multi.getPlayer(_id); 

	statePacket << ePacket::PeerState << (uint8_t)_id << player.getPosition().x << player.getPosition().y
		<< player.getRotation();

	sendPacket(statePacket, endPoint);
}

void Peer::sendBulletData(const PeerEndPoint& endPoint)
{
	sf::Packet bulletPacket;
	auto& player = _multi.getPlayer(_id);

	if (_event == eEvent::Shoot) {
		bulletPacket << ePacket::Bullet << (uint8_t)_id << player.getPosition().x << player.getPosition().y
			<< player.getDirection().x << player.getDirection().y;

		sendPacket(bulletPacket, endPoint);

		_event = eEvent::None;
	}
}

void Peer::sendAsteroidData(const PeerEndPoint& endPoint)
{
	sf::Packet asteroidPacket;
	auto& asteroids = _multi.getAsteroids(_id);

	if (asteroids.size() > 0) {
		for (size_t i = 0; i < asteroids.size(); ++i) {
			auto& a = asteroids[i];

			asteroidPacket << ePacket::Asteroid  << (uint8_t)i <<
				a.getPosition().x << a.getPosition().y
				<< a.getDirection().x << a.getDirection().y
				<< a.getRotation();

			sendPacket(asteroidPacket, endPoint);
		}
	}
}

void Peer::handleConnectionRequest(const sf::IpAddress& address, uint16_t port)
{
	/*
		This function should only run on the peer who is hosting.
	*/
	sf::Packet response;
	PeerEndPoint endPoint;
	endPoint.address = address;
	endPoint.port = port;

	sf::Vector2f spawnPosition = sf::Vector2f((float)(SCREEN_WIDTH / 2.0f) - 60.0f, (float)SCREEN_HEIGHT / 2.0f);

	Client_t peerId = _multi.addPlayer(endPoint);
	_multi.handleSpawn(peerId, spawnPosition.x, spawnPosition.y);
	_multi.handleAsteroidSpawn(_id, 10, 10);

	_event = eEvent::AsteroidSpawn;

	response << ePacket::ConnectionResponse << (uint8_t)1 << peerId << _id << spawnPosition.x << spawnPosition.y;
	sendPacket(response, endPoint);

	_multi.setSimRunning(_id, true);
	_multi.setSimRunning(peerId, true);
}

void Peer::handleConnectionResponse(sf::Packet &packet, const sf::IpAddress& address, uint16_t port)
{
	uint8_t response;
	Client_t our_id;
	Client_t host_id;
	float x, y;
	///*
	//	Check if incoming response data is coming from the host peer
	//*/
	if (_multi.getHost().address == address && _multi.getHost().port == port) {
		std::cout << "Incoming from host\n";
		if (packet >> response) {
			if (response) {
				if (packet >> our_id >> host_id >> x >> y) {
					//Save id and host id and create new peer
					_id = our_id;
				
					std::cout << "Host id: " << (int)host_id << std::endl;
					std::cout << "Our id: " << (int)our_id << std::endl;
					_multi.addPlayer(_multi.getHost(), host_id); //add host
					_multi.addPlayer({ this->address, this->port }, _id); //add ourself

					_multi.handleSpawn(_id, x, y);
					_multi.handleAsteroidSpawn(_id, 10, 10);

					socket.setBlocking(false);
					/*
						Successfully connected
					*/
					_connected = true;
					_multi.setSimRunning(_id, true);
					_multi.setSimRunning(host_id, true);
				}
			}
		}
	}
}

void Peer::handleBroadcast(const sf::IpAddress& address, uint16_t port)
{
	std::cout << "Incoming connection\n";
	sf::Packet response;
	response << ePacket::BroadcastResponse;
	sendPacket(response, {address, port});
}

void Peer::handleBroadcastResponse(const sf::IpAddress& address, uint16_t port)
{
	_multi.setHost({ address, port });
	_foundHost = true;
}

void Peer::handlePeerState(sf::Packet& packet)
{
	Client_t id;
	float x, y, rot;
	if (packet >> id) {
		packet >> x >> y >> rot;
		_multi.updatePeer(id, { x, y, rot });
	}
}

void Peer::handlePeerBullet(sf::Packet& packet)
{
	Client_t id;
	float x, y, dx, dy;
	if (packet >> id) {
		packet >> x >> y >> dx >> dy;
		_multi.spawnPeerBullet(id, { x, y, dx, dy });
	}
}

void Peer::handlePeerAsteroid(sf::Packet& packet)
{
	uint8_t asteroid_id;
	float x, y, dx, dy, rot;
	if (packet >> asteroid_id) {
		packet >> x >> y >> dx >> dy >> rot;
		_multi.updateAsteroid(_id, asteroid_id, {x, y, dx, dy, rot });
	}
}

bool Peer::connect()
{
	sf::Clock timeout;
	timeout.restart();
	
	//broadcast/search for host
	socket.setBlocking(false);
	while (!_foundHost) {
		if (timeout.getElapsedTime().asSeconds() >= 3.0f)
			break;

		broadcast();
		receivePackets();
	}

	if (_foundHost) {
		std::cout << "Host IP: " << _multi.getHost().address << std::endl;
		std::cout << "Host Port: " << _multi.getHost().port << std::endl;

		sf::Packet request;
		request << ePacket::ConnectionRequest;
		sendPacket(request, _multi.getHost());

		//Wait for response from host peer
		while (!_connected) {
			if (_connected) break;
			receivePackets();
		}
		if (_connected) {
			std::cout << "Sucessfully connected\n";
		}

		return true;
	}
	else {
		std::cerr << "Host not found\n";
		return false;
	}	
}