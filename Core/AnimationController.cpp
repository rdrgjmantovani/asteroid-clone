#include "AnimationController.hpp"

AnimationController::AnimationController(const float frameTime,  const bool loop) : m_frameTime(frameTime),
m_loop(loop),
m_currentFrame(0),
m_state(AnimationState::Paused)
{
}

AnimationController::~AnimationController() {
	m_animation.reset();
}

void AnimationController::update(float deltaTime) {

	
	if (m_state != AnimationState::Paused || m_state != AnimationState::Done && m_animation) {		

		m_state = AnimationState::Playing;

		m_time += deltaTime;
		
		if(m_time >= m_frameTime) {

			m_time = 0;

			m_currentFrame++;

			if (m_currentFrame >= m_animation->getSize()) {

				if (m_loop)
					m_currentFrame = 0;

				else {
					m_currentFrame--;				
					m_state = AnimationState::Done;							
				}
					
			}
						
			if (m_frameChangedCallback) 				
				m_frameChangedCallback(m_animation->getFrame(m_currentFrame));
			
		}
	}
}

void AnimationController::play() {

	m_state = AnimationState::Playing;
}

void AnimationController::pause() {

	m_state = AnimationState::Paused;
}

void AnimationController::stop() {
	m_state = AnimationState::Done;

	m_time = 0;
	m_currentFrame = 0;

}

void AnimationController::setAnimation(AnimationPtr a) {

	m_animation = std::move(a);

	if(m_frameChangedCallback)
		m_frameChangedCallback(m_animation->getFrame(0));

	m_state = AnimationState::Playing;
}

void AnimationController::setFrameTime(float frameTime) {

	m_frameTime = frameTime;
}

void AnimationController::setLoop(bool loop) {
	m_loop = loop;
}

AnimationController::AnimationState AnimationController::getState() const {	
	return m_state;
}
