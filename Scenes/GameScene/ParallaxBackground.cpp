#include "ParallaxBackground.hpp"



ParallaxBackground::ParallaxBackground(DataPtr data): m_data(data),
m_offsetX(0),
m_offsetY(0) {

	const std::string BACKGROUND_PATH = SPRITES_PATH "background.png";
	const std::string SHADER_PATH = SHADERS_PATH "parallax.vert";

	if(!m_data->textureAssets.loaded(CLASS_NAME(ParallaxBackground)))
		m_data->textureAssets.load(CLASS_NAME(ParallaxBackground), BACKGROUND_PATH, false);

	m_data->textureAssets.get(CLASS_NAME(ParallaxBackground)).setRepeated(true);
	m_background.setTexture(m_data->textureAssets.get(CLASS_NAME(ParallaxBackground)));
	

	sf::FloatRect backgroundBounds = m_background.getGlobalBounds();
	m_background.setScale(WINDOW_WIDTH / backgroundBounds.width, WINDOW_HEIGHT / backgroundBounds.height);

	m_parallax.loadFromFile(SHADER_PATH, sf::Shader::Vertex);
}

void ParallaxBackground::update() {

	m_offsetX += OFFSET_X;
	m_offsetY += OFFSET_Y;

	m_parallax.setUniform(OFFSET_X_NAME, m_offsetX);
	m_parallax.setUniform(OFFSET_Y_NAME, m_offsetY);
}

void ParallaxBackground::draw() {

	m_data->window.draw(m_background, m_parallax);
}



