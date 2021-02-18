#pragma once

#include <SFML/Graphics.hpp>
#include "SharedData.hpp"
#include "../Scenes/SplashScreen/SplashScreenScene.hpp"

class Game {

public:
	
	Game();
	~Game();
	Game(const Game&) = delete;
	Game& operator=(const Game&) = delete;

	void run();
	
private:

	sf::RenderWindow m_window;
	
	DataPtr m_data;

	sf::Time m_elapsed;
	sf::Clock m_clock;

};

