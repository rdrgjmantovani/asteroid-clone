#pragma once

#include "BaseScene.hpp"

#include <memory>
#include <stack>



using Scene = std::unique_ptr<BaseScene>;

class SceneManager {
	
public:	
	SceneManager();

	SceneManager(const SceneManager&) = delete;
	SceneManager& operator=(const SceneManager&) = delete;

	void add(Scene scene, bool replacing = true);
	void remove();
	
	Scene& getActiveScene();

private:
	void handleChanges();

private:

	std::stack<Scene> m_scenes;

	Scene m_scene;
	
	bool m_removing;
	bool m_adding;
	bool m_replacing;
};

