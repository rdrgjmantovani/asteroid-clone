#include "LifeCounter.hpp"

const std::unordered_map<std::string, sf::IntRect> LifeCounter::m_spritesheetFrames = {
		{"life_1", sf::IntRect(1, 1, 33, 26)},
		{"life_2", sf::IntRect(36, 1, 37, 26)},
		{"life_3", sf::IntRect(1, 29, 32, 26)}
};


LifeCounter::LifeCounter(DataPtr data, const Player& player) {

	const std::string SPRITESHEET_PATH = SPRITES_PATH "lives.png";

	if (!data->textureAssets.loaded(CLASS_NAME(LifeCounter)))
		data->textureAssets.load(CLASS_NAME(LifeCounter), SPRITESHEET_PATH, false);


	std::string name = "life_" + std::to_string(static_cast<int>(player.getShip()));

	const int MAX_LIVES = 3;
	const float LIVES_OFFSET_X = 0.05f;
	const float LIVES_OFFSET_Y = 0.1f;

	m_lives = MAX_LIVES;

	for (size_t i = 0; i < MAX_LIVES; i++)
	{
		std::unique_ptr<sf::Sprite> life = std::make_unique<sf::Sprite>(data->textureAssets.get(CLASS_NAME(LifeCounter)));

		life->setTextureRect(m_spritesheetFrames.at(name));

		sf::FloatRect bounds = life->getGlobalBounds();
		life->setPosition(WINDOW_WIDTH * LIVES_OFFSET_X + bounds.width * i, WINDOW_HEIGHT * LIVES_OFFSET_Y);

		m_sprites.push_back(std::move(life));
	}
}

LifeCounter::~LifeCounter() {	
	for (auto it = m_sprites.begin(); it != m_sprites.end();) {

		(*it).reset();

		it = m_sprites.erase(it);
	}
}

void LifeCounter::decrement() {

	if (m_lives <= 0) return;

	m_lives--;
	
	if (m_lives >= 0) {
		m_sprites.back().reset();
		m_sprites.pop_back();
	}
}

int LifeCounter::getLives() const {
	return m_lives;
}

void LifeCounter::draw(sf::RenderTarget& target, sf::RenderStates states) const {

	for (auto& s : m_sprites)
		target.draw(*s, states);
}
