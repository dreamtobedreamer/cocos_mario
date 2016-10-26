#ifndef __ItemTortoise_H__
#define __ItemTortoise_H__
#include "Item.h"

class ItemTortoise :public Item
{
public:
	static ItemTortoise* create(CCDictionary* dict) {
		ItemTortoise* pRet = new ItemTortoise;
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
	void sleep(float);
	// 控制变量
	struct status {
		Common::DIR _dir;
		bool _isSleep;
	};
	struct status _status;
	struct status _backup;

	// 接口函数
	void move(float dt);
	void collision();
};

#endif