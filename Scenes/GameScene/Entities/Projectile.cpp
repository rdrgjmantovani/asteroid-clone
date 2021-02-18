#include "Projectile.hpp"

Projectile::Projectile(DataPtr data) : Entity(data, CLASS_NAME(Projectile)),
CircleCollision(COLLISION_RADIUS),
m_playerProjectile(false){

	const std::string SPRITE_PATH = SPRITES_PATH "projectile.png";

	if (!data->textureAssets.loaded(CLASS_NAME(Projectile)))
		data->textureAssets.load(CLASS_NAME(Projectile), SPRITE_PATH, false);

	setTexture(data->textureAssets.get(CLASS_NAME(Projectile)));

	centerOrigin();
}

void Projectile::setup(float speed, const sf::Vector2f& position, const sf::Vector2f& direction, int rotation, const sf::Color& color, bool playerProjectile) {

	const int SPRITE_ROTATION = 270;

	setPosition(position);

	setColliderPosition(getPosition());

	m_direction = sf::Vector2f(direction.x * speed, direction.y * speed);

	rotate(rotation - SPRITE_ROTATION);
	setColor(color);

	m_playerProjectile = playerProjectile;
}

void Projectile::update(float deltaTime) {

	if (!isActive()) return;

	move(m_direction);
	setColliderPosition(getPosition());

	if (outOfBounds() && m_releaseCallback)
		m_releaseCallback(this);
}


void Projectile::checkCollision(Player& player, Enemies& enemies) {

	if (!isActive()) return;

	bool collided = false;

	if (playerProjectile()) {

		for (auto& e : enemies) {

			if (!e->isActive() || !e) continue;

			if (isColliding(*e)) {
				e->takeDamage();

				collided = true;

				break;
			}			
		}
	}
	else {

		if (!player.isInvulnerable() && player.isActive()) {
			if (isColliding(player)) {
				player.explode();
				collided = true;
			}								
		}
	}	


	if (collided && m_releaseCallback)
		m_releaseCallback(this);
}

void Projectile::checkCollision(Meteors& meteors) {

	if (!isActive()) return;
	
	for (auto& m : meteors) {

		if (m->isActive() && !m->outOfBounds()) {

			if (isColliding(*m)) {
				m->explode(m_playerProjectile);

				if (m_releaseCallback)
					m_releaseCallback(this);

				break;
			}				
		}
	}
	
}

bool Projectile::playerProjectile() const {

	return m_playerProjectile;
}


