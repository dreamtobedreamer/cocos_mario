#ifndef __ItemBoss_H__
#define __ItemBoss_H__
#include "Item.h"

class ItemBoss :public Item
{
public:
	static ItemBoss* create(CCDictionary* dict) {
		ItemBoss* pRet = new ItemBoss;
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
	// ½Ó¿Úº¯Êý
	void move(float dt);
	void collision();
};

#endif