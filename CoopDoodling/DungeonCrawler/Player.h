#ifndef PLAYER_H
#define PALYER_H

#include <SFML\Graphics.hpp>
#include <SFML\Network.hpp>

class Player {
public:
	Player()
		:m_actions(3),
		m_initReady(false),
		m_actionsLeft(3)
	{};
	~Player() {};
	void endTurn() { m_actionsLeft = m_actions; };
	sf::Sprite m_sprite;
	sf::Texture m_texture;
	sf::TcpSocket m_socket;
	bool m_initReady = false;
	sf::Color m_color;
	int m_id;
	int m_actionsLeft;
	int m_actions;
};

#endif
