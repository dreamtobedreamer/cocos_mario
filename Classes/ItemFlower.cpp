#include "ItemFlower.h"
#include "Mario.h"
bool ItemFlower::init(CCDictionary* dict) {
	Item::init();
	_speedH = 50;
	_speedAcc = 10;
	_speedDown = 50;
	_dir = Common::DOWN;
	_bWait = false;
	setPos(dict);
	_startY = getPositionY();
	setZOrder(0);
	Common::moveNode(this, ccp(-8, 0));
	createAnimationAndFrame();

	// 加载动画
	CCAnimate* animate = CCAnimate::create(
		CCAnimationCache::sharedAnimationCache()->animationByName("Flower"));
	runAction(CCRepeatForever::create(animate));
	return true;
}

void ItemFlower::move(float dt) {
	if (!isInWindow())
		return;
	if (_bWait)
		return;
	if (_dir == Common::DOWN) {
		if (canDown(dt))
			Common::moveNode(this, ccp(0, -_speedDown*dt));
		else
			_dir = Common::UP;
	}
	else {
		if (getPositionY() > _startY) {
			_bWait = true;
			scheduleOnce(schedule_selector(ItemFlower::wait), 1.0f);
			_dir = Common::DOWN;
		}
		else
			Common::moveNode(this, ccp(0, _speedDown*dt));
	}
}

void ItemFlower::wait(float dt) {
	_bWait = false;
}

void ItemFlower::collision() {
	if (!isInWindow())
		return;
	CCRect rcThis = boundingBox();
	CCRect rcMario = _mario->boundingBox();
	if (rcThis.intersectsRect(rcMario))
		_mario->die(false);
}

void ItemFlower::createAnimationAndFrame() {
	CCAnimation* animation = Common::createAnimation("flower0.png", 0, 1, 16, .5f);
	CCAnimationCache::sharedAnimationCache()->addAnimation(animation, "Flower");
}

bool ItemFlower::canDown(float dt) {
	CCRect rect = boundingBox();
	CCPoint pt[3];
	pt[0] = ccp(rect.getMinX(), rect.getMinY() - dt*_speedDown);
	pt[1] = ccp(rect.getMidX(), rect.getMinY() - dt*_speedDown);
	pt[2] = ccp(rect.getMaxX(), rect.getMinY() - dt*_speedDown);
	// 地板层 石头块 管道
	const char* layerName[] = { "land" };
	for (int i = 0; i < 3; ++i) {
		if (pt[i].y > getMap()->getContentSize().height)
			break;
		if (pt[i].y < 0)
			break;
		CCPoint p = Common::point2tile(getMap(), pt[i]);
		for (int j = 0; j < sizeof(layerName) / sizeof(*layerName); ++j) {
			CCTMXLayer* layer = getMap()->layerNamed(layerName[j]);
			int gid = layer->tileGIDAt(p);
			if (gid != 0) {
				return false;
			}
		}
	}
	return true;
}