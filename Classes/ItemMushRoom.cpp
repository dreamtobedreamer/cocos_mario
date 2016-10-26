#include "ItemMushRoom.h"
#include "Mario.h"

bool ItemMushRoom::init(CCDictionary* dict) {
	Item::init();
	_speedH = 50;
	_speedAcc = 10;
	_speedDown = 10;
	_dir = Common::LEFT;
	_status._isDead = false;
	memcpy(&_backup, &_status, sizeof(struct status));
	createAnimationAndFrame();
	setPos(dict);
	// ¶¯»­
	CCAnimation* animation = CCAnimationCache::sharedAnimationCache()->
		animationByName("MushRoomMove");
	CCAction* action = CCRepeatForever::create(CCAnimate::create(animation));
	runAction(action);
	return true;
}

void ItemMushRoom::move(float dt) {
	if (!isInWindow())
		return;
	if (_status._isDead)
		return;
	if (_dir == Common::LEFT)
	{
		if (canMoveLeft(dt))
			Common::moveNode(this, ccp(-dt*_speedH, 0));
		else
			_dir = Common::RIGHT;
	}
	if (_dir == Common::RIGHT)
	{
		if (canMoveRight(dt))
			Common::moveNode(this, ccp(dt*_speedH, 0));
		else
			_dir = Common::LEFT;
	}
	
	if (canMoveDown(dt))
	{
		Common::moveNode(this, ccp(0, -_speedDown*dt));
		_speedDown += _speedAcc;
	}
}

void ItemMushRoom::collision() {
	if (!isInWindow())
		return;
	if (isFarAwayMario())
	{
		unscheduleAllSelectors();
		removeFromParent();
		return;
	}
	if (_status._isDead)
		return;
	CCRect rcMario = _mario->boundingBox();
	CCRect rcThis = boundingBox();
	if (rcMario.intersectsRect(rcThis))
	{
		if (_mario->_status._isFly &&
			_mario->_speedUp <= 0 && 
			rcMario.getMinY() > rcThis.getMidY())
		{// ±»²ÈËÀÁË
			// ²¥·ÅÉùÒô
			SimpleAudioEngine::sharedEngine()->playEffect("CaiSiGuaiWu.ogg");
			// mario²È×¡¹Ö ±»µ¯Æð
			_mario->_speedUp = (_mario->_speedDown - 50) > 0 ? _mario->_speedDown - 50 : 10;
			_mario->_speedDown = 0;
			// Ä¢¹½¹ÖËÀÍö²¢²¥·ÅËÀÍö¶¯»­
			_status._isDead = true;
			stopAllActions();
			CCAnimation* animation = CCAnimationCache::sharedAnimationCache()->
				animationByName("MushRoomDie");
			CCAnimate* animate = CCAnimate::create(animation);
			CCMoveBy* act2 = CCMoveBy::create(1.0f, ccp(0, -winSize.height));
			CCCallFunc* act3 = CCCallFunc::create(this, callfunc_selector(ItemMushRoom::removeFromParent));
			runAction(CCSequence::create(animate, act2, act3,NULL));
		}
		else {// _mario±»²ÈÁË
			_mario->die(false);
		}
	}
}



void ItemMushRoom::createAnimationAndFrame() {
	// MushRoomMove
	CCAnimationCache::sharedAnimationCache()->addAnimation(
		Common::createAnimation("Mushroom0.png",0,1,16,.1f),
		"MushRoomMove");
	// MushRoomDie
	CCAnimationCache::sharedAnimationCache()->addAnimation(
		Common::createAnimation("Mushroom0.png", 2, 3, 16, .1f),
		"MushRoomDie");
}