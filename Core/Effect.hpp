#pragma once

#include "Entity.hpp"
#include <unordered_set>
#include <SFML/Audio/Sound.hpp>

class Effect : public Entity {

public:

	Effect(DataPtr data, const std::string& name);
	~Effect();
	Effect(const Effect&) = delete;
	Effect& operator=(const Effect&) = delete;

	virtual void update(float deltaTime);		
	virtual void reset() override;

protected:

	const float DEFAULT_FRAME_TIME = 0.016f;
	
	void createAnimation(int sizeX, int sizeY, int width, int height);
		
	sf::Sound m_sound;
	bool m_playingSound;
	
};

