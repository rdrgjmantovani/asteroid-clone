#include "EntityPool.hpp"

EntityPool::~EntityPool() {
	clear();	
}
void EntityPool::release(Entity* e) {	
	e->reset();
	e->setActive(false);
}

void EntityPool::clear() {

	for (auto it = m_pooledEntities.begin(); it != m_pooledEntities.end();) {
		
		delete *it;
		it = m_pooledEntities.erase(it);
	}
}

