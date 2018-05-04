#ifndef NETWORK_H
#define NETWORK_H

#include <SFML\Network.hpp>
#include <iostream>
#define PORT 3231

class Network
{
public:
	Network();
	~Network();
	bool connect(sf::IpAddress adress);
	sf::TcpListener& listener() { return m_listener; }
	bool isHost() { return m_isHost; }
	void setHost(bool isHost) { m_isHost = isHost; };
	void storePacket(sf::Packet& packet);
	std::vector<sf::Packet>& packets();
private:
	sf::TcpSocket m_socket;
	sf::TcpListener m_listener;
	bool m_isHost;
	bool m_newConnection;
	std::vector<sf::Packet> m_packets
};

#endif