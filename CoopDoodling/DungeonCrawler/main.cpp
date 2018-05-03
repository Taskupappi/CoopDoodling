#include <SFML/Graphics.hpp>

#include "scene.h"
#include "network.h"
#include <random>
#include <iostream>
#include <chrono>
#include "Player.h"

sf::Sprite playerFrame;
sf::Sprite playerSprite;
sf::Sprite groundSprite;
int clientId;
Player* activePlayer;
void passTurn() {};
sf::Clock systemTime;

std::vector<Player*> players;
std::vector<int> turnOrder;
sf::Color randomPlayerColor();

struct Move {
	sf::Vector2i oldPosition;
	sf::Vector2i newPosition;
};
std::vector<Move> activePlayerMoves;

//random
std::random_device rd;
std::mt19937 gen(rd());
std::uniform_int_distribution<> dis(0, 255);

void endTurn();
void sendTurn();

int main(int argc, char* argv[])
{
	Zone zone(3,3);

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
		} while (!connected);
	}
	else if (connectionMode == "h") {
		network.setHost(true);
		if (network.listener().listen(PORT) != sf::Socket::Done) {
			std::cout << "couldn't set up network listener!" << std::endl;
		}
		sf::Color playerColor(dis(gen), dis(gen), dis(gen), 255);
		//sf::Color playerColor = sf::Color::White;
		Player* player = new Player(&zone);
		player->m_id = clientId;
		player->m_texture.loadFromFile("testSprite.png");
		player->m_sprite.setTexture(player->m_texture);
		player->m_sprite.setColor(playerColor);
		player->m_sprite.setOrigin((TILESIZE / 2.0f), (TILESIZE / 2.0f));
		player->m_sprite.setPosition(sf::Vector2f(200, 200));
		player->m_sprite.setScale(0.8f, 0.8f);
		players.push_back(player);
		activePlayer = player;
	}

	//set window settings
	sf::RenderWindow window(sf::VideoMode(800, 600), "Dungeon Crawler");
	window.setKeyRepeatEnabled(true);
	//set camera

	//generate map if host

	while (window.isOpen())
	{
		//print mouse position

		if (network.isHost()) {
			///receive data
			///actions
			sf::Event event;
			while (window.pollEvent(event))
			{
				if (event.type == sf::Event::Closed)
					window.close();

				//handle keyboard events
				if (activePlayer->m_id == clientId
					&& event.type == sf::Event::KeyPressed) {
					if (event.key.code == sf::Keyboard::W
						&& activePlayer->m_actionsLeft > 0) {
						Move move;
						move.oldPosition = sf::Vector2i(activePlayer->m_position.x, activePlayer->m_position.y);
						activePlayer->m_sprite.move(0, -(activePlayer->m_sprite.getGlobalBounds().height));
						move.newPosition = sf::Vector2i(activePlayer->m_position.x, activePlayer->m_position.y);
						activePlayerMoves.push_back(move);
						--activePlayer->m_actionsLeft;						
						if (activePlayer->m_actionsLeft == 0) {
							endTurn();
						}
					} 
					else if (event.key.code == sf::Keyboard::S
						&& activePlayer->m_actionsLeft > 0) {
						Move move;
						move.oldPosition = sf::Vector2i(activePlayer->m_position.x, activePlayer->m_position.y);
						activePlayer->m_sprite.move(0, activePlayer->m_sprite.getGlobalBounds().height);
						move.newPosition = sf::Vector2i(activePlayer->m_position.x, activePlayer->m_position.y);
						activePlayerMoves.push_back(move);
						--activePlayer->m_actionsLeft;
						if (activePlayer->m_actionsLeft == 0) {
							endTurn();
						}
					}
					else if (event.key.code == sf::Keyboard::A
						&& activePlayer->m_actions > 0) {
						Move move;
						move.oldPosition = sf::Vector2i(activePlayer->m_position.x, activePlayer->m_position.y);
						activePlayer->m_sprite.move(-(activePlayer->m_sprite.getGlobalBounds().width), 0);
						move.newPosition = sf::Vector2i(activePlayer->m_position.x, activePlayer->m_position.y);
						activePlayerMoves.push_back(move);
						--activePlayer->m_actionsLeft;
						if (activePlayer->m_actionsLeft == 0) {
							endTurn();
						}
					}
					else if (event.key.code == sf::Keyboard::D
						&& activePlayer->m_actions > 0) {
						Move move;
						move.oldPosition = sf::Vector2i(activePlayer->m_position.x, activePlayer->m_position.y);
						activePlayer->m_sprite.move(activePlayer->m_sprite.getGlobalBounds().width, 0);
						move.newPosition = sf::Vector2i(activePlayer->m_position.x, activePlayer->m_position.y);
						activePlayerMoves.push_back(move);
						--activePlayer->m_actionsLeft;
						if (activePlayer->m_actionsLeft == 0) {
							endTurn();
						}
					}
				}
			}			

			//draw stuff
			window.clear();
			
			zone.draw(&window);
			
			for (Player* player : players) {
				if (player->m_id == clientId) {
					playerFrame.setPosition(player->m_sprite.getPosition());
					window.draw(playerFrame);
				}
				window.draw(player->m_sprite);
			}

			window.display();

			if (activePlayer->m_actions == 0
				&& activePlayer->m_id == clientId) {
				//send data of movement to clients
				
				//pack moves

				
				//send packed moves
				for (Player* player : players) {

				}

			}
			
			//handle new players connecting to a game
			sf::TcpSocket* socket = new sf::TcpSocket();
			if (network.listener().accept(*socket) == sf::Socket::Done) {
				Player* player = new Player(&zone);
				player->m_id = clientId;
				player->m_texture.loadFromFile("testSprite.png");
				player->m_sprite.setTexture(player->m_texture);
				player->m_sprite.setColor(randomPlayerColor());
				player->m_sprite.setOrigin((TILESIZE / 2.0f), (TILESIZE / 2.0f));
				std::uniform_int_distribution<> pos(0, 8);
				float x = static_cast<float>(pos(gen));
				float y = static_cast<float>(pos(gen));
				player->m_sprite.setPosition(sf::Vector2f(TILESIZE * x, TILESIZE * y));
				player->m_sprite.setScale(0.8f, 0.8f);
				player->setSocket(socket);
				players.push_back(player);
			}
			else {
				delete socket;
				socket = nullptr;
			}

			//end turn
		}
		else {
			sf::Event event;
			while (window.pollEvent(event))
			{
				if (event.type == sf::Event::Closed)
					window.close();

				//handle keyboard events
				if (activePlayer->m_id == clientId
					&& event.type == sf::Event::KeyPressed) {
					if (event.key.code == sf::Keyboard::W
						&& activePlayer->m_actionsLeft > 0) {
						activePlayer->m_sprite.move(0, -(activePlayer->m_sprite.getGlobalBounds().height));
						--activePlayer->m_actionsLeft;
						if (activePlayer->m_actionsLeft == 0) {
							activePlayer->endTurn();
						}
					}
					else if (event.key.code == sf::Keyboard::S
						&& activePlayer->m_actionsLeft > 0) {
						activePlayer->m_sprite.move(0, activePlayer->m_sprite.getGlobalBounds().height);
						--activePlayer->m_actionsLeft;
						if (activePlayer->m_actionsLeft == 0) {
							activePlayer->endTurn();
						}
					}
					else if (event.key.code == sf::Keyboard::A
						&& activePlayer->m_actions > 0) {
						activePlayer->m_sprite.move(-(activePlayer->m_sprite.getGlobalBounds().width), 0);
						--activePlayer->m_actionsLeft;
						if (activePlayer->m_actionsLeft == 0) {
							activePlayer->endTurn();
						}
					}
					else if (event.key.code == sf::Keyboard::D
						&& activePlayer->m_actions > 0) {
						activePlayer->m_sprite.move(activePlayer->m_sprite.getGlobalBounds().width, 0);
						--activePlayer->m_actionsLeft;
						if (activePlayer->m_actionsLeft == 0) {
							activePlayer->endTurn();
						}
					}
				}
			}

			//draw stuff
			window.clear();

			zone.draw(&window);

			for (Player* player : players) {
				if (player->m_id == clientId) {
					playerFrame.setPosition(player->m_sprite.getPosition());
					window.draw(playerFrame);
				}
				window.draw(player->m_sprite);
			}

			window.display();

			if (activePlayer->m_actions == 0
				&& activePlayer->m_id == clientId) {
				//send data of movement to clients
				sendTurn();
			}			
		}



		///send data




	}

	return 0;
}

sf::Color randomPlayerColor()
{
	sf::Color playerColor;
	//get Red color for a player
	playerColor.r = dis(gen);
	//get Green color for a player
	playerColor.g = dis(gen);
	//get Blue color for a player
	playerColor.b = dis(gen);
	//check if the color is available
	for (Player* player : players) {
		if ((playerColor.r - 10) < player->color().r && player->color().r < (playerColor.r + 10)
			&& (playerColor.g - 10) < player->color().g && player->color().g < (playerColor.g + 10)
			&& (playerColor.b - 10) < player->color().b && player->color().b < (playerColor.b + 10)) {
			randomPlayerColor();
		}
	}
	return playerColor;
}

void endTurn()
{
	activePlayer->endTurn();

}

void sendTurn()
{

}