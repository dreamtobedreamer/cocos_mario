#ifndef __ItemMushRoom_H__
#define __ItemMushRoom_H__
#include "Item.h"

class ItemMushRoom :public Item
{
public:
	static ItemMushRoom* create(CCDictionary* dict) {
		ItemMushRoom* pRet = new ItemMushRoom;
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
	struct status {	
		bool _isDead;
	};
	Common::DIR _dir;

	bool init(CCDictionary* dict);
	void createAnimationAndFrame();
	
	// ½Ó¿Úº¯Êý
	void move(float dt);
	void collision();

	struct status _backup;
	struct status _status;
};

#endif