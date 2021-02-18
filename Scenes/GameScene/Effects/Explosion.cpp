#include "Explosion.hpp"

Explosion::Explosion(DataPtr data) : Effect(data, CLASS_NAME(Explosion)) {

	const int ANIMATION_X = 8;
	const int ANIMATION_Y = 8;
	const int FRAME_SIZE_X = 512;
	const int FRAME_SIZE_Y = 512;

	m_animationController.setFrameTime(DEFAULT_FRAME_TIME);

	createAnimation(ANIMATION_X, ANIMATION_Y, FRAME_SIZE_X, FRAME_SIZE_Y);


	if (!data->soundAssets.loaded(CLASS_NAME(Explosion)))
		data->soundAssets.load(CLASS_NAME(Explosion), EXPLOSION_SOUND_PATH, false);


	m_sound.setBuffer(data->soundAssets.get(CLASS_NAME(Explosion)));

}



