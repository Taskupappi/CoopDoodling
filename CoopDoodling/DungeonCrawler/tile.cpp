#include "tile.h"

Tile::Tile(int x, int y, Type type)
	:m_position(x, y),
	m_type(type)
{
	m_player = nullptr;
}

Tile::~Tile()
{
	delete m_player;
	m_player = nullptr;
}

Player * Tile::player()
{
	return m_player;
}

void Tile::setPlayer(Player* player)
{
	m_player = player;
}
