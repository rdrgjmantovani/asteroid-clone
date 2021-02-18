#pragma once

#include <SFML/Window/Event.hpp>
#include <iostream>
#include <functional>

class EventCallback {

	enum class Type {
		Mouse,
		Keyboard
	};
		
public:

	using Callback = std::function<void(const sf::Event&)>;

			
	template<class T>
	EventCallback(const sf::Keyboard::Key& key, sf::Event::EventType eventType, void(T::* handler)(const sf::Event&), T* instance);
						
	template<class T>
	EventCallback(const sf::Mouse::Button& button, sf::Event::EventType eventType, void(T::* handler)(const sf::Event&), T* instance);
			
	EventCallback& operator=(const EventCallback& other);
	bool operator==(const sf::Event& event) const;
	bool operator==(const EventCallback& other) const;

	template<class T>
	void bindFunction(void(T::* handler)(const sf::Event&), T* instance);
			
	Callback& getCallback();

private:

	Callback m_callback;
	Type m_type;
	sf::Event m_event;
	sf::Event::EventType m_eventType;

};

template<class T>
inline EventCallback::EventCallback(const sf::Keyboard::Key& key, sf::Event::EventType eventType, void(T::* handler)(const sf::Event&), T* instance) {

	m_eventType = eventType;
	m_event.key.code = key;

	m_type = Type::Keyboard;

	bindFunction(handler, instance);
}

template<class T>
inline EventCallback::EventCallback(const sf::Mouse::Button& button, sf::Event::EventType eventType, void(T::* handler)(const sf::Event&), T* instance) {

	m_eventType = eventType;
	m_event.mouseButton.button = button;

	m_type = Type::Mouse;

	bindFunction(handler, instance);
}

template<class T>
inline void EventCallback::bindFunction(void(T::* handler)(const sf::Event&), T* instance) {

	m_callback = std::bind(handler, instance, std::placeholders::_1);

}
