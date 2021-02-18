
#include "Entity.hpp"

Entity::Entity(DataPtr data, const std::string& name) : m_active(true),
m_name(name) {
	m_animationController.onFrameChanged(&Entity::frameChanged, this);
}

Entity::~Entity(){
	m_releaseCallback = NULL;
}

void Entity::setActive(bool active) {
	m_active = active;
}

bool Entity::isActive() const {
	return m_active;
}

const std::string& Entity::getName() const {	
	return m_name;
}

bool Entity::outOfBounds() {
	
	sf::Vector2f origin = getOrigin();
	sf::Vector2f position = getPosition();


	return position.x <= -origin.x || position.x >= WINDOW_WIDTH + origin.x ||
		position.y <= -origin.y || position.y >= WINDOW_HEIGHT + origin.y;
}

void Entity::reset() {
	setPosition(sf::Vector2f(0, 0));
	setRotation(0);	
}

void Entity::centerOrigin() {	

	sf::FloatRect bounds = getGlobalBounds();	
	setOrigin(sf::Vector2f(bounds.width / 2, bounds.height / 2));
	
}

void Entity::frameChanged(const sf::IntRect& rect) {			
	setTextureRect(rect);
}
