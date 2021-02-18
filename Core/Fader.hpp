#pragma once

#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/RenderTarget.hpp>

#include "SharedData.hpp"

class Fader : public sf::Drawable{
	
public:

	enum class State {

		FadingIn,
		FadedIn,		
		FadingOut,		
		Done

	};

	Fader(DataPtr data, float seconds = 5.f);
	~Fader() = default;

	Fader(const Fader&) = delete;
	Fader& operator=(const Fader&) = delete;
	
	void update(float deltaTime);

	void setState(State state);
	State getState() const;

private:
	
	virtual void draw(sf::RenderTarget& target, sf::RenderStates state) const override;

private:
	
	const int MAX_ALPHA = 255;

	sf::Sprite m_fadeSprite;
	State m_state;

	float m_fadeSpeed;
	float m_alpha;
	
};

