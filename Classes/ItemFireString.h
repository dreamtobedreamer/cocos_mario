#ifndef __ItemFireString_H__
#define __ItemFireString_H__
#include "Item.h"

class ItemFireString :public Item
{
public:
	static ItemFireString* create(CCDictionary* dict) {
		ItemFireString* pRet = new ItemFireString;
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