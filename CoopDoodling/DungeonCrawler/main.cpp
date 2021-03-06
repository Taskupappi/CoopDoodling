#include <SFML/Graphics.hpp>

#include "Player.h"
#include "zone.h"
#include "scene.h"
#include "network.h"
#include <random>
#include <iostream>
#include <chrono>

#define SCREEN_DIMENSION_X 800
#define SCREEN_DIMENSION_Y 600
#define FONT_NAME "veteran_typewriter.ttf"

sf::Font font;
sf::Text playerTurn;

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


std::vector<Move> activePlayerMoves;

//random
std::random_device rd;
std::mt19937 gen(rd());
std::uniform_int_distribution<> dis(0, 255);

void endTurn();
void sendTurn();

int main(int argc, char* argv[])
{
	activePlayer = nullptr;

	if (!font.loadFromFile(FONT_NAME)) {
		std::cerr << "failed to load the required font: " << FONT_NAME << std::endl;
		system("PAUSE");
		return -1;
	}
	playerTurn.setFont(font);
	playerTurn.setCharacterSize(20);
	playerTurn.setStyle(sf::Text::Regular);
	playerTurn.setPosition(SCREEN_DIMENSION_X / 2.0f, 30.0f);

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

		//wait for required data
		std::cout << "Waiting for data from server" << std::endl;
		sf::Packet packet;
		std::size_t received;
		bool allInitPacketsReceived = false;
		while (!allInitPacketsReceived) {
			if (network.socket().receive(packet) != sf::Socket::Status::Done) {
				std::cerr << "error while trying to receive data from the host" << std::endl;
				system("PAUSE");
				exit(1);
			}
			Network::PacketType type;
			packet >> type;
			
			switch (type) {
			case Network::PacketType::Movement: {

				break;
			}
			case Network::PacketType::Player: {
				Player* remotePlayer = new Player(&zone);
				packet >> remotePlayer;
				players.push_back(remotePlayer);
				break;
			}
			case Network::PacketType::InitDone: {
				allInitPacketsReceived = true;
				break;
			}
			}

		}
		for (Player* player : players) {
			if (player->activePlayer()) {
				activePlayer->setActivePlayer(false);
				activePlayer = player;
				player->setActivePlayer(true);
				break;
			}
		}
	
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
		player->setActivePlayer(true);
		players.push_back(player);
		activePlayer = player;
	}

	//set window settings
	sf::RenderWindow window(sf::VideoMode(SCREEN_DIMENSION_X, SCREEN_DIMENSION_Y), "Dungeon Crawler");
	window.setKeyRepeatEnabled(true);
	//set camera

	//generate map if host

	while (window.isOpen())
	{
		playerTurn.setString("Player's: " + std::to_string(activePlayer->m_id) + " turn");
		playerTurn.setOrigin(playerTurn.getLocalBounds().width / 2.0f,
			playerTurn.getLocalBounds().height / 2.0f);
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
						move.playerId = activePlayer->m_id;
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
						move.playerId = activePlayer->m_id;
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
						move.playerId = activePlayer->m_id;
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
						move.playerId = activePlayer->m_id;
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
			window.draw(playerTurn);

			window.display();

			if (activePlayer->m_actions == 0
				&& activePlayer->m_id == clientId) {
				//send data of movement to clients
				
				//pack moves
				for (Move move : activePlayerMoves) {
					sf::Packet packet;
					packet << move;
					network.storePacket(packet);
				}
				
				//send packed moves
				for (Player* player : players) {
					for (sf::Packet packet : network.packets()) {
						player->socket()->send(packet);
					}
				}
				//discard sent packets
				network.packets().clear();

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

				for (Player* player : players) {
					sf::Packet playerPacket;
					playerPacket << player;
					network.socket().send(playerPacket);
				}
			}
			else {
				delete socket;
				socket = nullptr;
			}

			//end turn
		}
		else {
			//Client's side
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
			window.draw(playerTurn);
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
	for (std::size_t i = 0; i < players.size(); ++i) {
		if (players[i]->m_id == activePlayer->m_id
			&& (i + static_cast<unsigned int>(1)) < players.size()) {
			activePlayer->setActivePlayer(false);
			players[i + 1]->setActivePlayer(true);
			activePlayer = players[i + 1];
		}
		else {
			activePlayer->setActivePlayer(false);
			players[0]->setActivePlayer(true);
			activePlayer = players[0];
		}
	}
}

void sendTurn()
{

}