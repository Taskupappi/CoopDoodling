#include "zone.h"

Zone::Zone(std::uint64_t xSize, std::uint64_t ySize)
	: m_sizeX(xSize),
	m_sizeY(ySize)
{
	m_groundTexture.loadFromFile("groundSprite.png");
	m_groundSprite = new sf::Sprite(m_groundTexture);
	m_wallTexture.loadFromFile("wallSprite.png");
	m_wallSprite = new sf::Sprite(m_wallTexture);
	
	for (std::uint64_t x = 0; x < xSize; ++x) {
		for (std::uint64_t y = 0; y < ySize; ++y) {
			Tile* tile = new Tile(x * TILESIZE, y * TILESIZE, Tile::Type::ground);
			m_tiles.push_back(tile);
		}
	}
}

Zone::~Zone()
{
	delete m_groundSprite;
	delete m_wallSprite;
}

void Zone::draw(sf::RenderWindow * window)
{
	for (Tile* tile : m_tiles) {
		m_groundSprite->setPosition(tile->position().x, tile->position().y);
		window->draw(*m_groundSprite);
	}
}
