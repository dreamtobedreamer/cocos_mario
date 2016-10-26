#ifndef __ItemFlagPoint_H__
#define __ItemFlagPoint_H__
#include "Item.h"

class ItemFlagPoint :public Item
{
public:
	static ItemFlagPoint* create(CCDictionary* dict) {
		ItemFlagPoint* pRet = new ItemFlagPoint;
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
	void collision();
};

#endif