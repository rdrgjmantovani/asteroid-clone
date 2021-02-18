#include "Player.hpp"

const std::unordered_map<std::string, sf::IntRect> Player::m_spritesheetFrames = {
		{"player_1", sf::IntRect(1, 1, 99, 75)},
		{"player_2", sf::IntRect(1, 78, 112, 75)},
		{"player_3", sf::IntRect(102, 1, 98, 75)}
};

const std::unordered_map<std::string, sf::Color> Player::m_colors = {
		{"player_1",sf::Color(54, 187, 242)},
		{"player_2", sf::Color(222,83,44)},
		{"player_3", sf::Color(113,201,55)}
};

Player::Player(DataPtr data) : Entity(data, CLASS_NAME(Player)),
CircleCollision(COLLISION_RADIUS),
m_acceleration(0, 0),
m_rotation(0),
m_moving(false),
m_invulnerable(false),
m_started(false)
{

	const std::string SHOOT_SOUND_PATH = SFX_PATH "player_laser.ogg";
	const std::string SPRISHEET_PATH = SPRITES_PATH "players.png";

	m_ship = static_cast<Ship>(data->random.getInt(m_spritesheetFrames.size()));

	std::string name = "player_" + std::to_string(static_cast<int>(m_ship));


	if (!data->soundAssets.loaded(CLASS_NAME(Player)))
		data->soundAssets.load(CLASS_NAME(Player), SHOOT_SOUND_PATH, false);

	m_shootSound.setBuffer(data->soundAssets.get(CLASS_NAME(Player)));

	if (!data->textureAssets.loaded(CLASS_NAME(Player)))
		data->textureAssets.load(CLASS_NAME(Player), SPRISHEET_PATH, false);


	setTexture(data->textureAssets.get(CLASS_NAME(Player)));

	setTextureRect(m_spritesheetFrames.at(name));

	m_color = m_colors.at(name);

	centerOrigin();

	setPosition(sf::Vector2f(WINDOW_WIDTH / 2, WINDOW_HEIGHT / 2));
	setColliderPosition(getPosition());

	updateRotation();
	updateDirection();
	updateShootPosition();

	data->window.addCallback(new EventCallback(sf::Keyboard::Key::Space, sf::Event::EventType::KeyPressed, &Player::shoot, this));
	data->window.addCallback(new EventCallback(sf::Mouse::Button::Left, sf::Event::EventType::MouseButtonPressed, &Player::shoot, this));

	m_shootTimer.restart();
}

Player::~Player() {

	m_hitCallback = NULL;
}

void Player::update(float deltaTime) {

	if (!isActive()) return;

	const float ROTATION_SPEED = 2.2f;

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::W)) {
		m_started = true;
		m_moving = true;

		if (m_invulnerable)
			setInvulnerable(false);
	}
	else
		m_moving = false;


	if (sf::Keyboard::isKeyPressed(sf::Keyboard::A)) {
		m_rotation -= ROTATION_SPEED;
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::D)) {
		m_rotation += ROTATION_SPEED;
	}


	updateRotation();


	if (m_moving) {
		updateDirection();
		handleAcceleration();
	}

	sf::Vector2f velocity = m_direction + m_acceleration;


	if (m_started) {
		move(velocity);
		updateShootPosition();
		boundsCheck();
		setColliderPosition(getPosition());
	}

}


void Player::boundsCheck() {

	sf::FloatRect bounds = getGlobalBounds();


	sf::Vector2f position = getPosition();

	if (getPosition().x - bounds.width / 2 > WINDOW_WIDTH)
		position.x = -bounds.width / 2;

	else if (getPosition().x + bounds.width / 2 < 0)
		position.x = WINDOW_WIDTH + bounds.width / 2;

	if (getPosition().y + bounds.height / 2 < 0)
		position.y = WINDOW_HEIGHT + bounds.height / 2;

	else if (getPosition().y - bounds.height / 2 > WINDOW_HEIGHT)
		position.y = -bounds.height / 2;

	setPosition(position);
}


void Player::updateShootPosition() {
	sf::Vector2f direction = getShootDirection();

	m_shootPosition = sf::Vector2f(getPosition().x + (direction.x * COLLISION_RADIUS), getPosition().y + (direction.y * COLLISION_RADIUS));
}

void Player::checkCollision(Meteors& meteors) {

	if (!isActive()) return;

	for (auto& m : meteors) {

		if (!m || !m->isActive()) continue;

		if (isColliding(*m)) {

			if (!isInvulnerable() && isActive()) {
				m->explode(false);
				explode();

				break;
			}
				
			else if (isInvulnerable() && isActive())
				m->explode(false);										
		}		
	}
}

void Player::checkCollision(Enemies& enemies) {

	if (!isActive()) return;

	for (auto& e : enemies) {

		if (!e->isActive() || !e) continue;

		if (isColliding(*e)) {
			if (!isInvulnerable() && isActive()) {
				explode();

				break;
			}
				

			else if (isInvulnerable() && isActive()) {
				e->explode();
			}
		}		
	}
}



void Player::explode() {

	if (m_hitCallback)
		m_hitCallback(getPosition());

	setInvulnerable(true);
}

bool Player::started() const {

	return m_started;
}

void Player::setInvulnerable(bool value) {


	if (!isActive()) return;

	const sf::Uint8 INVULNERABLE_ALPHA = 125;
	const sf::Uint8 MAX_ALPHA = 255;

	sf::Uint8 alpha = MAX_ALPHA;

	m_invulnerable = value;

	sf::Color color = getColor();

	if (value) {
		alpha = INVULNERABLE_ALPHA;
		reset();
	}


	color.a = alpha;
	setColor(color);

}

bool Player::isInvulnerable() const {

	return m_invulnerable;
}

void Player::reset() {

	m_acceleration = sf::Vector2f(0, 0);
	m_rotation = 0;

	m_moving = false;
	m_started = false;

	setPosition(sf::Vector2f(WINDOW_WIDTH / 2, WINDOW_HEIGHT / 2));

	setColliderPosition(getPosition());

	updateRotation();
	updateDirection();
	updateShootPosition();
}

const Player::Ship Player::getShip() const {
	return m_ship;

}

sf::Vector2f Player::getShootDirection() {
	float x = std::cos(m_rotation * DEG_TO_RAD);
	float y = std::sin(m_rotation * DEG_TO_RAD);


	return sf::Vector2f(x, y);
}

void Player::shoot(const sf::Event& e) {

	if (!isActive()) return;

	const float SHOOT_DELAY = 0.2f;
	const float PROJECTILE_SPEED = 25.f;

	if (m_shootTimer.getElapsedTime().asSeconds() < SHOOT_DELAY || !m_started) return;

	sf::Vector2f direction = getShootDirection();

	if (m_shootCallback)
		m_shootCallback(PROJECTILE_SPEED, m_shootPosition, direction, m_rotation, m_color, true);

	m_shootSound.play();
	m_shootTimer.restart();

}

void Player::updateRotation() {

	const int SPRITE_ROTATION = 270;
	setRotation(m_rotation - SPRITE_ROTATION);
}

void Player::updateDirection() {

	m_direction.x = std::cos(m_rotation * DEG_TO_RAD);
	m_direction.y = std::sin(m_rotation * DEG_TO_RAD);
}

void Player::handleAcceleration() {
	const float VELOCITY_LIMIT = 6.2f;
	const float ACCELERATION_FACTOR = 0.025f;

	m_acceleration.x += m_direction.x * ACCELERATION_FACTOR;
	m_acceleration.y += m_direction.y * ACCELERATION_FACTOR;

	if (m_acceleration.x < -VELOCITY_LIMIT) m_acceleration.x = -VELOCITY_LIMIT;
	else if (m_acceleration.x > VELOCITY_LIMIT) m_acceleration.x = VELOCITY_LIMIT;

	if (m_acceleration.y < -VELOCITY_LIMIT) m_acceleration.y = -VELOCITY_LIMIT;
	else if (m_acceleration.y > VELOCITY_LIMIT) m_acceleration.y = VELOCITY_LIMIT;
}


