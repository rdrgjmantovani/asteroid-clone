#include "MainScene.hpp"

MainScene::MainScene(DataPtr data) : BaseGameScene(data),
m_fader(data, FADE_TIME),
m_gameOver(false),
m_player(data),
m_background(data),
m_score(0),
m_enemySpawnScoreTracker(POINTS_TO_SPAWN_ENEMY),
m_lifeCounter(data, m_player)
{

}

void MainScene::init() {

	setupMusic();
	preLoadTextures(m_data);
	setupTexts();

	m_player.onShoot(&MainScene::spawnProjectile, this);
	m_player.onHit(&MainScene::loseLife, this);

	m_meteorClock.restart();
}

void MainScene::update(float deltaTime) {

	waitFadeIn(deltaTime);

	m_background.update();

	if (!m_running) return;

	m_player.update(deltaTime);

	handleProjectiles(deltaTime);

	handleMeteorSpawn();

	handleMeteors(deltaTime);
	handleEnemies(deltaTime);

	handleCollisions();

	handleEffects(deltaTime);

	handleGameOver(deltaTime);

}

void MainScene::draw() {

	m_background.draw();

	drawEntities();

	if (m_player.isActive())
		m_data->window.draw(m_player);

	drawEffects();	

#if _DEBUG
	drawCollisionCircles();
#endif

	if (m_fader.getState() != Fader::State::FadedIn)
		m_data->window.draw(m_fader);

	drawUI();
}

void MainScene::destroy() {

	BaseGameScene::destroy();

	m_music.reset();

	m_entityPool.clear();

	m_projectiles.clear();
	m_poofs.clear();
	m_explosions.clear();
	m_sparks.clear();

	deleteEntities();
}

void MainScene::preLoadTextures(DataPtr m_data) {

	/* Large Texture Preload*/

	if (!m_data->textureAssets.loaded(CLASS_NAME(Poof)))
		m_data->textureAssets.load(CLASS_NAME(Poof), POOF_PATH, false);

	if (!m_data->textureAssets.loaded(CLASS_NAME(Explosion)))
		m_data->textureAssets.load(CLASS_NAME(Explosion), EXPLOSION_PATH, false);

	if (!m_data->textureAssets.loaded(CLASS_NAME(Spark)))
		m_data->textureAssets.load(CLASS_NAME(Spark), SPARK_PATH, false);
}



void MainScene::drawEntities() {

	for (auto p : m_projectiles) {

		if (p->isActive())
			m_data->window.draw(*p);
	}

	for (auto& m : m_meteors) {

		if (m && m->isActive())
			m_data->window.draw(*m);
	}

	for (auto& e : m_enemies) {

		if (e && e->isActive())
			m_data->window.draw(*e);
	}

}

void MainScene::drawEffects() {
	for (auto p : m_poofs) {

		if (p && p->isActive())
			m_data->window.draw(*p);
	}

	for (auto e : m_explosions) {

		if (e && e->isActive())
			m_data->window.draw(*e);
	}

	for (auto s : m_sparks) {

		if (s && s->isActive())
			m_data->window.draw(*s);
	}
}

void MainScene::drawUI() {
	m_data->window.draw(m_scoreText);

	m_data->window.draw(m_lifeCounter);

	if (m_gameOver)
		m_data->window.draw(m_gameOverText);

}


void MainScene::setupMusic() {

	const std::string MUSIC_PATH = MUSICS_PATH "fire-it-up.wav";
	const float VOLUME = 50.f;

	if (!m_data->musicAssets.loaded(CLASS_NAME(MainScene)))
		m_data->musicAssets.load(CLASS_NAME(MainScene), MUSIC_PATH, false);

	m_music = m_data->musicAssets.get(CLASS_NAME(MainScene));

	m_music->setVolume(VOLUME);
	m_music->setLoop(true);
	m_music->play();

}

void MainScene::setupTexts() {

	const std::string FONT_PATH = FONTS_PATH "astron_boy.ttf";
	const sf::String GAME_OVER_TEXT = "Game over";

	if (!m_data->fontAssets.loaded(CLASS_NAME(MainScene)))
		m_data->fontAssets.load(CLASS_NAME(MainScene), FONT_PATH, false);

	//Score
	const float SCORE_TEXT_OFFSET_X = 0.05f;
	const float SCORE_TEXT_OFFSET_Y = 0.03f;
	const int SCORE_TEXT_CHAR_SIZE = 45;

	m_scoreText.setFont(m_data->fontAssets.get(CLASS_NAME(MainScene)));
	m_scoreText.setCharacterSize(SCORE_TEXT_CHAR_SIZE);

	m_scoreText.setPosition(WINDOW_WIDTH * SCORE_TEXT_OFFSET_X, WINDOW_HEIGHT * SCORE_TEXT_OFFSET_Y);

	updateScoreText();

	//-----------------------------------------------------------------------------

	//GameOver
	const unsigned int GAME_OVER_CHAR_SIZE = 115;

	m_gameOverText.setFont(m_data->fontAssets.get(CLASS_NAME(MainScene)));
	m_gameOverText.setCharacterSize(GAME_OVER_CHAR_SIZE);

	m_gameOverText.setString(GAME_OVER_TEXT);

	sf::FloatRect gameOverBounds = m_gameOverText.getGlobalBounds();

	m_gameOverText.setPosition(WINDOW_WIDTH / 2, WINDOW_HEIGHT / 2);
	m_gameOverText.setOrigin(gameOverBounds.width / 2, gameOverBounds.height / 2);

	//-----------------------------------------------------------------------------
}


void MainScene::waitFadeIn(float deltaTime) {
	if (!m_running) {
		m_fader.update(deltaTime);

		if (m_fader.getState() == Fader::State::FadedIn)
			m_running = true;
	}
}

void MainScene::handleProjectiles(float deltaTime)
{
	for (auto p : m_projectiles)		
		p->update(deltaTime);	
}

void MainScene::handleMeteors(float deltaTime) {

	for (auto& m : m_meteors)
		m->update(deltaTime);

}

void MainScene::handleEnemies(float deltaTime) {

	for (auto& e : m_enemies) {

		e->updatePlayerPosition(m_player.getPosition());
		e->update(deltaTime);
	}
}

void MainScene::handleEffects(float deltaTime) {

	for (auto p : m_poofs) 		
		p->update(deltaTime);

	for (auto e : m_explosions) 		
		e->update(deltaTime);
	
	for (auto s : m_sparks) 		
		s->update(deltaTime);
	
}

void MainScene::handleCollisions()
{
	handlePlayerCollision();
	handleMeteorCollision();
	handleProjectileCollision();
	
	clearNullEntities();

	handleSplitMeteors();
}

void MainScene::handleMeteorCollision() {

	for (auto& m : m_meteors)
		m->checkCollision(m_enemies);				
}

void MainScene::handleProjectileCollision() {

	for (auto p : m_projectiles) {
		
		p->checkCollision(m_player, m_enemies);	
		p->checkCollision(m_meteors);
	}
}

void MainScene::handlePlayerCollision() {
	
	m_player.checkCollision(m_meteors);
	m_player.checkCollision(m_enemies);	
}

void MainScene::clearNullEntities() {

	for (Meteors::iterator it = m_meteors.begin(); it != m_meteors.end();) {

		if (!(*it)->isActive() || !*it) {

			(*it).reset();
			it = m_meteors.erase(it);
		}
		else
			it++;
	}

	for (Enemies::iterator it = m_enemies.begin(); it != m_enemies.end();) {

		if (!(*it)->isActive() || !*it) {

			(*it).reset();
			it = m_enemies.erase(it);
		}
		else
			it++;
	}
}

void MainScene::handleMeteorSpawn()
{
	if (!m_player.started() && !m_player.isInvulnerable() || !m_player.isActive()) {

		m_meteorClock.restart();
		return;
	}

	if (m_meteorClock.getElapsedTime().asSeconds() >= METEOR_SPAWN_TIMER) {

		const int METEOR_SPAWN_OFFSET = 200;

		Corner corner = getRandomCorner();

		sf::Vector2f offScreenPosition = getOffScreenPosition(corner, METEOR_SPAWN_OFFSET);
		sf::Vector2f screenPoint = getRandomScreenPoint();

		sf::Vector2f direction = getDirection(offScreenPosition, screenPoint);

		m_meteors.push_back(std::make_unique<Meteor>(m_data, offScreenPosition, direction));

		m_meteors.back()->onDestroy(&MainScene::score, this);
		m_meteors.back()->onSplit(&MainScene::createSplitMeteors, this);

		m_meteorClock.restart();
	}
}

void MainScene::handleSplitMeteors() {

	for (auto& m : m_splitMeteors)
		m_meteors.push_back(std::move(m));

	if (!m_splitMeteors.empty())
		m_splitMeteors.clear();
}


void MainScene::spawnProjectile(float speed, const sf::Vector2f& shootPosition, const sf::Vector2f& direction, int rotation, const sf::Color& color, bool playerProjectile) {

	Projectile* p = m_entityPool.get<Projectile>(CLASS_NAME(Projectile), m_data);

	p->setup(speed, shootPosition, direction, rotation, color, playerProjectile);
	p->onRelease(&EntityPool::release, &m_entityPool);
	
	m_projectiles.insert(p);
}

void MainScene::spawnExplosion(const sf::Vector2f& position) {

	Explosion* e = m_entityPool.get<Explosion>(CLASS_NAME(Explosion), m_data);

	e->setPosition(position);
	e->onRelease(&EntityPool::release, &m_entityPool);
	m_explosions.insert(e);
}

void MainScene::createSplitMeteors(const sf::Vector2f& meteorPosition, const Meteor::MeteorType& type, const Meteor::MeteorColor& color) {

	for (int i = 0; i < METEOR_SPLIT_SPAWN_COUNT; i++) {

		float angleRad = m_data->random.getInt(0, 360) * DEG_TO_RAD;

		float x = std::cos(angleRad);
		float y = -std::sin(angleRad);

		sf::Vector2f direction = sf::Vector2f(x, y);

		m_splitMeteors.push_back(std::make_unique<Meteor>(m_data, meteorPosition, direction, type, color));

		m_splitMeteors.back()->onDestroy(&MainScene::score, this);
		m_splitMeteors.back()->onSplit(&MainScene::createSplitMeteors, this);

	}
}

sf::Vector2f MainScene::getOffScreenPosition(const Corner& corner, const int offset) {

	int x = 0;
	int y = 0;

	switch (corner) {
	case Corner::Top:

		x = m_data->random.getInt(-offset, WINDOW_WIDTH + offset);
		y = -offset;

		break;

	case Corner::Bottom:

		x = m_data->random.getInt(-offset, WINDOW_WIDTH + offset);
		y = WINDOW_HEIGHT + offset;

		break;

	case Corner::Left:

		y = m_data->random.getInt(-offset, WINDOW_HEIGHT + offset);
		x = -offset;

		break;

	case Corner::Right:

		y = m_data->random.getInt(-offset, WINDOW_HEIGHT + offset);
		x = WINDOW_WIDTH + offset;

		break;
	}
	return sf::Vector2f(x, y);
}

sf::Vector2f MainScene::getRandomScreenPoint() {

	const float SCREEN_POINT_OFFSET = 0.2f;

	int x = m_data->random.getInt(0 + (WINDOW_WIDTH * SCREEN_POINT_OFFSET), WINDOW_WIDTH - (WINDOW_WIDTH * SCREEN_POINT_OFFSET));
	int y = m_data->random.getInt(0 + (WINDOW_HEIGHT * SCREEN_POINT_OFFSET), WINDOW_HEIGHT - (WINDOW_HEIGHT * SCREEN_POINT_OFFSET));

	return sf::Vector2f(x, y);
}

sf::Vector2f MainScene::getDirection(const sf::Vector2f& p1, const sf::Vector2f& p2) {

	float dx = p2.x - p1.x;
	float dy = p2.y - p1.y;

	float mag = std::sqrt(dx * dx + dy * dy);

	return sf::Vector2f(dx / mag, dy / mag);
}

MainScene::Corner MainScene::getRandomCorner() {

	Corner corner = static_cast<Corner>(m_data->random.getInt(4));

	return corner;
}

void MainScene::score(const sf::Vector2f& position, int score) {

	m_score += score;

	spawnPoof(position);

	updateScoreText();

	if (score != 0 && m_score >= m_enemySpawnScoreTracker)
		spawnEnemy();
}

void MainScene::spawnEnemy() {

	const int ENEMY_SPAWN_OFFSET = 100;

	m_enemySpawnScoreTracker += POINTS_TO_SPAWN_ENEMY;

	Corner corner = getRandomCorner();

	sf::Vector2f offScreenPosition = getOffScreenPosition(corner, ENEMY_SPAWN_OFFSET);
	sf::Vector2f screenPoint = getRandomScreenPoint();

	sf::Vector2f direction = getDirection(offScreenPosition, screenPoint);

	m_enemies.push_back(std::make_unique<Enemy>(m_data, offScreenPosition, screenPoint, direction));

	m_enemies.back()->onShoot(&MainScene::spawnProjectile, this);
	m_enemies.back()->onExplosion(&MainScene::spawnExplosion, this);
	m_enemies.back()->onHit(&MainScene::spawnSpark, this);

}


void MainScene::spawnPoof(const sf::Vector2f& position) {

	Poof* p = m_entityPool.get<Poof>(CLASS_NAME(Poof), m_data);

	p->setPosition(position);
	p->onRelease(&EntityPool::release, &m_entityPool);

	m_poofs.insert(p);
}

void MainScene::spawnSpark(const sf::Vector2f& position) {

	Spark* s = m_entityPool.get<Spark>(CLASS_NAME(Spark), m_data);

	s->setPosition(position);
	s->onRelease(&EntityPool::release, &m_entityPool);
	m_sparks.insert(s);
}

void MainScene::updateScoreText() {
	m_scoreText.setString("Score: " + std::to_string(m_score));
}

void MainScene::loseLife(const sf::Vector2f& position) {

	spawnExplosion(position);
	
	m_lifeCounter.decrement();

	if (m_lifeCounter.getLives() == 0)
		gameOver();		
}

void MainScene::gameOver() {

	m_gameOver = true;
	m_player.setActive(false);

	for (auto& e : m_enemies)
		e->explode();

	clearNullEntities();

	m_fader.setState(Fader::State::FadingOut);
}


void MainScene::deleteEntities() {

	for (Meteors::iterator it = m_meteors.begin(); it != m_meteors.end();) {

		(*it).reset();

		it = m_meteors.erase(it);
	}

	for (Meteors::iterator it = m_splitMeteors.begin(); it != m_splitMeteors.end();) {

		(*it).reset();

		it = m_splitMeteors.erase(it);
	}

	for (Enemies::iterator it = m_enemies.begin(); it != m_enemies.end();) {

		(*it).reset();

		it = m_enemies.erase(it);
	}
}

void MainScene::handleGameOver(float deltaTime) {

	if (!m_gameOver) return;

	if (m_gameOver && m_fader.getState() == Fader::State::FadingOut) {
		m_fader.update(deltaTime);
	}
	else if (m_gameOver && m_fader.getState() == Fader::State::Done) {
		destroy();
		m_data->sceneManager.add(Scene(new MainScene(m_data)));

	}
}


#if _DEBUG
void MainScene::drawCollisionCircles() {

	for (auto& p : m_projectiles)
		m_data->window.draw(p->getDebugCircle());

	for (auto& m : m_meteors)
		m_data->window.draw(m->getDebugCircle());

	for (auto& e : m_enemies)
		m_data->window.draw(e->getDebugCircle());

	m_data->window.draw(m_player.getDebugCircle());
}
#endif
