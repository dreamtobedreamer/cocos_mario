#ifndef __ItemLadderUD_H__
#define __ItemLadderUD_H__
#include "Item.h"

class ItemLadderUD :public Item
{
public:
	static ItemLadderUD* create(CCDictionary* dict) {
		ItemLadderUD* pRet = new ItemLadderUD;
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
	// 变量
	Common::DIR _dir;
	float _dis;
	float _curDis;
	bool _isOnThisLadder;
	// 接口函数
	void move(float dt);
	void collision();
};

#endif