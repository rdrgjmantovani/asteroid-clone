#pragma once

#include <vector>
#include <SFML/Graphics/Rect.hpp>

class Animation {


public:

	Animation() = default;
	~Animation();

	void addFrame(const sf::IntRect& rect);
	void addFrames(const std::vector<sf::IntRect>& rects);

	std::size_t getSize() const;
	const sf::IntRect& getFrame(std::size_t n);

private:	
	std::vector<sf::IntRect> m_frames;
};

