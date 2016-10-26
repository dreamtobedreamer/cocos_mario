#ifndef __ItemTortoiseFly_H__
#define __ItemTortoiseFly_H__
#include "Item.h"

class ItemTortoiseFly :public Item
{
public:
	enum STATE{NORMAL,SLEEP,FLY};
	struct status {
		Common::DIR _dir;
		STATE _state;
	};
	static ItemTortoiseFly* create(CCDictionary* dict) {
		ItemTortoiseFly* pRet = new ItemTortoiseFly;
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
	struct status _status;
	struct status _backup;
	float _dis;
	float _curDis;
	// 接口函数
	void move(float dt);
	void collision();
};

#endif