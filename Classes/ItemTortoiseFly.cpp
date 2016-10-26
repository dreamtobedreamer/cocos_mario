#include "ItemTortoiseFly.h"
#include "Mario.h"
bool ItemTortoiseFly::init(CCDictionary* dict) {
	Item::init();
	_speedH = 50;
	_speedAcc = 10;
	_speedDown = 0;

	_status._dir = Common::DOWN;
	_status._state = FLY;
	_backup = _status;
	_dis = dict->valueForKey("flyDis")->floatValue();
	_curDis = 0;

	setPos(dict);
	createAnimationAndFrame();
	updateStatus();
	return true;
}

void ItemTortoiseFly::move(float dt) {
	if (_status._state == SLEEP)
		return;
	
	if (_status._state == FLY) {
		if (_status._dir == Common::DOWN)
			Common::moveNode(this,ccp(0,-_speedH*dt));
		else
			Common::moveNode(this, ccp(0,_speedH*dt));
		_curDis += _speedH*dt;
		if (_curDis > _dis) {
			if (_status._dir == Common::UP)
				_status._dir = Common::DOWN;
			else
				_status._dir = Common::UP;
			_curDis = 0;
		}
	}
	else {
		if (_status._dir == Common::LEFT)
			if (canMoveLeft(dt))
				Common::moveNode(this, ccp(-_speedH*dt, 0));
			else
				_status._dir = Common::RIGHT;
		else
			if (canMoveRight(dt))
				Common::moveNode(this, ccp(_speedH*dt, 0));
			else
				_status._dir = Common::LEFT;
		if (canMoveDown(dt)) {
			Common::moveNode(this, ccp(0, -_speedDown*dt));
			_speedDown += _speedAcc;
		}
	}
	if (memcmp(&_status, &_backup, sizeof(struct status)) != 0) {
		updateStatus();
		_backup = _status;
	}
	
}

void ItemTortoiseFly::collision() {
	if (isFarAwayMario())
	{
		unscheduleAllSelectors();
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
		{// ±»²ÈÁË
			// mario²È×¡¹Ö ±»µ¯Æð
			_mario->_speedUp = (_mario->_speedDown - 50) > 0 ? _mario->_speedDown - 50 : 10;
			_mario->_speedDown = 0;

			if (_status._state == FLY) {
				_status._state = NORMAL;
				_status._dir = Common::LEFT;
			}
			else if(_status._state == NORMAL) {
				_status._state = SLEEP;
				scheduleOnce(schedule_selector(ItemTortoiseFly::sleep), 5.0f);
			}
			else {
				// ÎÚ¹ê¹ÖËÀÍö²¢²¥·ÅËÀÍö¶¯»­
				stopAllActions();
				unscheduleAllSelectors();
				CCMoveBy* act2 = CCMoveBy::create(1.0f, ccp(0, -winSize.height));
				CCCallFunc* act3 = CCCallFunc::create(this,
					callfunc_selector(ItemTortoiseFly::removeFromParent));
				runAction(CCSequence::create(act2, act3, NULL));
			}
		}
		else {// _mario±»²ÈÁË
			if (_status._state == SLEEP) {
				// ÎÚ¹ê¹ÖËÀÍö²¢²¥·ÅËÀÍö¶¯»­
				stopAllActions();
				unscheduleAllSelectors();
				CCMoveBy* act2 = CCMoveBy::create(1.0f, ccp(0, -winSize.height));
				CCCallFunc* act3 = CCCallFunc::create(this,
					callfunc_selector(ItemTortoiseFly::removeFromParent));
				runAction(CCSequence::create(act2, act3, NULL));
			}
			else {
				_mario->die(false);
			}
		}
	}
}

void ItemTortoiseFly::createAnimationAndFrame() {
	// TortoiseMoveLeft
	// TortoiseMoveRight
	// TortoiseSleep
	// TortoiseFlyLeft
	// TortoiseFlyRight
	CCAnimationCache::sharedAnimationCache()->addAnimation(
		Common::createAnimation("tortoise0.png", 2, 3, 18, .1f), "TortoiseMoveLeft");
	CCAnimationCache::sharedAnimationCache()->addAnimation(
		Common::createAnimation("tortoise0.png", 4, 5, 18, .1f), "TortoiseMoveRight");
	CCAnimationCache::sharedAnimationCache()->addAnimation(
		Common::createAnimation("tortoise0.png", 0, 1, 18, .1f), "TortoiseFlyLeft");
	CCAnimationCache::sharedAnimationCache()->addAnimation(
		Common::createAnimation("tortoise0.png", 6, 7, 18, .1f), "TortoiseFlyRight");
	CCAnimationCache::sharedAnimationCache()->addAnimation(
		Common::createAnimation("tortoise0.png", 8, 9, 18, .1f), "TortoiseSleep");
}

void ItemTortoiseFly::sleep(float dt) {
	_status._state = NORMAL;
}

void ItemTortoiseFly::updateStatus() {
	stopAllActions();
	if (_status._state == FLY) {
		if (_status._dir == Common::DOWN) {
			CCAnimate* animate = CCAnimate::create(
				CCAnimationCache::sharedAnimationCache()->animationByName("TortoiseFlyLeft"));
			runAction(CCRepeatForever::create(animate));
		}
		else {
			CCAnimate* animate = CCAnimate::create(
				CCAnimationCache::sharedAnimationCache()->animationByName("TortoiseFlyRight"));
			runAction(CCRepeatForever::create(animate));
		}
	}
	else if(_status._state == SLEEP){
		CCAnimate* animate = CCAnimate::create(
			CCAnimationCache::sharedAnimationCache()->animationByName("TortoiseSleep"));
		runAction(CCRepeatForever::create(animate));
	}
	else {
		if (_status._dir == Common::LEFT) {
			CCAnimate* animate = CCAnimate::create(
				CCAnimationCache::sharedAnimationCache()->animationByName("TortoiseMoveLeft"));
			runAction(CCRepeatForever::create(animate));
		}
		else {
			CCAnimate* animate = CCAnimate::create(
				CCAnimationCache::sharedAnimationCache()->animationByName("TortoiseMoveRight"));
			runAction(CCRepeatForever::create(animate));
		}
	}
}