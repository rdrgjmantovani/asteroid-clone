#pragma once

#include <vector>
#include "Entity.hpp"
 
class EntityPool {

public:
	EntityPool() = default;	
	EntityPool(const EntityPool&) = delete;
	EntityPool& operator=(const EntityPool&) = delete;
	~EntityPool();
	
	void release(Entity* e);

	template<class T = Entity>
	T* get(const std::string& name, DataPtr data);

	void clear();
	
private:	
	std::vector<Entity*> m_pooledEntities;
};


template<class T>
inline T* EntityPool::get(const std::string& name, DataPtr data) {

	
	for (Entity* e : m_pooledEntities) {

		if (e->getName().compare(name) == 0 && !e->isActive()) {
			e->setActive(true);

			e->reset();

			return static_cast<T*>(e);
		}
	}

	T* t = new T(data);

	t->reset();

	m_pooledEntities.push_back(t);

	return t;		
}
