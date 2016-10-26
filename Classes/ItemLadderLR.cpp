#include "ItemLadderLR.h"
#include "Mario.h"
bool ItemLadderLR::init(CCDictionary* dict) {
	Item::init();
	_isOnThisLadder = false;
	_speedH = 50;
	_speedAcc = 10;
	_speedDown = 0;
	_dir = dict->valueForKey("LorR")->intValue() ? Common::LEFT : Common::RIGHT;
	_curDis = 0;
	_dis = dict->valueForKey("ladderDis")->floatValue();
	setPos(dict);
	createAnimationAndFrame();
	initWithSpriteFrameName("LadderLR");
	return true;
}

void ItemLadderLR::move(float dt) {
	if (_dir == Common::LEFT) {
		Common::moveNode(this, ccp(-_speedH*dt, 0));
		if (_isOnThisLadder)
			Common::moveNode(_mario, ccp(-_speedH*dt, 0));
	}
	else {
		Common::moveNode(this, ccp(_speedH*dt, 0));
		if (_isOnThisLadder)
			Common::moveNode(_mario, ccp(_speedH*dt, 0));
	}
	_curDis += _speedH*dt;
	if (_curDis >= _dis) {
		if (_dir == Common::LEFT)
			_dir = Common::RIGHT;
		else
			_dir = Common::LEFT;
		_curDis = 0;
	}
}

void ItemLadderLR::collision() {
	if (!isInWindow())
		return;
	if (isFarAwayMario()) {
		unscheduleUpdate();
		removeFromParent();
		return;
	}
	CCRect rcThis = boundingBox();
	CCRect rcMario = _mario->boundingBox();
	if (rcThis.intersectsRect(rcMario))
	{
		if (_mario->_speedUp > 0) {
			_mario->_speedDown = _mario->_speedUp;
			_mario->_speedUp = 0;
		}
		else {
			_mario->_isOnLadder = true;
			_isOnThisLadder = true;
			_mario->setPositionY(rcThis.getMaxY());
		}
	}
	else {
		if (_isOnThisLadder) { // ??????
			_mario->_isOnLadder = false;
			_isOnThisLadder = false;
		}	
	}
}

void ItemLadderLR::createAnimationAndFrame() {
	CCSpriteFrameCache::sharedSpriteFrameCache()->addSpriteFrame(
		Common::getFrame("ladder.png", 0, 48),"LadderLR");
}