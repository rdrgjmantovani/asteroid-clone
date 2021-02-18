#include <random>

class Random {

    using engine = std::mt19937;

public:

    Random();
    ~Random();
        
    int getInt(int min, int max);
    int getInt(int max);

    float getFloat(float min, float max);
        
private:
    std::random_device m_service;
    const int m_seed;

    engine m_engine;    
};