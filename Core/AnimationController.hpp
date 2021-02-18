#pragma once

#include "Animation.hpp"
#include <vector>
#include <memory>
#include <functional>


using AnimationPtr = std::unique_ptr<Animation>;

class AnimationController {
		
public:

	enum class AnimationState {
		Playing,
		Paused,
		Done
	};

	
	AnimationController(const float frameTime = 0.5f, const bool loop = false);
	~AnimationController();

	void update(float deltaTime);

	void play();
	void pause();
	void stop();

	void setAnimation(AnimationPtr a);
	void setFrameTime(float frameTime);
	void setLoop(bool loop);

	AnimationState getState() const;

	template<class T>
	void onFrameChanged(void(T::* handler)(const sf::IntRect&), T* instance);
	
private:


	AnimationState m_state;
	bool m_loop;

	bool m_paused;
	AnimationPtr m_animation;

	float m_frameTime;
	float m_time;
	std::size_t m_currentFrame;	

	std::function<void(const sf::IntRect&)> m_frameChangedCallback;	
};

template<class T>
inline void AnimationController::onFrameChanged(void(T::* handler)(const sf::IntRect&), T* instance) {

	m_frameChangedCallback = std::bind(handler, instance, std::placeholders::_1);
}

