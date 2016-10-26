#include "ItemTortoise.h"
#include "Mario.h"
bool ItemTortoise::init(CCDictionary* dict) {
	Item::init();
	// 初始化变量
	_speedH = 50;
	_speedAcc = 10;
	_speedDown = 10;
	_status._dir = Common::LEFT;
	_status._isSleep = false;
	_backup = _status;
	setPos(dict);
	createAnimationAndFrame();
	updateStatus();
	return true;
}

void ItemTortoise::move(float dt) {
	if (!isInWindow())
		return;
	if (memcmp(&_status, &_backup, sizeof(struct status)) != 0) {
		updateStatus();
		_backup = _status;
	}
	if (_status._isSleep)
		return;
	if (_status._dir == Common::LEFT)
	{
		if (canMoveLeft(dt))
			Common::moveNode(this, ccp(-dt*_speedH, 0));
		else
			_status._dir = Common::RIGHT;
	}
	else {
		if (canMoveRight(dt))
			Common::moveNode(this, ccp(dt*_speedH, 0));
		else
			_status._dir = Common::LEFT;
	}
	if (canMoveDown(dt)) {
		Common::moveNode(this, ccp(0, -_speedDown*dt));
		_speedDown += _speedAcc;
	}

}

void ItemTortoise::collision() {
	if (!isInWindow())
		return;
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
		{// 被踩了
			// mario踩住怪 被弹起
			_mario->_speedUp = (_mario->_speedDown - 50) > 0 ? _mario->_speedDown - 50 : 10;
			_mario->_speedDown = 0;

			if (!_status._isSleep) {
				_status._isSleep = true;
				scheduleOnce(schedule_selector(ItemTortoise::sleep),5.0f);
			}
			else {
				// 乌龟怪死亡并播放死亡动画
				stopAllActions();
				unscheduleAllSelectors();
				CCMoveBy* act2 = CCMoveBy::create(1.0f, ccp(0, -winSize.height));
				CCCallFunc* act3 = CCCallFunc::create(this, 
					callfunc_selector(ItemTortoise::removeFromParent));
				runAction(CCSequence::create(act2, act3, NULL));
			}
		}
		else {// _mario被踩了
			if (_status._isSleep) {
				// 乌龟怪死亡并播放死亡动画
				stopAllActions();
				unscheduleAllSelectors();
				CCMoveBy* act2 = CCMoveBy::create(1.0f, ccp(0, -winSize.height));
				CCCallFunc* act3 = CCCallFunc::create(this,
					callfunc_selector(ItemTortoise::removeFromParent));
				runAction(CCSequence::create(act2, act3, NULL));
			}
			else {
				_mario->die(false);
			}
		}
	}
}

void ItemTortoise::sleep(float) {
	_status._isSleep = false;
}

void ItemTortoise::updateStatus() {
	stopAllActions();

	if (_status._isSleep)
	{
		CCAnimate* animate = CCAnimate::create(
			CCAnimationCache::sharedAnimationCache()->animationByName("TortoiseSleep"));
		runAction(CCRepeatForever::create(animate));
		return;
	}
	if (Common::LEFT == _status._dir)
	{
		CCAnimate* animate = CCAnimate::create(
			CCAnimationCache::sharedAnimationCache()->animationByName("TortoiseMoveLeft"));
		runAction(CCRepeatForever::create(animate));
		return;
	}
	else {
		CCAnimate* animate = CCAnimate::create(
			CCAnimationCache::sharedAnimationCache()->animationByName("TortoiseMoveRight"));
		runAction(CCRepeatForever::create(animate));
		return;
	}
}

void ItemTortoise::createAnimationAndFrame() {
	// TortoiseMoveLeft
	// TortoiseMoveRight
	// TortoiseSleep
	CCAnimationCache::sharedAnimationCache()->addAnimation(
		Common::createAnimation("tortoise0.png", 2, 3, 18, .1f),"TortoiseMoveLeft");
	CCAnimationCache::sharedAnimationCache()->addAnimation(
		Common::createAnimation("tortoise0.png", 4, 5, 18, .1f), "TortoiseMoveRight");
	CCAnimationCache::sharedAnimationCache()->addAnimation(
		Common::createAnimation("tortoise0.png", 8, 9, 18, .1f), "TortoiseSleep");
}