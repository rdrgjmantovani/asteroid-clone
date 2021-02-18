#include "Enemy.hpp"

const std::unordered_map<std::string, sf::IntRect> Enemy::m_spritesheetFrames = {
		{"enemy_1", sf::IntRect(1, 1, 91, 91)},
		{"enemy_2", sf::IntRect(94, 1, 91, 91)},
		{"enemy_3", sf::IntRect(1, 94, 91, 91)},
		{"enemy_4", sf::IntRect(94, 94, 91, 91)}
};

const std::unordered_map<std::string, sf::Color> Enemy::m_spritesheetColors = {
		{"enemy_1", sf::Color(54, 187, 242)},
		{"enemy_2", sf::Color(113,201,55)},
		{"enemy_3", sf::Color(191,82,55)},
		{"enemy_4", sf::Color(255,204,0)}
};

Enemy::Enemy(DataPtr data, const sf::Vector2f& spawnPosition, const sf::Vector2f& destination, const sf::Vector2f& direction) : Entity(data, CLASS_NAME(Enemy)),
CircleCollision(COLLISION_RADIUS),
m_shouldMove(true),
m_health(MAX_HEALTH) {

	const std::string SPRITESHEET_PATH = SPRITES_PATH "enemies.png";
	const std::string SHOOT_SOUND_PATH = SFX_PATH "enemy_laser.ogg";

	if (!data->soundAssets.loaded(CLASS_NAME(Enemy)))
		data->soundAssets.load(CLASS_NAME(Enemy), SHOOT_SOUND_PATH, false);

	m_shootSound.setBuffer(data->soundAssets.get(CLASS_NAME(Enemy)));

	if (!data->textureAssets.loaded(CLASS_NAME(Enemy)))
		data->textureAssets.load(CLASS_NAME(Enemy), SPRITESHEET_PATH, false);

	setTexture(data->textureAssets.get(CLASS_NAME(Enemy)));

	setup(data, destination, spawnPosition, direction);

	m_shootClock.restart();

}

Enemy::~Enemy() {
	m_shootCallback = NULL;
	m_explosionCallback = NULL;
	m_hitCallback = NULL;
}

void Enemy::update(float deltaTime) {	
	rotate(m_rotation);
	
	sf::Vector2f position = getPosition();

	float dx = m_destination.x - position.x;
	float dy = m_destination.y - position.y;

	int distance = std::sqrt(dx * dx + dy * dy);

	if (distance < m_speed) {
		m_shouldMove = false;
		setPosition(m_destination);
		setColliderPosition(getPosition());
	}

	if (m_shouldMove) {
		move(m_direction);	
		setColliderPosition(getPosition());
	}

	else if (!m_shouldMove && m_shootClock.getElapsedTime().asSeconds() >= m_shootDelay) 
		shoot();	
}


void Enemy::takeDamage() {

	m_health--;

	if (m_health > 0 && m_hitCallback)
		m_hitCallback(getPosition());


	else if (m_health == 0) 
		explode();
	

}

void Enemy::explode() {
	m_health = 0;

	setActive(false);

	if (m_explosionCallback)
		m_explosionCallback(getPosition());
}

int Enemy::getHealth() const {
	return m_health;
}

void Enemy::updatePlayerPosition(const sf::Vector2f& position) {
	m_playerPosition = position;
}

void Enemy::setup(DataPtr data, const sf::Vector2f& destination, const sf::Vector2f& spawnPosition, const sf::Vector2f& direction) {

	std::string name = "enemy_" + std::to_string(data->random.getInt(m_spritesheetFrames.size()));

	setTextureRect(m_spritesheetFrames.at(name));

	m_color = m_spritesheetColors.at(name);

	centerOrigin();	
	setPosition(spawnPosition);

	m_destination = destination;

	const float MINIUM_SPEED = 3.5f;
	const float MAXIMUM_SPEED = 5.25f;	
	const float MINIUM_SHOOT_DELAY = 2.5f;
	const float MAXIMUM_SHOOT_DELAY = 3.5f;

	const int ROTATION = 4;

	m_speed = data->random.getFloat(MINIUM_SPEED, MAXIMUM_SPEED);

	m_direction = sf::Vector2f(direction.x * m_speed, direction.y * m_speed);
	
	setColliderPosition(getPosition());

	m_shootDelay = data->random.getFloat(MINIUM_SHOOT_DELAY, MAXIMUM_SHOOT_DELAY);

	m_rotation = data->random.getInt(-ROTATION, ROTATION);

	if (m_rotation == 0) m_rotation = ROTATION;
}


void Enemy::shoot() {

	if (m_shootCallback) {

		const float PROJECTILE_SPEED = 10.f;

		float dx = m_playerPosition.x - getPosition().x;		
		float dy = m_playerPosition.y - getPosition().y;

		float magnitude = std::sqrt(dx * dx + dy * dy);

		sf::Vector2f direction = sf::Vector2f(dx / magnitude, dy / magnitude);

		int rotation = std::atan2(direction.y, direction.x) * RAD_TO_DEG;

		m_shootCallback(PROJECTILE_SPEED, getPosition(), direction, rotation, m_color, false);

		m_shootSound.play();
	}

	m_shootClock.restart();
}
