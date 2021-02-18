#include "Meteor.hpp"

/*
	meteor_[color]_[type]_[sprite]
*/
const std::unordered_map<std::string, sf::IntRect> Meteor::m_spritesheetFrames = {
		{"meteor_1_3_1", sf::IntRect(1, 1, 101, 84)},
		{"meteor_1_3_2", sf::IntRect(104, 1, 120, 98)},
		{"meteor_1_3_3", sf::IntRect(1, 101, 89, 82)},
		{"meteor_1_3_4", sf::IntRect(92, 101, 98, 96)},
		{"meteor_1_2_1", sf::IntRect(226, 1, 43, 43)},
		{"meteor_1_2_2", sf::IntRect(226, 46, 45, 40)},
		{"meteor_1_1_1", sf::IntRect(226, 88, 28, 28)},
		{"meteor_1_1_2", sf::IntRect(256, 88, 29, 26)},
		{"meteor_2_3_1", sf::IntRect(192, 118, 101, 84)},
		{"meteor_2_3_2", sf::IntRect(1, 199, 120, 98)},
		{"meteor_2_3_3", sf::IntRect(123, 204, 89, 82)},
		{"meteor_2_3_4", sf::IntRect(287, 1, 98, 96)},
		{"meteor_2_2_1", sf::IntRect(295, 99, 43, 43)},
		{"meteor_2_2_2", sf::IntRect(340, 99, 45, 40)},
		{"meteor_2_1_1", sf::IntRect(340, 141, 28, 28)},
		{"meteor_2_1_2", sf::IntRect(295, 171, 29, 26)}
};

Meteor::Meteor(DataPtr data, const sf::Vector2f& spawnPosition, const sf::Vector2f& direction, 
	const MeteorType type, const MeteorColor color) : Entity(data, CLASS_NAME(Meteor)),
m_enteredScreen(false)
{

	const std::string SPRITESHEET_PATH = SPRITES_PATH "meteors.png";
	
	
	if (!data->textureAssets.loaded(CLASS_NAME(Meteor)))
		data->textureAssets.load(CLASS_NAME(Meteor), SPRITESHEET_PATH, false);


	setTexture(data->textureAssets.get(CLASS_NAME(Meteor)));

	selectMeteor(data, type, color);
	setPosition(spawnPosition);
	
	centerOrigin();

	setup(data, direction);	
}

Meteor::~Meteor() { 		

	m_destroyCallback = NULL;	
	m_splitCallback = NULL;
}

void Meteor::update(float deltaTime) {

	if (!isActive()) return;
	
	rotate(m_rotation);		
	move(m_direction);
	setColliderPosition(getPosition());

	if (!outOfBounds() && !m_enteredScreen)
		m_enteredScreen = true;

	else if (outOfBounds() && m_enteredScreen)	
		setActive(false);			
}

void Meteor::explode(bool shouldScore) {
		
	const int SMALL_METEOR_SCORE = 25;
	const int MEDIUM_METEOR_SCORE = 50;
	const int BIG_METEOR_SCORE = 100;

	int score = 0;
	MeteorType splitType = MeteorType::Unknown;

	switch (m_type) {
		
		case MeteorType::Small:
			score = SMALL_METEOR_SCORE;			
		break;

		case MeteorType::Medium:
			score = MEDIUM_METEOR_SCORE;
			splitType = MeteorType::Small;

			break;

		case MeteorType::Big:
			score = BIG_METEOR_SCORE;			
			splitType = MeteorType::Medium;

			break;
	}


	if (m_splitCallback && splitType != MeteorType::Unknown)
		m_splitCallback(getPosition(), splitType, m_color);

	setActive(false);

	if (!shouldScore)
		score = 0;

	if (m_destroyCallback)
		m_destroyCallback(getPosition(),score);
}


void Meteor::checkCollision(Enemies& enemies) {

	if (!isActive()) return;
	
	for (auto& e : enemies) {

		if (!e || !e->isActive()) continue;

		if (isColliding(*e)) 
			explode(false);							
	}	
}


bool Meteor::enteredScreen() const 	{
	return m_enteredScreen;
}

void Meteor::selectMeteor(DataPtr data, const MeteorType type, const MeteorColor color) {

	const int TYPE_COUNT = 3;
	const int COLOR_COUNT = 2;

	const int BIG_METEOR_RADIUS = 40;
	const int  MEDIUM_METEOR_RADIUS = 20;
	const int SMALL_METEOR_RADIUS = 14;

	const int BIG_METEOR_SPRITE_COUNT = 4;
	const int MEDIUM_METEOR_SPRITE_COUNT = 2;
	const int SMALL_METEOR_SPRITE_COUNT = 2;

	m_type = type == MeteorType::Unknown ? static_cast<MeteorType>(data->random.getInt(TYPE_COUNT)) : type;
	m_color = color == MeteorColor::Unknown ? static_cast<MeteorColor>(data->random.getInt(COLOR_COUNT)) : color;

	int sprite = 1;

	switch (m_type) {

	case MeteorType::Small:

		sprite = data->random.getInt(SMALL_METEOR_SPRITE_COUNT);
		setRadius(SMALL_METEOR_RADIUS);

		break;

	case MeteorType::Medium:

		sprite = data->random.getInt(MEDIUM_METEOR_SPRITE_COUNT);
		setRadius(MEDIUM_METEOR_RADIUS);

		break;

	case MeteorType::Big:

		sprite = data->random.getInt(BIG_METEOR_SPRITE_COUNT);
		setRadius(BIG_METEOR_RADIUS);

		break;
	}

	std::string name = "meteor_" + std::to_string(static_cast<int>(m_color)) + "_" +
		std::to_string(static_cast<int>(m_type)) + "_" + std::to_string(sprite);

	setTextureRect(m_spritesheetFrames.at(name));
}

void Meteor::setup(DataPtr data, const sf::Vector2f& direction) {

	const int ROTATION = 4;
	const float MINIMUM_SPEED = 0.8f;
	const float MAXIMUM_SPEED = 3.25f;

	m_rotation = data->random.getInt(-ROTATION, ROTATION);

	if (m_rotation == 0) m_rotation = ROTATION;

	setColliderPosition(getPosition());

	float speed = data->random.getFloat(MINIMUM_SPEED, MAXIMUM_SPEED);

	m_direction = sf::Vector2f(direction.x * speed, direction.y * speed);
}
