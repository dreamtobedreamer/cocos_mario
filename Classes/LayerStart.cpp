#include "LayerStart.h"
#include "LayerChoose.h"

bool LayerStart::init() {
	CCLayer::init();
	// ��ӱ���
	this->addBg();
	// ��Ӳ˵�
	this->addMenu();

	return true;
}


void LayerStart::addBg() {
	CCSprite *bg = CCSprite::create("background.png");
	addChild(bg);
	bg->setAnchorPoint(ccp(0, 0));
	bg->setPosition(ccp(0, 0));
}

void LayerStart::addMenu() {
	CCMenu *menu = CCMenu::create();
	addChild(menu);
	
	// ��ʼ�˵�
	CCMenuItemImage *start = CCMenuItemImage::create(
		"select_n.png",
		"select_s.png",
		this,
		menu_selector(LayerStart::start));
	menu->addChild(start);
	// ���ò˵�
	CCMenuItemImage *set = CCMenuItemImage::create(
		"set_n.png",
		"set_s.png",
		this,
		menu_selector(LayerStart::set));
	menu->addChild(set);
	// ����
	CCMenuItemImage *about = CCMenuItemImage::create(
		"about_normal.png",
		"about_select.png",
		this,
		menu_selector(LayerStart::about));
	menu->addChild(about);
	// �˳�
	CCMenuItemImage *quit = CCMenuItemImage::create(
		"quitgame_normal.png",
		"quitgame_select.png",
		this,
		menu_selector(LayerStart::quit));
	menu->addChild(quit);

	menu->alignItemsVerticallyWithPadding(5);
	Common::moveNode(menu, ccp(0, -50));
}

void LayerStart::start(CCObject* obj) {
	CCDirector::sharedDirector()->replaceScene(
		Common::scene((CCLayer*)LayerChoose::create()));
}

void LayerStart::set(CCObject* obj) {

}

void LayerStart::about(CCObject* obj) {

}

void LayerStart::quit(CCObject* obj) {
	CCDirector::sharedDirector()->end();
}