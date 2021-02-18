#include "Game.hpp"

Game::Game() :
m_elapsed(sf::Time::Zero),
m_data(std::make_shared<SharedData>()) {

	m_data->window.create();
	
	m_data->sceneManager.add(Scene(new SplashScreenScene(m_data)));
}

Game::~Game() {

	m_data.reset();
}

void Game::run() {


	const float FRAME_RATE = 1.0f / 60.0f;

	while (!m_data->window.closed()) {

		m_data->window.handleEvents();

		if (m_data->window.isFocused()) {

			if (m_elapsed.asSeconds() >= FRAME_RATE) {
				
				m_data->sceneManager.getActiveScene()->update(m_elapsed.asSeconds());
				m_elapsed -= sf::seconds(FRAME_RATE);				
				
				m_data->window.clear();

				m_data->sceneManager.getActiveScene()->draw();
				m_data->window.display();

			}		
			else
				m_elapsed += m_clock.restart();
		}
			
	}

	m_data->clearAllAssets();
}




