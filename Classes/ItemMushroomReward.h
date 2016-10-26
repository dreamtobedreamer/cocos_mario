#ifndef __ItemMushroomReward_H__
#define __ItemMushroomReward_H__
#include "Item.h"

class ItemMushroomReward :public Item
{
public:
	static ItemMushroomReward* create(CCDictionary* dict) {
		ItemMushroomReward* pRet = new ItemMushroomReward;
		if (pRet && pRet->init(dict))
		{
			pRet->autorelease();
			return pRet;
		}
		else {
			delete pRet;
			return NULL;
		}
	}

	Common::DIR _dir;
	enum STATE{NORMAL,MOVE};
	STATE _state;
	bool _isGod;

	bool init(CCDictionary* dict);
	void createAnimationAndFrame();
	bool canDown(float dt);
	void autoRun() {
		_state = MOVE;
		_isGod = false;
	}
	// ½Ó¿Úº¯Êý
	void move(float dt);
	void collision();
};

#endif