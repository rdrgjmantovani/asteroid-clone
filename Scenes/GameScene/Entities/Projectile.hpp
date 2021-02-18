#pragma once

#include <unordered_set>

#include "../../../Core/Entity.hpp"
#include "../../../Core/CircleCollision.hpp"
#include "Meteor.hpp"
#include "Player.hpp"
#include "Enemy.hpp"

class Projectile : public Entity, public CircleCollision {

public:

	Projectile(DataPtr data);
			
	void setup(float speed, const sf::Vector2f& position, const sf::Vector2f& direction, int rotation, const sf::Color& color, bool playerProjectile = false);
	virtual void update(float deltaTime);

	void checkCollision(Player& player, Enemies& enemies);
	void checkCollision(Meteors& meteors);
	
	bool playerProjectile() const;	

private:

	const int COLLISION_RADIUS = 5;
	
	bool m_playerProjectile;
	sf::Vector2f m_direction;
		
};

using Projectiles = std::unordered_set<Projectile*>;

