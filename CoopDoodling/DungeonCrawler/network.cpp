#include "network.h"

Network::Network()
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
}
