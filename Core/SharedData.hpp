#pragma once

#include <memory>

#include "SceneManager.hpp"
#include "Window.hpp"
#include "AssetsManager.hpp"
#include "Random.hpp"

#define MUSICS_PATH "Assets/Musics/"
#define SFX_PATH "Assets/SFX/"
#define SPRITES_PATH "Assets/Sprites/"
#define SHADERS_PATH "Assets/Shaders/"
#define FONTS_PATH "Assets/Fonts/"

#define CLASS_NAME(x) #x

#define PI  3.14159265359 
#define DEG_TO_RAD (PI / 180)
#define RAD_TO_DEG (180 / PI)

struct SharedData {
	SceneManager sceneManager;
	
	AssetManager<std::string, sf::Texture> textureAssets;
	AssetManager<std::string, sf::Music> musicAssets;
	AssetManager<std::string, sf::SoundBuffer> soundAssets;
	AssetManager<std::string, sf::Font> fontAssets;

	Window window;
	Random random;


	void clearAssets() {

		textureAssets.clear();
		musicAssets.clear();
		soundAssets.clear();
		fontAssets.clear();
	}

	void clearAllAssets() {
		textureAssets.clearAll();
		musicAssets.clearAll();
		soundAssets.clearAll();
		fontAssets.clearAll();
	}
};

using DataPtr = std::shared_ptr<SharedData>;

