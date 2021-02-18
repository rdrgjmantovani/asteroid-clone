#pragma once

#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Window/Event.hpp>
#include <SFML/Graphics/Drawable.hpp>
#include <functional>
#include <vector>
#include "EventCallback.hpp"

#define WINDOW_WIDTH 1600
#define WINDOW_HEIGHT 900

class Window {

public:

	using EventCallbackFunc = std::function<void(const sf::Event&)>;

	Window();
	~Window() = default;

	void create();
	void handleEvents();

	bool isFocused() const;
	bool closed() const;

	void clear();
	void display();
	
	void addCallback(EventCallback* callback);
	void clearCallbacks();

	sf::RenderWindow& getRenderWindow();
	
	void draw(const sf::Drawable& d);
	void draw(const sf::Drawable& d, const sf::Shader& shader);

private:
	void handleWindowEvents(sf::Event& e);		
private:

	std::vector<EventCallback*> m_callbacks;

	sf::RenderWindow m_window;

	bool m_focused;
	bool m_closed;	
};


