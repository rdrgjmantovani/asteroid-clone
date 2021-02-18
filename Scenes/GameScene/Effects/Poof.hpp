#pragma once
#include "../../../Core/Effect.hpp"

#define POOF_PATH SPRITES_PATH "poof.png"
#define POOF_SOUND_PATH  SFX_PATH "poof.ogg"

class Poof : public Effect{

public:

	Poof(DataPtr data);
	~Poof() = default;
	Poof(const Poof&) = delete;
	Poof& operator=(const Poof&) = delete;
	

};

using Poofs = std::unordered_set<Poof*>;