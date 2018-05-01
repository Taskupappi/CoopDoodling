#ifndef TILE_H
#define TILE_H

#include <SFML\System\Vector2.hpp>

class Player;

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
	sf::Vector2f position();

private:
	sf::Vector2f m_position;
	sf::Vector2i m_index;
	Player* m_player;
	const Type m_type;
};

#endif