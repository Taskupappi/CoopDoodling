#ifndef PLAYER_H
#define PALYER_H

#include <SFML\Graphics.hpp>
#include <SFML\Network.hpp>

#include "zone.h"

class Player {
public:
	Player(Zone* zone);
	~Player();
	void endTurn();
	void move(int x, int y);
	void setSocket(sf::TcpSocket* socket);
	sf::TcpSocket* socket();
	sf::Color color() const { return m_color; }
	void setColor(const sf::Color color) { m_color = color; }
	sf::Sprite m_sprite;
	sf::Texture m_texture;
	bool m_initReady;
	int m_id;
	int m_actionsLeft;
	int m_actions;
	Zone* m_zone;
	sf::Vector2i m_position;
	const bool activePlayer();
	void setActivePlayer(bool isActive);
private:
	sf::Color m_color;
	sf::TcpSocket* m_socket;
	bool m_activePlayer;
};

#endif
