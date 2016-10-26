#ifndef __ItemTortoiseRound_H__
#define __ItemTortoiseRound_H__
#include "Item.h"

class ItemTortoiseRound :public Item
{
public:
	static ItemTortoiseRound* create(CCDictionary* dict) {
		ItemTortoiseRound* pRet = new ItemTortoiseRound;
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
	enum STATE{NORMAL, SLEEP, CRAZY};
	struct status {
		Common::DIR _dir;
		STATE _state;
	};
	struct status _status;
	struct status _backup;
	float _dis;               // 设置巡逻范围
	float _totalDis;
	
	// 接口函数
	void move(float dt);
	void collision();
};

#endif