#include "network.h"
#include "Player.h"

Network::Network()
	:m_newConnection(false)
{
	m_listener.setBlocking(false);
	m_socket.setBlocking(true);
}

Network::~Network()
{
}

bool Network::connect(sf::IpAddress address)
{
	sf::Socket::Status status = m_socket.connect(address, PORT);
	if (status != sf::Socket::Done) {
		std::cout << "couldn't connect to network!" << std::endl;
		return false;
	}
	return true;
}

void Network::storePacket(sf::Packet & packet)
{
	m_packets.push_back(packet);
}

std::vector<sf::Packet>& Network::packets()
{
	return m_packets;
}

sf::Packet& operator <<(sf::Packet& packet, const Move& move) {
	packet << move.playerId;
	packet << move.oldPosition.x << move.oldPosition.y;
	packet << move.newPosition.x << move.newPosition.y;
	return packet;
}

sf::Packet& operator >>(sf::Packet& packet, Move& move) {
	packet >> move.playerId;
	packet >> move.oldPosition.x >> move.oldPosition.y;
	packet >> move.newPosition.x >> move.newPosition.y;
	return packet;
}

sf::Packet& operator <<(sf::Packet& packet, const Player* player) {
	packet << player->m_id;
	packet << player->m_actions;
	packet << player->m_actionsLeft;
	packet << player->m_position.x << player->m_position.y;
	packet << player->color().r << player->color().g << player->color().b << player->color().a;
	return packet;
}

sf::Packet& operator >>(sf::Packet& packet, Player* player) {
	packet >> player->m_id;
	packet >> player->m_actions;
	packet >> player->m_actionsLeft;
	sf::Vector2i pos;
	packet >> pos.x << pos.y;
	player->m_position.x = pos.x;
	player->m_position.y = pos.y;
	sf::Color color;
	packet >> color.r >> color.g >> color.g >> color.a;
	player->setColor(color);
	return packet;
}