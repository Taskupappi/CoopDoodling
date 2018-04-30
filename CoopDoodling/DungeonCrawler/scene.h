#ifndef SCENE_H
#define SCENE_H

#include "tile.h"
#include <vector>

class Scene
{
public:
	enum class Type {
		InGame
	};

	Scene(Type type):m_type(type){};
	~Scene() {};

	virtual void init() = 0;
	virtual void run() = 0;
	virtual void uninit() = 0;
	Type type() { return m_type; }
private:
	Type m_type;
};

#endif