#ifndef __ItemFlyFish_H__
#define __ItemFlyFish_H__
#include "Item.h"

class ItemFlyFish :public Item
{
public:
	static ItemFlyFish* create(CCDictionary* dict) {
		ItemFlyFish* pRet = new ItemFlyFish;
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
	void updateStatus();
	// 控制变量
	Common::DIR _dir;
	Common::DIR _dirBackup;
	int _duration;
	int _offsetH;
	int _offsetV;
	bool _isFly;

	// 接口函数
	void move(float dt);
	void collision();
};

#endif