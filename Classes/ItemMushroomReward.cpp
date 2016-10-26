#include "ItemMushroomReward.h"
#include "Mario.h"

bool ItemMushroomReward::init(CCDictionary* dict) {
	Item::init();
	_speedH = 50;
	_speedAcc = 10;
	_speedDown = 10;
	_dir = Common::RIGHT;
	_state = NORMAL;
	_isGod = false;

	createAnimationAndFrame();
	initWithSpriteFrameName("MushRoomReward");

	int x = dict->valueForKey("x")->intValue();
	int y = dict->valueForKey("y")->intValue();
	setAnchorPoint(ccp(0.0f, 0.0f));
	setPosition(ccp(x, y - 18));
	setZOrder(0);
	setVisible(false);
	return true;
}

void ItemMushroomReward::move(float dt) {
	if (!isInWindow())
		return;
	if (_state != MOVE)
		return;
	setZOrder(100);
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

	if (canDown(dt))
	{
		Common::moveNode(this, ccp(0, -_speedDown*dt));
		_speedDown += _speedAcc;
	}
}

void ItemMushroomReward::collision() {
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
		if (!_isGod) {
			if (_state == NORMAL) {
				_isGod = true;
				setVisible(true);
				// 上升一段距离
				CCMoveBy* moveBy = CCMoveBy::create(1.0f, ccp(0, 20));
				// 置位
				CCCallFunc* callFunc = CCCallFunc::create(this,
					callfunc_selector(ItemMushroomReward::autoRun));
				runAction(CCSequence::create(moveBy, callFunc, NULL));
			}
			else {
				unscheduleUpdate();
				_mario->_status._isBig = true;
				removeFromParent();
			}
		}
	}
}



void ItemMushroomReward::createAnimationAndFrame() {
	// MushRoomReward
	CCSpriteFrameCache::sharedSpriteFrameCache()->addSpriteFrame(
		Common::getFrame("rewardMushroomSet.png", 1, 16), "MushRoomReward");
}

bool ItemMushroomReward::canDown(float dt) {
	CCRect rect = boundingBox();
	CCPoint pt[3];
	pt[0] = ccp(rect.getMinX(), rect.getMinY() - dt*_speedDown);
	pt[1] = ccp(rect.getMidX(), rect.getMinY() - dt*_speedDown);
	pt[2] = ccp(rect.getMaxX(), rect.getMinY() - dt*_speedDown);
	// 地板层 石头块 管道
	const char* layerName[] = { "land", "block", "pipe" };
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
				CCPoint ptLB = Common::tile2pointLB(getMap(), p + ccp(0, -1));
				setPositionY(ptLB.y);
				return false;
			}
		}
	}
	return true;
}