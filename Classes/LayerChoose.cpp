#include "LayerChoose.h"
#include "LayerStart.h"
#include "LayerGame.h"

bool LayerChoose::init() {
	CCLayer::init();
	// 添加背景
	CCSprite *bg = CCSprite::create("bg.png");
	bg->setAnchorPoint(ccp(0, 0));
	bg->setPosition(ccp(0, 0));
	addChild(bg);
	// 添加滑动菜单
	addScrollMenu();
	// 添加返回菜单
	addBackButton();
	setTouchEnabled(true);
	setTouchMode(kCCTouchesOneByOne);
	return true;
}

// 添加滑动菜单
void LayerChoose::addScrollMenu() {
	CCNode* node = CCNode::create();
	for (int i = 0; i < 8; ++i)
	{
		char* buf = Common::format(i + 1, "select", ".jpg");
		CCSprite* sprite = CCSprite::create(buf);
		node->addChild(sprite);
		sprite->setTag(1000 + i);
		sprite->setPosition(ccp(winSize.width/2 + winSize.width*i, winSize.height / 2));
	}
	CCScrollView *menu = CCScrollView::create(winSize, node);
	_view = menu;
	addChild(menu);
	menu->setDirection(kCCScrollViewDirectionHorizontal);//设置方向
	menu->setContentSize(CCSizeMake(winSize.width*8, winSize.height));//设置大小
	//menu->setBounceable(false);// 去掉弹性
}

void LayerChoose::addBackButton() {
	CCMenu *menu = CCMenu::create();
	addChild(menu);

	CCMenuItemImage *back = CCMenuItemImage::create(
		"backA.png", "backB.png",
		this,
		menu_selector(LayerChoose::back));
	menu->addChild(back);
	int x = winSize.width / 2 - 20;
	int y = winSize.height / 2 - 20;
	Common::moveNode(back, ccp(x, -y));
}

void LayerChoose::back(CCObject* obj) {
	CCDirector::sharedDirector()->replaceScene(
		Common::scene((CCLayer*)LayerStart::create()));
}



bool LayerChoose::ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent) {
	return true;
}

void LayerChoose::ccTouchEnded(CCTouch *pTouch, CCEvent *pEvent) {
	if (isClick(pTouch))
	{
		CCNode* node = _view->getContainer();
		CCPoint ptInNode = node->convertTouchToNodeSpace(pTouch);
		for (int i = 0; i < 8; ++i) {
			CCSprite* s = (CCSprite*)node->getChildByTag(1000 + i);
			if (s->boundingBox().containsPoint(ptInNode))
			{
				CCDirector::sharedDirector()->replaceScene(
					Common::scene(LayerGame::create(i + 1)));
				break;
			}
		}
	}
}

bool LayerChoose::isClick(CCTouch *t) {
	CCPoint s = t->getStartLocation();
	CCPoint c = t->getLocation();
	return s.getDistanceSq(c) < 25;
}