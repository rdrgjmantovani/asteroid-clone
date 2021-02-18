#include "Window.hpp"

Window::Window() :
	m_focused(false),
	m_closed(false)
{ }


void Window::create() {

	sf::Uint32 style = sf::Style::Close | sf::Style::None;

	const std::string NAME = "Asteroid Clone";

	m_window.create(sf::VideoMode(WINDOW_WIDTH, WINDOW_HEIGHT), NAME, style);

	m_focused = true;	
}

void Window::handleEvents() {

	sf::Event e;

	while (m_window.pollEvent(e)) {

		handleWindowEvents(e);

		for (auto& c : m_callbacks) {
	
			if (*c == e) {
				auto function = c->getCallback();
				function(e);				
			}
		}
		
	}
}

bool Window::isFocused() const {
	return m_focused;
}

bool Window::closed() const {
	return m_closed;
}

void Window::clear() {
	
	m_window.clear();
}

void Window::display() {

	m_window.display();
}

void Window::addCallback(EventCallback* callback) {

	m_callbacks.push_back(callback);
}

void Window::clearCallbacks() {

	for (auto it = m_callbacks.begin(); it != m_callbacks.end();) {

		delete* it;
		
		it = m_callbacks.erase(it);
	}
}

sf::RenderWindow& Window::getRenderWindow() {

	return m_window;
}

void Window::draw(const sf::Drawable& d) {

	m_window.draw(d);
}

void Window::draw(const sf::Drawable& d, const sf::Shader& shader) {

	m_window.draw(d, &shader);
}

void Window::handleWindowEvents(sf::Event& e) {

	if (e.type == sf::Event::Closed) {
		m_window.close();

		m_closed = true;
	}

	if (!closed()) {

		if (e.type == sf::Event::LostFocus)
			m_focused = false;
		else if (e.type == sf::Event::GainedFocus)
			m_focused = true;
	}
	


}



