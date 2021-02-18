#include "Poof.hpp"

Poof::Poof(DataPtr data) : Effect(data, CLASS_NAME(Poof)) {
	
	const int ANIMATION_X = 6;
	const int ANIMATION_Y = 5;
	const int FRAME_SIZE_X = 256;
	const int FRAME_SIZE_Y = 256;

	const float SCALE = 0.75f;

	m_animationController.setFrameTime(DEFAULT_FRAME_TIME);
	
	createAnimation(ANIMATION_X, ANIMATION_Y, FRAME_SIZE_X, FRAME_SIZE_Y);	

	setScale(SCALE, SCALE);

	if (!data->soundAssets.loaded(CLASS_NAME(Poof)))
		data->soundAssets.load(CLASS_NAME(Poof), POOF_SOUND_PATH, false);

	m_sound.setBuffer(data->soundAssets.get(CLASS_NAME(Poof)));

}

