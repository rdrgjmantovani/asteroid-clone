#include "EventCallback.hpp"


EventCallback& EventCallback::operator=(const EventCallback& other) {
	
	m_event = other.m_event;
	m_eventType = other.m_eventType;

	return *this;
}

bool EventCallback::operator==(const sf::Event& event) const {	
	bool result = false;
	
	switch (m_type) {

		case Type::Keyboard:
			result = event.type == m_eventType && event.key.code == m_event.key.code;
			break;

		case Type::Mouse:
			result = event.type == m_eventType && event.mouseButton.button == m_event.mouseButton.button;
			break;
	}


	return result;
}

bool EventCallback::operator==(const EventCallback& other) const {
	
	bool result = false;

	if (other.m_type == m_type) {

		if (m_type == Type::Keyboard)
			result = m_event.key.code == other.m_event.key.code && m_eventType == other.m_eventType;

		else if (m_type == Type::Mouse)
			result = m_event.mouseButton.button == other.m_event.mouseButton.button && m_eventType == other.m_eventType;
	}


	return result;
}

EventCallback::Callback& EventCallback::getCallback() {
	
	return m_callback;
}
