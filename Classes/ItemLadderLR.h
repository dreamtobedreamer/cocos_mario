#ifndef __ItemLadderLR_H__
#define __ItemLadderLR_H__
#include "Item.h"

class ItemLadderLR :public Item
{
public:
	static ItemLadderLR* create(CCDictionary* dict) {
		ItemLadderLR* pRet = new ItemLadderLR;
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
	// ����
	Common::DIR _dir;
	float _dis;
	float _curDis;
	bool _isOnThisLadder;
	// �ӿں���
	void move(float dt);
	void collision();
};

#endif