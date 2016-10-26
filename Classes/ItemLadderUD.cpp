#include "ItemLadderUD.h"
#include "Mario.h"
bool ItemLadderUD::init(CCDictionary* dict) {
	Item::init();
	_speedH = 50;
	_speedAcc = 10;
	_speedDown = 0;
	_dir = dict->valueForKey("UorD")->intValue() ? Common::UP : Common::DOWN;
	_dis = dict->valueForKey("ladderDis")->floatValue();
	_curDis = 0;
	_isOnThisLadder = false;
	setPos(dict);
	createAnimationAndFrame();
	initWithSpriteFrameName("LadderUD");
	return true;
}

void ItemLadderUD::move(float dt) {
	if (_dir == Common::UP) {
		Common::moveNode(this, ccp(0,_speedH*dt));
		if (_isOnThisLadder)
			Common::moveNode(_mario, ccp(0,_speedH*dt));
	}
	else {
		Common::moveNode(this, ccp(0,-_speedH*dt));
		if (_isOnThisLadder)
			Common::moveNode(_mario, ccp(0,-_speedH*dt));
	}
	_curDis += _speedH*dt;
	if (_curDis >= _dis) {
		if (_dir == Common::UP)
			_dir = Common::DOWN;
		else
			_dir = Common::UP;
		_curDis = 0;
	}
}

void ItemLadderUD::collision() {
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

void ItemLadderUD::createAnimationAndFrame() {
	CCSpriteFrameCache::sharedSpriteFrameCache()->addSpriteFrame(
		Common::getFrame("ladder.png", 0, 48), "LadderUD");
}