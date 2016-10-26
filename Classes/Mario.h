#ifndef __Mario_H__
#define __Mario_H__
#include "Common.h"

class Mario : public CCSprite
{
public:
	struct status {
		Common::DIR _dirRun;     // 奔跑方向 Left Right 静止
		Common::DIR _dirFace;	 // 面向方向 Left Right
		bool _isBig;             // 是否变大
		int _life;               // 还有几条命
		bool _isFly;             // 判断是否在空中
		bool _isGod;             // 无敌状态
	};
	CREATE_FUNC(Mario);
	bool init();
	void createAnimate();
	void jump(float);
	void move(float);
	void moveLeft(float);
	void moveRight(float);
	void moveUp(float);
	void moveDown(float);
	bool canMoveLeft(float);
	bool canMoveRight(float);
	bool canMoveUp(float);
	bool canMoveDown(float);
	void updateStatus();
	void gameOver();
	void setGod(bool tf) { _status._isGod = tf; }
	void godTime(float dt) { _status._isGod = false; }
	void die(bool tf);
	void dead(float);
	void autoRun();
	void autoMove(float);
	void autoRunFinish() { _isAutoRunFinish = true; }
	bool missionComplete() { return _isMissionComplete; }
	void eatCoin(float);
	void hit(CCTMXLayer* layer, const char* name, CCPoint ptTile, int gid);
	CCTMXTiledMap* getMap() { return (CCTMXTiledMap*)getParent(); }

	Common::DIR _dir; // 控制变量,只是用来控制按键的变量 Left Right 静止
	int _speedH;      // 水平速度
	int _speedAccH;   // 水平加速度 未用到
	int _speedUp;     // 垂直向上速度
	int _speedDown;   // 垂直向下速度
	int _speedAccV;   // 垂直加速度
	bool _isOnLadder; // 是否在梯子上
	bool _isDead;     // 是否死了
	bool _isAutoRun;  // 自动行走
	bool _isAutoRunFinish; // 自动行走结束标志
	bool _isMissionComplete;// 是否通关

	struct status _status;
	struct status _backup;
};

#endif

