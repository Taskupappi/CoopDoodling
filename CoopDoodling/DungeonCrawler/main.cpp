#include <SFML/Graphics.hpp>

#include "scene.h"
#include "network.h"
#include <random>
#include <iostream>
#include <chrono>
#include "Player.h"

struct Zone {
	enum class tileType {
		ground,
	};
};

Zone generateZone();

sf::Sprite playerFrame;
sf::Sprite playerSprite;
sf::Sprite groundSprite;
int clientId;
Player* activePlayer;
void passTurn() {};
sf::Clock systemTime;

std::vector<Player*> players;
std::vector<int> turnOrder;

//random
std::random_device rd;
std::mt19937 gen(rd());
std::uniform_int_distribution<> dis(0, 255);

int main(int argc, char* argv[])
{
	sf::Texture playerFrameTexture;
	playerFrameTexture.loadFromFile("playerFrame.png");
	playerFrame.setTexture(playerFrameTexture);
	playerFrame.setOrigin(32.0f, 32.0f);

	systemTime.restart();
	sf::Clock clock;
	clock.restart();
	sf::Time elapsed = clock.getElapsedTime();
	clientId = static_cast<int>(systemTime.getElapsedTime().asMilliseconds());

	Network network;
	std::string connectionMode;
	do {
		std::cout << "insert mode: (h) Host, (c) Client" << std::endl;
		std::cin >> connectionMode;
	} while (connectionMode != "h" && connectionMode != "c");
	if (connectionMode == "c") {
		network.setHost(false);
		bool connected = false;
		do {
			std::string connectionAddress;
			std::cout << "insert ip address to connect:" << std::endl;
			std::cin >> connectionAddress;
			sf::IpAddress address(connectionAddress);
			connected = network.connect(address);
		} while (connected);
	}
	else if (connectionMode == "h") {
		network.setHost(true);
		if (network.listener().listen(PORT) != sf::Socket::Done) {
			std::cout << "couldn't set up network listener!" << std::endl;
		}
		sf::Color playerColor(dis(gen), dis(gen), dis(gen), 255);
		//sf::Color playerColor = sf::Color::White;
		Player* player = new Player();
		player->m_id = clientId;
		player->m_texture.loadFromFile("testSprite.png");
		player->m_sprite.setTexture(player->m_texture);
		player->m_sprite.setColor(playerColor);
		player->m_sprite.setOrigin(32, 32);
		player->m_sprite.setPosition(sf::Vector2f(200, 200));
		player->m_sprite.setScale(0.8f, 0.8f);
		players.push_back(player);
		activePlayer = player;
	}

	//set window settings
	sf::RenderWindow window(sf::VideoMode(800, 600), "SFML works!");
	window.setKeyRepeatEnabled(true);
	//set camera

	//generate map if host

	while (window.isOpen())
	{
		//print mouse position

		if (network.isHost()) {
			//handle new players connecting to a game
			network.CheckNewConnections();

			///receive data

			///actions
			sf::Event event;
			while (window.pollEvent(event))
			{
				if (event.type == sf::Event::Closed)
					window.close();

				//handle turns
				if (activePlayer->m_id == clientId
					&& event.type == sf::Event::KeyPressed) {
					if (event.key.code == sf::Keyboard::W) {
						for (Player* player : players) {
							if (player->m_id == clientId
								&& player->m_actionsLeft > 0) {
								player->m_sprite.move(0, -(player->m_sprite.getGlobalBounds().height));
								--player->m_actionsLeft;
								if (player->m_actionsLeft == 0) {
									player->endTurn();
								}
							}
						}
					}
					else if (event.key.code == sf::Keyboard::S) {
						for (Player* player : players) {
							if (player->m_id == clientId
								&& player->m_actionsLeft > 0) {
								player->m_sprite.move(0, player->m_sprite.getGlobalBounds().height);
								--player->m_actionsLeft;
								if (player->m_actionsLeft == 0) {
									player->endTurn();
								}
							}
						}
					}
					else if (event.key.code == sf::Keyboard::A) {
						for (Player* player : players) {
							if (player->m_id == clientId
								&& player->m_actionsLeft > 0) {
								player->m_sprite.move(-(player->m_sprite.getGlobalBounds().width), 0);
								--player->m_actionsLeft;
								if (player->m_actionsLeft == 0) {
									player->endTurn();
								}
							}
						}
					}
					else if (event.key.code == sf::Keyboard::D) {
						for (Player* player : players) {
							if (player->m_id == clientId
								&& player->m_actionsLeft > 0) {
								player->m_sprite.move(player->m_sprite.getGlobalBounds().width, 0);
								--player->m_actionsLeft;
								if (player->m_actionsLeft == 0) {
									player->endTurn();
								}
							}
						}
					}
				}
			}			

			window.clear();
			for (Player* player : players) {
				if (player->m_id == clientId) {
					playerFrame.setPosition(player->m_sprite.getPosition());
					window.draw(playerFrame);
				}
				window.draw(player->m_sprite);
			}
			window.display();
		}
		else {

		}



		///send data




	}

	return 0;
}


Zone generateZone()
{
	Zone zone;
	
	return zone;
};