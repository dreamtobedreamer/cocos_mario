#ifndef __ItemMushroomAddLife_H__
#define __ItemMushroomAddLife_H__
#include "Item.h"

class ItemMushroomAddLife :public Item
{
public:
	static ItemMushroomAddLife* create(CCDictionary* dict) {
		ItemMushroomAddLife* pRet = new ItemMushroomAddLife;
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
	bool init(CCDictionary* dict);
	void createAnimationAndFrame();
	void autoRun();
	bool canDown(float);

	enum STATE{NORMAL,MOVE};
	Common::DIR _dir;
	STATE _state;
	bool _isGod;

	// ½Ó¿Úº¯Êý
	void move(float dt);
	void collision();
};

#endif