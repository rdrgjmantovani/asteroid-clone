#include "Effect.hpp"


Effect::Effect(DataPtr data, const std::string& name) : Entity(data, name) , m_playingSound(false){

	setTexture(data->textureAssets.get(getName()));					
}

Effect::~Effect() { }

void Effect::update(float deltaTime) {
	
	if (!isActive()) return;

	if (!m_playingSound) {	
		m_sound.play();		
		m_playingSound = true;
	}
		
	m_animationController.update(deltaTime);

	if (m_animationController.getState() == AnimationController::AnimationState::Done && m_releaseCallback)
		m_releaseCallback(this);
}

void Effect::reset() {
	m_animationController.stop();	
	m_playingSound = false;
}

void Effect::createAnimation(int sizeX, int sizeY, int width, int height) {

	std::vector<sf::IntRect> frames;

	for (std::size_t y = 0; y < sizeY; y++)
		for (std::size_t x = 0; x < sizeX; x++)
			frames.push_back(sf::IntRect(width * x, height * y, width, height));

	AnimationPtr explosion = std::make_unique<Animation>();

	explosion->addFrames(frames);

	m_animationController.setAnimation(std::move(explosion));

	centerOrigin();
}



