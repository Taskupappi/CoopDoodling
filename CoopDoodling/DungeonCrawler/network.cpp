#include "network.h"

Network::Network()
	:m_newConnection(false)
{
}

Network::~Network()
{
}

bool Network::connect(sf::IpAddress address)
{
	sf::Socket::Status status = m_socket.connect(address, 32431);
	if (status != sf::Socket::Done) {
		std::cout << "couldn't connect to network!" << std::endl;
		return false;
	}
	return true;
}

void Network::CheckNewConnections()
{
	if (m_newConnection) {
		for (sf::TcpSocket* socket : m_clients) {

		}
	}
}
