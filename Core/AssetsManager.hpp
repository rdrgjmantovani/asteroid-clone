#pragma once

#include <iostream>
#include <unordered_map>
#include <memory>
#include <stdexcept>

#include <SFML/Graphics/Texture.hpp>
#include <SFML/Audio/Music.hpp>
#include <SFML/Audio/SoundBuffer.hpp>
#include <SFML/Graphics/Font.hpp>

template<typename K,typename T>
class AssetManager {

	using AssetPtr = std::unique_ptr<T>;

public:
	AssetManager() = default;
	AssetManager(const AssetManager&) = delete;
	AssetManager& operator=(const AssetManager&) = delete;
		
	void load(const K& key, const std::string& path, bool deleteOnClear = true);
	bool loaded(const K& key) const;

	T& get(const K& key);
	
	void clear();
	void clearAll();
	
private:

	std::unordered_map<K, AssetPtr> m_assets;
	std::vector<K> m_clearList;

};

template<typename K, typename T>
inline void AssetManager<K, T>::load(const K& key, const std::string& path, bool deleteOnClear) {
		
	AssetPtr ptr(new T);
	
	if (!ptr->loadFromFile(path))
		throw std::runtime_error("[Generic AssetManager] Could not Load " + path);

	std::cout << "[GenericAssetManager]" << " Loaded " << path << std::endl;

	m_assets.emplace(key, std::move(ptr));


	if (deleteOnClear)
		m_clearList.push_back(key);	
}

template<typename K, typename T>
inline bool AssetManager<K, T>::loaded(const K& key) const {

	return m_assets.find(key) != m_assets.end();
}

template<typename K, typename T>
inline T& AssetManager<K, T>::get(const K& key) {
	
	auto itr = m_assets.find(key);

	if (itr == m_assets.end())
		throw std::runtime_error("[GenericAssetManager] Invalid Key " + key);


	return *m_assets.at(key);
}


template<typename K, typename T>
inline void AssetManager<K, T>::clear() {

	for (auto c : m_clearList) {
		m_assets.at(c).reset();
		m_assets.erase(c);		
	}

	m_clearList.clear();
}

template<typename K, typename T>
inline void AssetManager<K, T>::clearAll() {

	for (auto& a : m_assets) 
		a.second.reset();
	
	m_assets.clear();
}


//-----------------------------------------------------------------------------------



using MusicPtr = std::shared_ptr<sf::Music>;

template<typename K>
class AssetManager<K, sf::Music> {	
public:

	AssetManager() = default;
	AssetManager(const AssetManager&) = delete;
	AssetManager& operator=(const AssetManager&) = delete;

	bool loaded(const K& key) const;
	void load(const K& key, const std::string& path, bool deleteOnClear = true);

	MusicPtr get(const K& key);

	void clear();
	void clearAll();

private:
	
	std::unordered_map<K, MusicPtr> m_musics;
	std::vector<K> m_clearList;
};

template<typename K>
inline bool AssetManager<K, sf::Music>::loaded(const K& key) const {

	return m_musics.find(key) != m_musics.end();
}


template<typename K>
inline void AssetManager<K, sf::Music>::load(const K& key, const std::string& path, bool deleteOnClear) {

	
	MusicPtr ptr(new sf::Music);

	if (!ptr->openFromFile(path))
		throw std::runtime_error("[MusicAssetManager] Could not Load " + path);

	std::cout << "[MusicAssetManager]" << " Loaded " << path << std::endl;

	m_musics.emplace(key, ptr);

	if (deleteOnClear)
		m_clearList.push_back(key);	
}

template<typename K>
inline MusicPtr AssetManager<K, sf::Music>::get(const K& key) {
	
	auto itr = m_musics.find(key);

	if (itr == m_musics.end())
		throw std::runtime_error("[MusicAssetManager] Invalid Key " + key);

	return m_musics.at(key);
}

template<typename K>
inline void AssetManager<K, sf::Music>::clear() {

	for (auto c : m_clearList) {
		m_musics.at(c).reset();
		m_musics.erase(c);
	}

	m_clearList.clear();	
}

template<typename K>
inline void AssetManager<K, sf::Music>::clearAll() {

	for (auto& m : m_musics) 
		m.second.reset();
	
	m_musics.clear();
}
