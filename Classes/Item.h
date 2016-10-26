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
	// ��������
	Mario* _mario;
	// ���Ʊ���
	int _speedH;
	int _speedDown;
	int _speedAcc;

	// �ƶ�����
	bool canMoveDown(float);
	bool canMoveLeft(float);
	bool canMoveRight(float);

	// �ӿں���
	virtual void move(float dt) {};
	virtual void collision() {};

	// ��������
	CCTMXTiledMap* getMap() { return (CCTMXTiledMap*)getParent(); }
	bool isInWindow();
	bool isFarAwayMario();
	void setPos(CCDictionary* dict);
};

#endif
