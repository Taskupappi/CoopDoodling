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


	sf::Sprite m_sprite;
	sf::Texture m_texture;
	sf::TcpSocket m_socket;
	bool m_initReady = false;
	sf::Color m_color;
	int m_id;
	int m_actionsLeft;
	int m_actions;
	Zone* m_zone
};

#endif
