#ifndef TILE_H
#define TILE_H

#include <SFML\System\Vector2.hpp>

class Tile {
public:
	Tile();
	~Tile();
private:
	sf::Vector2i m_position;
};

#endif