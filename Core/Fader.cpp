#include "Fader.hpp"

Fader::Fader(DataPtr data, float seconds) : m_alpha(MAX_ALPHA){

	
	const std::string FADE_PATH = SPRITES_PATH "fade.png";

	if (!data->textureAssets.loaded(CLASS_NAME(Fader)))
		data->textureAssets.load(CLASS_NAME(Fader),FADE_PATH, false);


	m_fadeSprite.setTexture(data->textureAssets.get(CLASS_NAME(Fader)));

	sf::FloatRect bounds = m_fadeSprite.getGlobalBounds();

	m_fadeSprite.setScale(WINDOW_WIDTH / bounds.width, WINDOW_HEIGHT / bounds.height);


	m_fadeSpeed = MAX_ALPHA / seconds;

	setState(State::FadingIn);	
}

void Fader::update(float deltaTime) {


	if (m_state != State::Done) {

		sf::Color color = m_fadeSprite.getColor();

		if (m_state == State::FadingIn) {		
			m_alpha -= deltaTime * m_fadeSpeed;

			if (m_alpha <= 0) {
				setState(State::FadedIn);
				m_alpha = 0;
			}

			color.a = m_alpha;
			
		}

		else if (m_state == State::FadingOut) {
			
			m_alpha += deltaTime * m_fadeSpeed;

			if (m_alpha >= MAX_ALPHA) {
				setState(State::Done);
				m_alpha = MAX_ALPHA;
			}

			color.a = m_alpha;			
		}

		m_fadeSprite.setColor(color);
	}
	
}

void Fader::setState(State state) {

	m_state = state;
}

Fader::State Fader::getState() const {
	
	return m_state;
}

void Fader::draw(sf::RenderTarget& target, sf::RenderStates state) const {

	target.draw(m_fadeSprite, state);
}
