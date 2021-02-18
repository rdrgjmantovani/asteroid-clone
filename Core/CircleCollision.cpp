#include "CircleCollision.hpp"

CircleCollision::CircleCollision(int radius) : m_radius(radius) {

#if _DEBUG

    m_debugCircle.setRadius(radius);
    sf::FloatRect circleBounds =  m_debugCircle.getGlobalBounds();

    m_debugCircle.setOrigin(circleBounds.width / 2, circleBounds.height / 2);

    m_debugCircle.setFillColor(sf::Color::Transparent);
    m_debugCircle.setOutlineThickness(1);
    m_debugCircle.setOutlineColor(sf::Color::Green);
    
#endif
    
}

const int CircleCollision::getRadius() const {
    return m_radius;
}

void CircleCollision::setRadius(int radius) {

    m_radius = radius;
  
#if _DEBUG
    m_debugCircle.setRadius(radius);

    sf::FloatRect circleBounds = m_debugCircle.getGlobalBounds();

    m_debugCircle.setOrigin(circleBounds.width / 2, circleBounds.height / 2);
#endif
}

const sf::Vector2f& CircleCollision::getColliderPosition() const {
    
    return m_position;
}

void CircleCollision::setColliderPosition(const sf::Vector2f& position){
    m_position = position;
    
#if _DEBUG
    m_debugCircle.setPosition(m_position);
#endif 

    
}

bool CircleCollision::isColliding(const CircleCollision& other) {

    sf::Vector2f p1 = getColliderPosition();
    sf::Vector2f p2 = other.getColliderPosition();
    
    int r1 = getRadius();
    int r2 = other.getRadius();
 
    sf::Vector2f distance = sf::Vector2f(p2.x - p1.x, p2.y - p1.y);

    float magnitude = std::sqrt(distance.x * distance.x + distance.y * distance.y);
    
    return r1 + r2 >= magnitude;    
}



#if _DEBUG

const sf::CircleShape& CircleCollision::getDebugCircle() const {

    return m_debugCircle;
}

#endif