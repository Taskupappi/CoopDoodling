#ifndef NETWORK_H
#define NETWORK_H

#include <SFML\Network.hpp>
#include <iostream>
#define PORT 32431

class Network
{
public:
	Network();
	~Network();
	bool connect(sf::IpAddress adress);
	sf::TcpListener& listener() { return m_listener; }
	void CheckNewConnections();
	bool isHost() { return m_isHost; }
	void setHost(bool isHost) { m_isHost = isHost; };
private:
	sf::TcpSocket m_socket;
	sf::TcpListener m_listener;
	std::vector<sf::TcpSocket*> m_clients;
	bool m_isHost;
	bool m_newConnection;
};

#endif