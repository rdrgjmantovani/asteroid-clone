#pragma once
#include "../../../Core/Effect.hpp"

#define EXPLOSION_PATH SPRITES_PATH "explosion.png"
#define EXPLOSION_SOUND_PATH SFX_PATH "explosion.ogg"

class Explosion : public Effect {

public:
	Explosion(DataPtr data);
	~Explosion() = default;
	Explosion(const Explosion&) = delete;
	Explosion& operator=(const Explosion&) = delete;

};

using Explosions = std::unordered_set<Explosion*>;