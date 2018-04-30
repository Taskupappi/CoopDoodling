#ifndef INGAME_H
#define INGAME_H

#include "scene.h"

class InGame : public Scene 
{
public:
	InGame(Type type);
	~InGame();

	void init();
	void run();
	void uninit();
private:
};

#endif