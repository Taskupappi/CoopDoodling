#ifndef NETWORK_H
#define NETWORK_H
#include <iostream>
#include <SFML\Graphics.hpp>
#include <SFML\Network.hpp>

#define PORT 3231
#define NETWORK_BUFFER_SIZE 4096

class Player;

struct Move {
	sf::Vector2i oldPosition;
	sf::Vector2i newPosition;
	int playerId;
};

enum class PacketType {
	Player,
	Move
};

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

sf::Packet& operator <<(sf::Packet& packet, const Move& move);
sf::Packet& operator >>(sf::Packet& packet, Move& move);
sf::Packet& operator <<(sf::Packet& packet, const Player* player);
sf::Packet& operator >>(sf::Packet& packet, Player* player);

#endif