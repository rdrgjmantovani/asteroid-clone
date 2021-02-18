#include "BaseGameScene.hpp"

BaseGameScene::BaseGameScene(DataPtr data) : m_data(data){ }

BaseGameScene::~BaseGameScene() {

	destroy();
}

void BaseGameScene::init() {
	std::cout << "[BaseGameScene] Init" << std::endl;
}

void BaseGameScene::update(float deltaTime) {
	std::cout << "[BaseGameScene] Update" << std::endl;
}

void BaseGameScene::draw() {
	std::cout << "[BaseGameScene] Draw" << std::endl;
}

void BaseGameScene::pause() {
	std::cout << "[BaseGameScene] Pause" << std::endl;
}

void BaseGameScene::resume() {
	std::cout << "[BaseGameScene] Resume" << std::endl;
}

void BaseGameScene::destroy() {
	std::cout << "[BaseGameScene] Destroy" << std::endl;

	m_data->clearAssets();

	m_data->window.clearCallbacks();
		
}




