#include "ItemFlyFish.h"
#include "Mario.h"
bool ItemFlyFish::init(CCDictionary* dict) {
	Item::init();
	_speedH = 100;
	_speedAcc = 10;
	_speedDown = 0;
	
	_dirBackup = _dir;
	_isFly = false;
	_duration = dict->valueForKey("duration")->intValue();
	_offsetH = dict->valueForKey("offsetH")->intValue();
	_offsetV = dict->valueForKey("offsetV")->intValue();
	_dir = _offsetH > 0 ? Common::RIGHT : Common::LEFT;
	createAnimationAndFrame();
	setPos(dict);
	return true;
}

void ItemFlyFish::move(float dt) {
	if (getPositionX() > _mario->getPositionX())
		return;
	if (_isFly)
		return;
	_isFly = true;
	updateStatus();
	// ÈÃ·ÉÓã×ö±´Èû¶ûÇúÏßÔË¶¯
	ccBezierConfig c;
	c.controlPoint_1.x = CCRANDOM_0_1() * _offsetH;
	c.controlPoint_1.y = CCRANDOM_0_1() * _offsetV;
	c.controlPoint_2.x = CCRANDOM_0_1() * _offsetH;
	c.controlPoint_2.y = CCRANDOM_0_1() * _offsetV;
	c.endPosition = ccp(_offsetH, _offsetV);
	CCBezierBy* bezier = CCBezierBy::create(_duration, c);

	ccBezierConfig c1;
	c1.controlPoint_1.x = CCRANDOM_0_1() * _offsetH;
	c1.controlPoint_1.y = CCRANDOM_0_1() * _offsetV;
	c1.controlPoint_2.x = CCRANDOM_0_1() * _offsetH;
	c1.controlPoint_2.y = CCRANDOM_0_1() * _offsetV;
	c1.endPosition = ccp(_offsetH, -_offsetV);
	CCBezierBy* bezier1 = CCBezierBy::create(_duration, c1);

	CCCallFunc* callFunc = CCCallFunc::create(this, callfunc_selector(ItemFlyFish::removeFromParent));
	runAction(CCSequence::create(bezier, bezier1, callFunc, NULL));
}

void ItemFlyFish::collision() {
	if (!isInWindow())
		return;
	if (isFarAwayMario()) {
		unscheduleUpdate();
		removeFromParent();
		return;
	}
	CCRect rcThis = boundingBox();
	CCRect rcMario = _mario->boundingBox();
	if (rcThis.intersectsRect(rcMario)) {
		unscheduleUpdate();
		removeFromParent();
		_mario->die(false);
	}
}

void ItemFlyFish::updateStatus() {
	stopAllActions();

	if (_dir == Common::LEFT)
		runAction(CCRepeatForever::create(
			CCAnimate::create(
				CCAnimationCache::sharedAnimationCache()->
					animationByName("FlyFishLeft"))));
	else
		runAction(CCRepeatForever::create(
			CCAnimate::create(
				CCAnimationCache::sharedAnimationCache()->
					animationByName("FlyFishRight"))));
}

void ItemFlyFish::createAnimationAndFrame() {
	// FlyFishLeft
	// FlyFishRight
	CCAnimation* animation;

	animation = Common::createAnimation("flyFishLeft.png", 
		0, 5, 16, 0.01f);
	CCAnimationCache::sharedAnimationCache()->
		addAnimation(animation,"FlyFishLeft");

	animation = Common::createAnimation("flyFishRight.png",
		0, 5, 16, 0.01f);
	CCAnimationCache::sharedAnimationCache()->
		addAnimation(animation, "FlyFishRight");
}