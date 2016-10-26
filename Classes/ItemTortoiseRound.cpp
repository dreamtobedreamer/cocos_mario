#include "ItemTortoiseRound.h"
#include "Mario.h"
bool ItemTortoiseRound::init(CCDictionary* dict) {
	Item::init();
	// 初始化
	_speedH = 50;
	_speedAcc = 10;
	_speedDown = 10;
	_dis = 0.0f;
	_totalDis = dict->valueForKey("roundDis")->floatValue();

	_status._dir = Common::LEFT;
	_status._state = STATE::NORMAL;
	_backup = _status;

	setPos(dict);
	createAnimationAndFrame();
	updateStatus();
	return true;
}

void ItemTortoiseRound::move(float dt) {
	if (!isInWindow())
		return;
	if (memcmp(&_status, &_backup, sizeof(struct status))) {
		updateStatus();
		_backup = _status;
	}
	if (_status._state == STATE::SLEEP)
		return;
	else if (_status._state == NORMAL) {
		if (Common::LEFT == _status._dir) {// 向左走
			_dis += _speedH*dt;
			if (_dis > _totalDis) {
				_status._dir = Common::RIGHT;
				_dis = 0;
			}
			else {
				Common::moveNode(this, ccp(-_speedH*dt, 0));
			}
		
		}
		else { // 向右走
			_dis += _speedH*dt;
			if (_dis > _totalDis) {
				_status._dir = Common::LEFT;
				_dis = 0;
			}
			else {
				Common::moveNode(this, ccp(_speedH*dt, 0));
			}
		}
	}
	else {// 疯狂状态
		if (Common::LEFT == _status._dir) {// 向左走
			if (canMoveLeft(dt))
				Common::moveNode(this, ccp(-_speedH*dt, 0));
			else
				_status._dir = Common::RIGHT;
		}
		else { // 向右走
			if (canMoveRight(dt))
				Common::moveNode(this, ccp(_speedH*dt, 0));
			else
				_status._dir = Common::LEFT;
		}
	}
	if (canMoveDown(dt)) { // 向下走
		Common::moveNode(this, ccp(0, -_speedDown*dt));
		_speedDown += _speedAcc;
	}
}

void ItemTortoiseRound::collision() {
	if (!isInWindow())
		return;
	if (isFarAwayMario()) {
		unscheduleUpdate();
		removeFromParent();
		return;
	}
	CCRect rcMario = _mario->boundingBox();
	CCRect rcThis = boundingBox();
	if (rcMario.intersectsRect(rcThis))
	{
		if (_mario->_status._isFly &&
			_mario->_speedUp <= 0 &&
			rcMario.getMinY() > rcThis.getMidY())
		{// 被踩了
			// mario踩住怪 被弹起
			_mario->_speedUp = (_mario->_speedDown - 50) > 0 ? _mario->_speedDown - 50 : 10;
			_mario->_speedDown = 0;

			if (_status._state == STATE::NORMAL) {
				_status._state = STATE::SLEEP;
				scheduleOnce(schedule_selector(ItemTortoiseRound::sleep), 5.0f);
			}
			else if (_status._state == STATE::SLEEP){
				unschedule(schedule_selector(ItemTortoiseRound::sleep));
				_status._state = STATE::CRAZY;
				_speedH = 200;
			}
			else {
				_status._state = NORMAL;
				_speedH = 50;
			}
		}
		else {// _mario被踩了
			if (_status._state == STATE::SLEEP) {
				_status._state = CRAZY;
				_speedH = 200;
			}
			else
				_mario->die(false);
		}
	}
}

void ItemTortoiseRound::sleep(float dt) {
	_status._state = STATE::NORMAL;
	_speedH = 50;
}


void ItemTortoiseRound::updateStatus() {
	stopAllActions();
	CCAnimate* animate;
		
	if (_status._state == STATE::CRAZY || _status._state == STATE::SLEEP) {
		CCAnimation* animation = CCAnimationCache::sharedAnimationCache()->
			animationByName("TortoiseSleep");
		animate = CCAnimate::create(animation);
		runAction(CCRepeatForever::create(animate));
		return;
	}

	if (_status._dir == Common::LEFT) {
		CCAnimation* animation = CCAnimationCache::sharedAnimationCache()->
			animationByName("TortoiseMoveLeft");
		animate = CCAnimate::create(animation);
	}
	else {
		CCAnimation* animation = CCAnimationCache::sharedAnimationCache()->
			animationByName("TortoiseMoveRight");
		animate = CCAnimate::create(animation);
	}
	runAction(CCRepeatForever::create(animate));
}

void ItemTortoiseRound::createAnimationAndFrame() {
	// TortoiseMoveLeft
	// TortoiseMoveRight
	// TortoiseSleep
	CCAnimationCache::sharedAnimationCache()->addAnimation(
		Common::createAnimation("tortoise0.png", 2, 3, 18, .1f), "TortoiseMoveLeft");
	CCAnimationCache::sharedAnimationCache()->addAnimation(
		Common::createAnimation("tortoise0.png", 4, 5, 18, .1f), "TortoiseMoveRight");
	CCAnimationCache::sharedAnimationCache()->addAnimation(
		Common::createAnimation("tortoise0.png", 8, 9, 18, .1f), "TortoiseSleep");
}