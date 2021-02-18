#pragma once

#include "../../../Core/Entity.hpp"
#include "../../../Core/CircleCollision.hpp"
#include "Enemy.hpp"

class Meteor : public Entity, public CircleCollision {	

public:

	enum class MeteorType {
		Small = 1,
		Medium,
		Big,
		Unknown
	};

	enum class MeteorColor {
		Brown = 1,
		Grey,
		Unknown
	};

	Meteor(DataPtr data, const sf::Vector2f& spawnPosition, const sf::Vector2f& direction,
		const MeteorType type = MeteorType::Unknown, const MeteorColor color = MeteorColor::Unknown);

	~Meteor();
	Meteor(const Meteor&) = delete;
	Meteor& operator=(const Meteor&) = delete;

	virtual void update(float deltaTime);

	void explode(bool shouldScore = true);
	
	void checkCollision(Enemies& enemies);

	bool enteredScreen() const;

	template<class T>
	void onDestroy(void(T::* handler)(const sf::Vector2f&, int), T* instance);

	template<class T>
	void onSplit(void(T::* handler)(const sf::Vector2f&, const MeteorType&, const MeteorColor&), T* instance);

private:

	void selectMeteor(DataPtr data, const MeteorType type, const MeteorColor color);
	void setup(DataPtr data, const sf::Vector2f& direction);	

private:		

	MeteorType m_type;

	MeteorColor m_color;
	sf::Vector2f m_direction;

	int m_rotation;

	bool m_enteredScreen;

	std::function<void(const sf::Vector2f&, int)> m_destroyCallback;
	std::function<void(const sf::Vector2f&, const MeteorType&, const MeteorColor&)> m_splitCallback;

	static const std::unordered_map<std::string, sf::IntRect> m_spritesheetFrames;
};

template<class T>
inline void Meteor::onDestroy(void(T::* handler)(const sf::Vector2f&, int), T* instance) {

	m_destroyCallback = std::bind(handler, instance, std::placeholders::_1, std::placeholders::_2);
}

template<class T>
inline void Meteor::onSplit(void(T::* handler)(const sf::Vector2f&, const MeteorType&, const MeteorColor&), T* instance) {

	m_splitCallback = std::bind(handler, instance, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3);
}

using Meteors = std::vector<std::unique_ptr<Meteor>>;