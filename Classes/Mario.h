#ifndef __Mario_H__
#define __Mario_H__
#include "Common.h"

class Mario : public CCSprite
{
public:
	struct status {
		Common::DIR _dirRun;     // ���ܷ��� Left Right ��ֹ
		Common::DIR _dirFace;	 // ������ Left Right
		bool _isBig;             // �Ƿ���
		int _life;               // ���м�����
		bool _isFly;             // �ж��Ƿ��ڿ���
		bool _isGod;             // �޵�״̬
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

	Common::DIR _dir; // ���Ʊ���,ֻ���������ư����ı��� Left Right ��ֹ
	int _speedH;      // ˮƽ�ٶ�
	int _speedAccH;   // ˮƽ���ٶ� δ�õ�
	int _speedUp;     // ��ֱ�����ٶ�
	int _speedDown;   // ��ֱ�����ٶ�
	int _speedAccV;   // ��ֱ���ٶ�
	bool _isOnLadder; // �Ƿ���������
	bool _isDead;     // �Ƿ�����
	bool _isAutoRun;  // �Զ�����
	bool _isAutoRunFinish; // �Զ����߽�����־
	bool _isMissionComplete;// �Ƿ�ͨ��

	struct status _status;
	struct status _backup;
};

#endif

