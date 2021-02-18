#pragma once
#include <unordered_set>

#include "../../Core/BaseGameScene.hpp"
#include "../../Core/Fader.hpp"
#include "../../Core/EntityPool.hpp"

#include <SFML/Graphics/Text.hpp>

#include "ParallaxBackground.hpp"

#include "Effects/GameSceneEffects.hpp"
#include "Entities/GameSceneEntities.hpp"
#include "LifeCounter.hpp"

class MainScene : public BaseGameScene {
	
	enum class Corner {
		Top = 1,
		Bottom,
		Left,
		Right
	};
	
public:
	MainScene(DataPtr data);

	virtual void init() override;
	virtual void update(float deltaTime) override;			
	virtual void draw() override;
	virtual void destroy() override;

	static void preLoadTextures(DataPtr data);

private:

	void drawEntities();
	void drawEffects();
	void drawUI();

	void setupMusic();
	void setupTexts();		
	void waitFadeIn(float deltaTime);

	void handleProjectiles(float deltaTime);
	void handleMeteors(float deltaTime);
	void handleEnemies(float deltaTime);
	void handleEffects(float deltaTime);
	void handleCollisions();
	void handleMeteorCollision();
	void handleProjectileCollision();
	void handlePlayerCollision();

	void clearNullEntities();

	void handleMeteorSpawn();	
	void handleSplitMeteors();
	
	void loseLife(const sf::Vector2f& position);

	void gameOver();
	void spawnProjectile(float speed, const sf::Vector2f& shootPosition, const sf::Vector2f& direction, int rotation, const sf::Color& color, bool playerProjectile);
	void spawnExplosion(const sf::Vector2f& position);

	void createSplitMeteors(const sf::Vector2f& meteorPosition, const Meteor::MeteorType& type, const Meteor::MeteorColor& color);

	void score(const sf::Vector2f& position, int score);
	void updateScoreText();
	
	void spawnPoof(const sf::Vector2f& position);
	void spawnSpark(const sf::Vector2f& position);

	void spawnEnemy();

	sf::Vector2f getOffScreenPosition(const Corner& corner, const int offset);
	sf::Vector2f getRandomScreenPoint();
	Corner getRandomCorner();

	sf::Vector2f getDirection(const sf::Vector2f& p1, const sf::Vector2f& p2);

	void deleteEntities();
	void handleGameOver(float deltaTime);


#if _DEBUG
	void drawCollisionCircles();
#endif

private:

	const float FADE_TIME = 5.f;
	Fader m_fader;
	MusicPtr m_music;
		
	bool m_running;

	Player m_player;
	ParallaxBackground m_background;
	
	LifeCounter m_lifeCounter;
		
	bool m_gameOver;
	sf::Text m_gameOverText;
	
	int m_score;
	sf::Text m_scoreText;
		
	const int POINTS_TO_SPAWN_ENEMY = 300;
	int m_enemySpawnScoreTracker;
	Enemies m_enemies;
						
	sf::Clock m_meteorClock;
	Meteors m_meteors;
	Meteors m_splitMeteors;	 

	const float METEOR_SPAWN_TIMER = 1.15f;
	const int METEOR_SPLIT_SPAWN_COUNT = 3;
	
	//Pooled Entities

	EntityPool m_entityPool;

	Projectiles m_projectiles;
	Poofs m_poofs;
	Explosions m_explosions;
	Sparks m_sparks;

	//----------------------------------------------------							
};



