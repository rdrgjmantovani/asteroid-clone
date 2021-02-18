#pragma once

#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/RenderTarget.hpp>


#include "../Core/SharedData.hpp"
#include "../Core/AnimationController.hpp"

class Entity : public sf::Sprite {

public:
	
	Entity(DataPtr data, const std::string& name = "DEFAULT");
	~Entity();
	Entity(const Entity&) = delete;
	Entity& operator=(const Entity&) = delete;
	
	virtual void update(float deltaTime) = 0;

	void setActive(bool active);
	bool isActive() const;

	const std::string& getName() const;	
	
	template<class T>
	void onRelease(void(T::* handler)(Entity*), T* instance);
		
	bool outOfBounds();
	virtual void reset();

protected:

	void centerOrigin();
	virtual void frameChanged(const sf::IntRect& rect);		

protected:

	AnimationController m_animationController;

	bool m_active;
	const std::string m_name;

	std::function<void(Entity*)> m_releaseCallback;

};

template<class T>
inline void Entity::onRelease(void(T::* handler)(Entity*), T* instance) {

	if(!m_releaseCallback)
		m_releaseCallback = std::bind(handler, instance, std::placeholders::_1);
}
