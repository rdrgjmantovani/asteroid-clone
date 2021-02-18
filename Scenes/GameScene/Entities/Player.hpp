#pragma once

#include "../../../Core/Entity.hpp"
#include "../../../Core/CircleCollision.hpp"

#include <SFML/Audio/Sound.hpp>
#include "Shooter.hpp"

#include "Meteor.hpp"
#include "Enemy.hpp"


class Player : public Entity , public CircleCollision, public Shooter {

	enum class Ship {
		Blue = 1,
		Red,
		Green
	};

public:

	Player(DataPtr data);
	~Player();
	Player(const Player&) = delete;
	Player& operator=(const Player&) = delete;
		
	virtual void update(float deltaTime);
	
	void boundsCheck();
	void updateShootPosition();

	void checkCollision(Meteors& meteors);
	void checkCollision(Enemies& enemies);			

	void explode();
	
	bool started() const;

	void setInvulnerable(bool value);
	bool isInvulnerable() const;

	virtual void reset() override;
	
	const Ship getShip() const;

	template<class T>
	void onHit(void(T::* handler)(const sf::Vector2f&), T* instance);
			
private:

	sf::Vector2f getShootDirection();
	void shoot(const sf::Event& e);
	void updateRotation();
	void updateDirection();
	void handleAcceleration();
		
private:

	Ship m_ship;

	static const int COLLISION_RADIUS = 34;
	
	bool m_invulnerable;
	bool m_moving;

	bool m_started;
	int m_rotation;

	sf::Vector2f m_direction;
	sf::Vector2f m_acceleration;
	sf::Vector2f m_shootPosition;
		
	sf::Clock m_shootTimer;		

	std::function<void(const sf::Vector2f&)> m_hitCallback;

	const static std::unordered_map<std::string, sf::IntRect> m_spritesheetFrames;	
	const static std::unordered_map<std::string, sf::Color> m_colors;	
};

template<class T>
inline void Player::onHit(void(T::* handler)(const sf::Vector2f&), T* instance) {

	m_hitCallback = std::bind(handler, instance, std::placeholders::_1);
}
