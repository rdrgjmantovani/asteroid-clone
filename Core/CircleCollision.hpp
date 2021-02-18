#pragma once
#include <SFML/Graphics/Sprite.hpp>
#include <cmath>

#if _DEBUG
#include <SFML/Graphics/CircleShape.hpp>
#endif

class CircleCollision {

public:

	CircleCollision(int radius = 10);
	~CircleCollision() = default;
	CircleCollision(const CircleCollision&) = delete;
	CircleCollision& operator=(const CircleCollision&) = delete;

	const int getRadius() const;
	void setRadius(int radius);
	const sf::Vector2f& getColliderPosition() const;

	void setColliderPosition(const sf::Vector2f& position);
	bool isColliding(const CircleCollision& other);


#if _DEBUG
public:
	const sf::CircleShape& getDebugCircle() const;

#endif

private:

	int m_radius;
	sf::Vector2f m_position;

#if _DEBUG
	sf::CircleShape m_debugCircle;
#endif		
};



