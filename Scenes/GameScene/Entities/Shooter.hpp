#pragma once

#include <SFML/Graphics/Color.hpp>
#include <SFML/System/Vector2.hpp>
#include <functional>

#include <SFML/Audio/Sound.hpp>

class Shooter {

public:

	template<class T>
	void onShoot(void(T::* handler)(float,const sf::Vector2f&, const sf::Vector2f&, int, const sf::Color&, bool), T* instance);

protected:

	std::function<void(float, const sf::Vector2f&, const sf::Vector2f&, int, const sf::Color&, bool)> m_shootCallback;

	sf::Color m_color;
	sf::Sound m_shootSound;

};

template<class T>
inline void Shooter::onShoot(void(T::* handler)(float, const sf::Vector2f&, const sf::Vector2f&, int, const sf::Color&, bool), T* instance) {

	m_shootCallback = std::bind(handler, instance, std::placeholders::_1, std::placeholders::_2, 
		std::placeholders::_3, std::placeholders::_4, std::placeholders::_5,std::placeholders::_6);
}
