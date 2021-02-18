#pragma once

#include "../../Core/BaseGameScene.hpp"
#include "../../Core/Fader.hpp"
#include "../GameScene/MainScene.hpp"

#include <SFML/Graphics/Sprite.hpp>
#include <SFML/System/Thread.hpp>

class SplashScreenScene : public BaseGameScene {

public:

	SplashScreenScene(DataPtr data);

	virtual void init() override;
	virtual void update(float deltaTime) override;
	virtual void draw() override;
	
private:	
	void startFadeOut(const sf::Event& e);	

private:

	const float FADE_TIME = 3.5f;

	sf::Thread m_loadThread;

	Fader m_fader;
	sf::Sprite m_splash;	
};

