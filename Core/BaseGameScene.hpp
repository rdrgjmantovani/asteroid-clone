#pragma once

#include "BaseScene.hpp"
#include "SharedData.hpp"
#include <iostream>
#include "Entity.hpp"
#include <list>

class BaseGameScene : public BaseScene {

protected:

	BaseGameScene(DataPtr data);
	~BaseGameScene();

public:
	virtual void init();
	virtual void update(float deltaTime);
	virtual void draw();
	virtual void pause();
	virtual void resume();
	virtual void destroy();

protected:
	
	DataPtr m_data;

};

