#ifndef __Item_H__
#define __Item_H__
#include "Common.h"
class Mario;
class Item : public CCSprite
{
public:
	virtual ~Item(){}
	static Item* create(CCDictionary* dict);
	bool init() { 
		CCSprite::init(); 
		setZOrder(100); 
		_mario = NULL;
		_speedH = 50;
		_speedAcc = 10;
		_speedDown = 10;
		scheduleUpdate();
		return true; 
	}
	void update(float dt) {
		move(dt);
		collision();
	}
	// 其他变量
	Mario* _mario;
	// 控制变量
	int _speedH;
	int _speedDown;
	int _speedAcc;

	// 移动函数
	bool canMoveDown(float);
	bool canMoveLeft(float);
	bool canMoveRight(float);

	// 接口函数
	virtual void move(float dt) {};
	virtual void collision() {};

	// 辅助函数
	CCTMXTiledMap* getMap() { return (CCTMXTiledMap*)getParent(); }
	bool isInWindow();
	bool isFarAwayMario();
	void setPos(CCDictionary* dict);
};

#endif
