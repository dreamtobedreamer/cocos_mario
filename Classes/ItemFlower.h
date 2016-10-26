#ifndef __ItemFlower_H__
#define __ItemFlower_H__
#include "Item.h"

class ItemFlower :public Item
{
public:
	static ItemFlower* create(CCDictionary* dict) {
		ItemFlower* pRet = new ItemFlower;
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
	bool canDown(float);
	void wait(float dt);
	// ����
	Common::DIR _dir;
	int _startY;
	bool _bWait;
	// �ӿں���
	void move(float dt);
	void collision();
};

#endif