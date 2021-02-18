#pragma once

#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/Shader.hpp>
#include <SFML/Graphics/Drawable.hpp>
#include "../../Core/SharedData.hpp"

class ParallaxBackground {

public:

	ParallaxBackground(DataPtr data);
	~ParallaxBackground() = default;
	ParallaxBackground(const ParallaxBackground&) = delete;
	ParallaxBackground& operator=(const ParallaxBackground&) = delete;

	void update();
	void draw();

private:

	const float OFFSET_X  = 0.0010f;
	const float OFFSET_Y = 0.0005f;

	const std::string OFFSET_X_NAME = "offset_x";
	const std::string OFFSET_Y_NAME = "offset_y";

	DataPtr m_data;
	sf::Sprite m_background;
	sf::Shader m_parallax;

	float m_offsetX;
	float m_offsetY;
};

