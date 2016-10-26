#include "Item.h"
#include "Mario.h"
#include "ItemMushRoom.h"
#include "ItemTortoise.h"
#include "ItemTortoiseRound.h"
#include "ItemFlower.h"
#include "ItemLadderLR.h"
#include "ItemLadderUD.h"
#include "ItemTortoiseFly.h"
#include "ItemMushroomAddLife.h"
#include "ItemMushroomReward.h"
#include "ItemFlyFish.h"
#include "ItemFlagPoint.h"
#include "ItemFinalPoint.h"
Item* Item::create(CCDictionary* dict) {
	const CCString* type = dict->valueForKey("type");
	if (type->m_sString == "mushroom")
		return ItemMushRoom::create(dict);
	else if (type->m_sString == "tortoise")
		return ItemTortoise::create(dict);
	else if (type->m_sString == "tortoise_round")
		return ItemTortoiseRound::create(dict);
	else if (type->m_sString == "flower")
		return ItemFlower::create(dict);
	else if (type->m_sString == "ladderLR")
		return ItemLadderLR::create(dict);
	else if (type->m_sString == "ladderUD")
		return ItemLadderUD::create(dict);
	else if (type->m_sString == "tortoise_fly")
		return ItemTortoiseFly::create(dict);
	else if (type->m_sString == "MushroomAddLife")
		return ItemMushroomAddLife::create(dict);
	else if (type->m_sString == "MushroomReward")
		return ItemMushroomReward::create(dict);
	else if (type->m_sString == "flyfish")
		return ItemFlyFish::create(dict);
	else if (type->m_sString == "flagpoint")
		return ItemFlagPoint::create(dict);
	else if (type->m_sString == "finalpoint")
		return ItemFinalPoint::create(dict);
	return NULL;
}

bool Item::canMoveLeft(float dt) {
	// 控制不能向左走出地图
	CCPoint ptInWrold = getPosition();
	if (ptInWrold.x - _speedH*dt < 0)
		return false;

	CCRect rect = boundingBox();
	CCPoint pt[3];
	pt[0] = ccp(rect.getMinX() - dt*_speedH, rect.getMaxY());
	pt[1] = ccp(rect.getMinX() - dt*_speedH, rect.getMidY());
	pt[2] = ccp(rect.getMinX() - dt*_speedH, rect.getMinY());
	// 地板层 石头块 管道
	const char* layerName[] = { "land", "block", "pipe" };
	for (int i = 0; i < 3; ++i) {
		if (pt[i].y > getMap()->getContentSize().height) // 出地图不判断
			break;
		if (pt[i].y < 0) // 出地图不判断
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

bool Item::canMoveRight(float dt) {
	CCRect rect = boundingBox();
	CCPoint pt[3];
	pt[0] = ccp(rect.getMaxX() + dt*_speedH, rect.getMaxY());
	pt[1] = ccp(rect.getMaxX() + dt*_speedH, rect.getMidY());
	pt[2] = ccp(rect.getMaxX() + dt*_speedH, rect.getMinY());
	// 地板层 石头块 管道
	const char* layerName[] = { "land", "block", "pipe" };
	for (int i = 0; i < 3; ++i) {
		if (pt[i].y > getMap()->getContentSize().height) // 出地图不判断
			break;
		if (pt[i].y < 0) // 出地图不判断
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

bool Item::canMoveDown(float dt) {
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
			if (gid != 0) {//碰到物体
				// 微调
				CCPoint ptLB = Common::tile2pointLB(getMap(), p + ccp(0, -1));
				setPositionY(ptLB.y);
				return false;
			}
		}
	}
	return true;
}

bool Item::isInWindow() {
	CCPoint ptInWorld = getMap()->convertToWorldSpace(getPosition());
	if (ptInWorld.x < winSize.width)
		return true;
	return false;
}

void Item::setPos(CCDictionary* dict) {
	const CCString* x = dict->valueForKey("x");
	const CCString* y = dict->valueForKey("y");
	setPosition(ccp(x->intValue(), y->intValue() - 16));
	setAnchorPoint(ccp(0, 0));
}

bool Item::isFarAwayMario() {
	int thisX = getPositionX();
	int marioX = _mario->getPositionX();
	if (marioX - thisX > winSize.width)
		return true;
	return false;
}