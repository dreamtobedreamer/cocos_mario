#ifndef __ItemFinalPoint_H__
#define __ItemFinalPoint_H__
#include "Item.h"

class ItemFinalPoint :public Item
{
public:
	static ItemFinalPoint* create(CCDictionary* dict) {
		ItemFinalPoint* pRet = new ItemFinalPoint;
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
	// ½Ó¿Úº¯Êý
	void collision();
};

#endif