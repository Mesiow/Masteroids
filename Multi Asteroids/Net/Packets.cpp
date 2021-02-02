#include "Packets.h"

sf::Packet& operator<<(sf::Packet& packet, ePacket p)
{
    packet << (Packet_t)p;
    return packet;
}

sf::Packet& operator>>(sf::Packet& packet, ePacket& p)
{
    Packet_t type;
    packet >> type;
    p = (ePacket)type;
    return packet;
}
