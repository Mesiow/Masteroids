#pragma once
#include <SFML/Network/Packet.hpp>
#include <cstdint>

using Input_t = sf::Uint8;
using Client_t = sf::Uint8;
using Packet_t = sf::Uint8;

enum class ePacket : Packet_t {
	ConnectionRequest = 0,
	ConnectionResponse,
	Broadcast,
	BroadcastResponse,


	/* Peer data */
	PeerState,
	Bullet,
	Died
};

sf::Packet& operator<<(sf::Packet& packet, ePacket p);
sf::Packet& operator>>(sf::Packet& packet, ePacket& p);