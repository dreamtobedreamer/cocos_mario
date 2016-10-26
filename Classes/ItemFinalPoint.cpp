#include "ItemFinalPoint.h"
#include "Mario.h"
bool ItemFinalPoint::init(CCDictionary* dict) {
	Item::init();
	_speedH = 100;
	_speedAcc = 10;
	_speedDown = 0;
	setContentSize(CCSize(16, 16));
	setPos(dict);

	return true;
}


void ItemFinalPoint::collision() {
	if (!isInWindow())
		return;
	CCRect rcThis = boundingBox();
	CCRect rcMario = _mario->boundingBox();
	if (rcThis.intersectsRect(rcMario)) {
		unscheduleUpdate();
		_mario->autoRunFinish();
	}
}