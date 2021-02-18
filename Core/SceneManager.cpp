#include "SceneManager.hpp"

SceneManager::SceneManager() : m_adding(false),
m_removing(false),
m_replacing(false)
{ }

void SceneManager::add(Scene scene, bool replacing) {

	m_adding = true;

	m_replacing = replacing;
		
	m_scene = std::move(scene);

	handleChanges();
}

void SceneManager::remove() {
	m_removing = true;

	handleChanges();
}

Scene& SceneManager::getActiveScene() {	
	return m_scenes.top();
}


void SceneManager::handleChanges() {

	if (m_removing && !m_scenes.empty()) {

		m_scenes.pop();

		if (!m_scenes.empty())
			m_scenes.top()->resume();

		m_removing = false;
	}

	if (m_adding) {

		if (!m_scenes.empty()) {

			if (m_replacing)
				m_scenes.pop();


			else
				m_scenes.top()->pause();
		}

		m_scenes.push(std::move(m_scene));
		m_scenes.top()->init();

		m_adding = false;
	}


}