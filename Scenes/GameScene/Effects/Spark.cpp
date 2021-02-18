#include "Spark.hpp"

Spark::Spark(DataPtr data) : Effect(data, CLASS_NAME(Spark)) {

	const float FRAME_TIME = 0.025f;

	const int ANIMATION_X = 5;
	const int ANIMATION_Y = 1;
	const int FRAME_SIZE_X = 64;
	const int FRAME_SIZE_Y = 64;
			
	m_animationController.setFrameTime(FRAME_TIME);

	createAnimation(ANIMATION_X, ANIMATION_Y, FRAME_SIZE_X, FRAME_SIZE_Y);
		
	if (!data->soundAssets.loaded(CLASS_NAME(Spark)))
		data->soundAssets.load(CLASS_NAME(Spark), SPARK_SOUND_PATH, false);

	m_sound.setBuffer(data->soundAssets.get(CLASS_NAME(Spark)));	
}

