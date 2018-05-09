#include "Player.h"
Player::Player(Zone* zone)
	:m_actions(3),
	m_initReady(false),
	m_actionsLeft(3)
{
	m_zone = zone;
	m_socket = nullptr;
}

Player::~Player()
{
}

void Player::endTurn()
{
	m_actionsLeft = m_actions;	
}

void Player::move(int x, int y)
{

}

void Player::setSocket(sf::TcpSocket * socket)
{
	m_socket = socket;
}

sf::TcpSocket * Player::socket()
{
	return m_socket;
}
