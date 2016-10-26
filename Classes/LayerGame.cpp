#include "LayerGame.h"
#include "ProgressBar.h"
#include "Mario.h"
#include "Item.h"
#include "LayerStart.h"
LayerGame* LayerGame::create(int idx) {
	LayerGame* layer = new LayerGame;
	if (layer && layer->init(idx))
	{
		layer->autorelease();
		return layer;
	}
	else
	{
		delete layer;
		layer = NULL;
		return layer;
	}
}

bool LayerGame::init(int idx) {
	CCLayer::init();
	CCSprite *bg = CCSprite::create("bg.png");
	addChild(bg);
	bg->setPosition(ccp(winSize.width / 2, winSize.height / 2));
	// 初始化
	_idx = idx;
	_loadFileCount = 0;
	_totalFileCount = 0;
	_bar = ProgressBar::create("sliderTrack.png", "sliderProgress.png");
	addChild(_bar);
	_bar->setPosition(winSize.width / 2, 50);
	_mario = NULL;

	loadResource();
	
	return true;
}

void LayerGame::checkGame(float dt) {
	if (_mario->missionComplete()) {// 如果通关就进入下一关
		if (_idx + 1 > 8)
			CCDirector::sharedDirector()->replaceScene(Common::scene(
				LayerStart::create()));
		else
			CCDirector::sharedDirector()->replaceScene(Common::scene(
				LayerGame::create(_idx + 1)));
	}
}

void LayerGame::startGame() {
	// 加载音乐
	SimpleAudioEngine::sharedEngine()->playBackgroundMusic("OnLand.wma");
	// 加载地图
	_map = CCTMXTiledMap::create(Common::format(_idx, "MarioMap", ".tmx"));
	addChild(_map);
	Common::moveNode(_map, ccp(0, winSize.height - _map->getContentSize().height));
	// 先创建Mario 先有Mario后有Item
	_mario = Mario::create();
	_map->addChild(_mario);
	schedule(schedule_selector(LayerGame::checkGame));
	// 创建地图元素
	CCTMXObjectGroup* objGroup = _map->objectGroupNamed("objects");
	CCArray* objs = objGroup->getObjects();
	CCObject* obj;
	CCARRAY_FOREACH(objs, obj) {
		CCDictionary* dict = (CCDictionary*)obj;
		const CCString* x = dict->valueForKey("x");
		const CCString* y = dict->valueForKey("y");
		const CCString* type = dict->valueForKey("type");
		if (type->m_sString != "BirthPoint")
		{
			// 创建地图元素
			Item* item = Item::create(dict);
			if (item != NULL)
			{
				_map->addChild(item);
				item->_mario = _mario;
			}
				
		}
		else
		{
			_mario->setAnchorPoint(ccp(0, 0));
			_mario->setPosition(ccp(x->floatValue(), 
				y->floatValue() - _map->getTileSize().height));
		}
	}
}

void LayerGame::loadResource() {
	// 加载音乐
	const char* micEffect[] = {
		"Jump.ogg",
		"CaiSiGuaiWu.ogg",
		"SuoXiao.ogg"
	};
	const char* mic[] = {
		"OnLand.wma"
	};
	for (int i = 0; i < sizeof(micEffect) / (sizeof(*micEffect)); ++i) {
		SimpleAudioEngine::sharedEngine()->preloadEffect(micEffect[i]);
	}
	for (int i = 0; i < sizeof(mic) / (sizeof(*mic)); ++i) {
		SimpleAudioEngine::sharedEngine()->preloadBackgroundMusic(mic[i]);
	}
	// 加载图片
	const char* pic[] = {
		"about_normal.png",
		"about_select.png",
		"AB_normal.png",
		"AB_select.png",
		"allow_walkLeft.png",
		"allow_walkRight.png",
		"arrow.png",
		"arrowActionL.png",
		"arrowActionR.png",
		"arrowBroken.png",
		"arrowBullet.png",
		"arrow_die.png",
		"axe.png",
		"backA.png",
		"backB.png",
		"background.png",
		"backKeyImage.png",
		"backKeyLeft.png",
		"backKeyRight.png",
		"backToMenu.png",
		"battery.png",
		"batteryBoom1.png",
		"batteryBoom2.png",
		"batteryBoom3.png",
		"batteryBullet.png",
		"bg.png",
		"blinkcoin.png",
		"boss.png",
		"bossBullet.png",
		"brokencoin.png",
		"bulletBorder.png",
		"CloseNormal.png",
		"CloseSelected.png",
		"cloud.png",
		"coinani.png",
		"controlUI.png",
		"darkCloud.png",
		"fireActionL.png",
		"fireActionR.png",
		"fireBall.png",
		"fireLeft.png",
		"fireRight.png",
		"fire_die.png",
		"flag.png",
		"flower0.png",
		"flyFishLeft.png",
		"flyFishRight.png",
		"fps_images.png",
		"gameover.png",
		"ladder.png",
		"left.png",
		"leftright.png",
		"lighting.png",
		"Mushroom0.png",
		"music_off.png",
		"music_on.png",
		"M_n.png",
		"M_s.png",
		"newgameA.png",
		"newgameB.png",
		"nextlevel_normal.png",
		"nextlevel_select.png",
		"normal_die.png",
		"PassFailure.png",
		"PassSuccess.png",
		"princess.png",
		"quitgame_normal.png",
		"quitgame_select.png",
		"restart_n.png",
		"restart_s.png",
		"resume_n.png",
		"resume_s.png",
		"retry_normal.png",
		"retry_select.png",
		"rewardMushroomSet.png",
		"right.png",
		"select_n.png",
		"select_s.png",
		"Setting_n.png",
		"setting_s.png",
		"Set_Menu.png",
		"Set_Music.png",
		"set_n.png",
		"set_s.png",
		"shanshuodecoin.png",
		"singleblock.png",
		"sliderProgress.png",
		"sliderTrack.png",
		"smallWalkLeft.png",
		"smallWalkRight.png",
		"small_die.png",
		"sound_effect_off.png",
		"sound_effect_on.png",
		"startgame_normal.png",
		"startgame_select.png",
		"superMarioMap.png",
		"switchBg.png",
		"Tools.png",
		"tortoise0.png",
		"walkLeft.png",
		"WalkLeft_fire.png",
		"walkRight.png",
		"WalkRight_fire.png"
	};
	
	_totalFileCount = sizeof(pic) / (sizeof(*pic));
	for (int i = 0; i < _totalFileCount; ++i) {
		//CCTextureCache::sharedTextureCache()->addImage(pic[i]);
		CCTextureCache::sharedTextureCache()->addImageAsync(pic[i],
			this,
			callfuncO_selector(LayerGame::loadCallBack));
	}
}

void LayerGame::loadCallBack(CCObject*) {
	_loadFileCount++;
	_bar->setPercentage(_loadFileCount*100.0f / _totalFileCount);
	if (_loadFileCount == _totalFileCount) {
		_bar->removeFromParent();
		startGame();
	}
}