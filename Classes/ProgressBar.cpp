#include "ProgressBar.h"

ProgressBar* ProgressBar::create(const char* bgFile, const char* foreFile) {
	ProgressBar *pRet = new ProgressBar;
	if (pRet && pRet->init(bgFile, foreFile))
	{
		pRet->autorelease();
		return pRet;
	}
	else
	{
		pRet = NULL;
		return pRet;
	}
}

bool ProgressBar::init(const char* bgFile, const char* foreFile) {
	CCLayer::init();
	_bg = CCSprite::create(bgFile);
	addChild(_bg);
	// 创建前景精灵
	CCSprite* fore = CCSprite::create(foreFile);
	_bar = CCProgressTimer::create(fore);
	// 设置条形的，从左到右运动的进度条
	_bar->setType(kCCProgressTimerTypeBar);
	_bar->setMidpoint(ccp(0, 0));
	_bar->setBarChangeRate(ccp(1, 0));
	_bar->setPercentage(0);
	addChild(_bar);
	return true;
}
