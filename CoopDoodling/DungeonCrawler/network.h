#ifndef NETWORK_H
#define NETWORK_H

#include <SFML\Network.hpp>
#include <iostream>
#define PORT 3231

class Player;

class Network
{
public:
	enum class PacketType {
		Movement,
		ActivePlayer
	};
	Network();
	~Network();
	bool connect(sf::IpAddress adress);
	sf::TcpListener& listener() { return m_listener; }
	bool isHost() { return m_isHost; }
	void setHost(bool isHost) { m_isHost = isHost; }
	void storePacket(sf::Packet& packet);
	std::vector<sf::Packet>& packets();
	sf::TcpSocket& socket() { return m_socket; }
private:
	sf::TcpSocket m_socket;
	sf::TcpListener m_listener;
	bool m_isHost;
	bool m_newConnection;
	std::vector<sf::Packet> m_packets;
};

sf::Packet& operator <<(sf::Packet& packet, const Move& move) {
	packet << move.playerId;
	packet << move.oldPosition.x << move.oldPosition.y;
	packet << move.newPosition.x << move.newPosition.y;
}

sf::Packet& operator >>(sf::Packet& packet, const Move& move) {
	packet >> move.playerId;
	packet >> move.oldPosition.x >> move.oldPosition.y;
	packet >> move.newPosition.x >> move.newPosition.y;
}

sf::Packet& operator <<(sf::Packet& packet, const Player* player) {
	packet << player->m_id;
	packet << player->m_actions;
	packet << player->m_actionsLeft;
	packet << player->m_position.x << player->m_position.y;
	packet << player->color().r << player->color().g << player->color().b << player->color().a;
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
}

#endif