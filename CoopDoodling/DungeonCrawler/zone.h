#ifndef ZONE_H
#define ZONE_H

#include <vector>
#include "tile.h"
#include <SFML\Graphics.hpp>

class Zone
{
public:
	Zone(std::uint64_t xSize, std::uint64_t ySize);
	~Zone();

private:
	std::vector<Tile*> m_tiles;
	sf::Texture m_groundTexture;
	sf::Sprite *m_groundSprite;
	sf::Texture m_wallTexture;
	sf::Sprite *m_wallSprite;
	std::uint64_t m_sizeX;
	std::uint64_t m_sizeY;
};

#endif