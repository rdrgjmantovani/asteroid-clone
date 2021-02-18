#include "Animation.hpp"

Animation::~Animation(){

	m_frames.clear();
}

void Animation::addFrame(const sf::IntRect& rect) {

	m_frames.push_back(rect);
}

void Animation::addFrames(const std::vector<sf::IntRect>& rects) {

	m_frames.insert(m_frames.begin(), rects.begin(), rects.end());
}

std::size_t Animation::getSize() const  {
	
	return m_frames.size();
}

const sf::IntRect& Animation::getFrame(std::size_t n) {
	
	return m_frames[n];
}

