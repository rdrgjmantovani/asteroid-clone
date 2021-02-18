#pragma once

#include "../../../Core/Entity.hpp"
#include "../../../Core/CircleCollision.hpp"

#include "Shooter.hpp"

class Enemy : public Entity, public CircleCollision, public Shooter {

public:

	Enemy(DataPtr data, const sf::Vector2f& spawnPosition, const sf::Vector2f& destination, const sf::Vector2f& direction);
	~Enemy();
	Enemy(const Enemy&) = delete;
	Enemy& operator=(const Enemy&) = delete;

	virtual void update(float deltaTime);
			
	void takeDamage();
	void explode();
	int getHealth() const;
	void updatePlayerPosition(const sf::Vector2f& position);

	template<class T>
	void onExplosion(void(T::* handler)(const sf::Vector2f&), T* instance);

	template<class T>
	void onHit(void(T::* handler)(const sf::Vector2f&), T* instance);
	
private:
	void setup(DataPtr data, const sf::Vector2f& destination, const sf::Vector2f& spawnPosition, const sf::Vector2f& direction);
	void shoot();		
private:

	static const int COLLISION_RADIUS = 43;
	static const int MAX_HEALTH = 3;

	bool m_shouldMove;

	float m_speed;	
	int m_rotation;
	int m_health;

	sf::Vector2f m_playerPosition;
	sf::Vector2f m_destination;
	sf::Vector2f m_direction;

	sf::Clock m_shootClock;
	float m_shootDelay;
	
	std::function<void(const sf::Vector2f&)> m_explosionCallback;
	std::function<void(const sf::Vector2f&)> m_hitCallback;

	static const std::unordered_map<std::string, sf::IntRect> m_spritesheetFrames;
	static const std::unordered_map<std::string, sf::Color> m_spritesheetColors;
};

template<class T>
inline void Enemy::onExplosion(void(T::* handler)(const sf::Vector2f&), T* instance) {
	m_explosionCallback = std::bind(handler, instance, std::placeholders::_1);
}

template<class T>
inline void Enemy::onHit(void(T::* handler)(const sf::Vector2f&), T* instance) {

	m_hitCallback = std::bind(handler, instance, std::placeholders::_1);
}


using Enemies = std::vector<std::unique_ptr<Enemy>>;