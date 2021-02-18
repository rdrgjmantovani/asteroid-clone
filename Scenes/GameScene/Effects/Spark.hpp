#pragma once
#include "../../../Core/Effect.hpp"

#define SPARK_PATH SPRITES_PATH "spark.png"
#define SPARK_SOUND_PATH SFX_PATH "spark.ogg"

class Spark : public Effect {

public:
	Spark(DataPtr data);
	~Spark() = default;
	Spark(const Spark&) = delete;
	Spark& operator=(const Spark&) = delete;	
};
	
using Sparks = std::unordered_set<Spark*>;
	

	


