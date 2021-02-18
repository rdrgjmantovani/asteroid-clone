#include "Random.hpp"

Random::Random() : m_seed(m_service()),
m_engine(m_seed)
{
}

Random::~Random() { }

int Random::getInt(int min, int max) {
     
    std::uniform_int_distribution<int> uid(min, max);
    
    return uid(m_engine);
}

int Random::getInt(int max) {        
    return getInt(1, max);
}

float Random::getFloat(float min, float max) {    
    std::uniform_real_distribution<float> urd(min, max);

    return urd(m_engine);

}
