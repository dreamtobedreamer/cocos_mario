#ifndef __LayerGame_H__
#define __LayerGame_H__

#include "Common.h"
class ProgressBar;
class Mario;

class LayerGame : public CCLayer
{
public:
	static LayerGame* create(int idx);
	bool init(int idx);
	void loadResource();
	void loadCallBack(CCObject*);
	void startGame();
	void checkGame(float);
	

	CCTMXTiledMap* _map;
	Mario* _mario;
	int _idx;
	ProgressBar* _bar;
	int _loadFileCount;
	int _totalFileCount;
};

#endif