#pragma once

class BaseScene {

public:
	
	virtual void init() = 0;
	virtual void update(float deltaTime) = 0;
	virtual void draw() = 0;
	virtual void pause() = 0;
	virtual void resume() = 0;
	virtual void destroy() = 0;
	
};