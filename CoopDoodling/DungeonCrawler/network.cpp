#include "network.h"

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

