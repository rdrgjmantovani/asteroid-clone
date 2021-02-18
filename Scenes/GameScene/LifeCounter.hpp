#pragma once

#include <SFML/Graphics/Sprite.hpp>
#include "../../Core/SharedData.hpp"

#include "Entities/Player.hpp"

class LifeCounter : public sf::Drawable {

public:

	LifeCounter(DataPtr data, const Player& player);
	~LifeCounter();
	LifeCounter(const LifeCounter&) = delete;
	LifeCounter& operator = (const LifeCounter&) = delete;

	void decrement();
	int getLives() const;

private:

	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;

private:
	int m_lives;

	std::vector<std::unique_ptr<sf::Sprite>> m_sprites;
	const static std::unordered_map<std::string, sf::IntRect> m_spritesheetFrames;
};

