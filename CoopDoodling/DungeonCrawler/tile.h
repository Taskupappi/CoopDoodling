#ifndef TILE_H
#define TILE_H

#include <SFML\System\Vector2.hpp>
#include "Player.h"

#define TILESIZE 64

class Tile {
public:
	enum class Type {
		ground,
		wall
	};
	
	Tile(int x, int y, Type type);
	~Tile();

	Player* player();
	void setPlayer(Player* player);

private:
	sf::Vector2i m_position;
	Player* m_player;
	const Type m_type;
};

#endif