#include "SplashScreenScene.hpp"

SplashScreenScene::SplashScreenScene(DataPtr data) : BaseGameScene(data) ,
m_fader(data, FADE_TIME),
m_loadThread(&MainScene::preLoadTextures, data) {

}

void SplashScreenScene::init() {

	const std::string SPLASHSCREEN_PATH =  SPRITES_PATH "splashscreen.png";
				
	m_data->textureAssets.load(CLASS_NAME(SplashScreenScene), SPLASHSCREEN_PATH);

	m_splash.setTexture(m_data->textureAssets.get(CLASS_NAME(SplashScreenScene)));
	
	sf::FloatRect bounds = m_splash.getGlobalBounds();

	m_splash.setScale(WINDOW_WIDTH / bounds.width, WINDOW_HEIGHT / bounds.height);

	m_data->window.addCallback(new EventCallback(sf::Keyboard::Key::Enter, sf::Event::EventType::KeyPressed, &SplashScreenScene::startFadeOut, this));
	m_data->window.addCallback(new EventCallback(sf::Keyboard::Key::Space, sf::Event::EventType::KeyPressed, &SplashScreenScene::startFadeOut, this));
	m_data->window.addCallback(new EventCallback(sf::Mouse::Button::Left, sf::Event::EventType::MouseButtonPressed, &SplashScreenScene::startFadeOut, this));
			
	m_loadThread.launch();	
}

void SplashScreenScene::update(float deltaTime) {

	if (m_fader.getState() != Fader::State::Done) {

		m_fader.update(deltaTime);

		if (m_fader.getState() == Fader::State::Done) {			
			m_loadThread.terminate();
			destroy();
			m_data->sceneManager.add(Scene(new MainScene(m_data)));
		}						
	}		
}

void SplashScreenScene::draw() {			
	m_data->window.draw(m_splash);
	m_data->window.draw(m_fader);
}


void SplashScreenScene::startFadeOut(const sf::Event& e) {	

	if (m_fader.getState() == Fader::State::FadedIn) {

		m_fader.setState(Fader::State::FadingOut);
	}
}


