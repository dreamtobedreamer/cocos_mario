#ifndef __Common_H__
#define __Common_H__
#include "cocos2d.h"
#include "cocos-ext.h"
#include "SimpleAudioEngine.h"

USING_NS_CC;
USING_NS_CC_EXT;
using namespace CocosDenshion;

#define winSize CCDirector::sharedDirector()->getWinSize()

class Common {
public:
	enum DIR{LEFT, UP, RIGHT, DOWN, NONE};
	// 创建场景
	static CCScene* scene(CCLayer *layer) {
		if (layer == NULL)
		{
			CCLog("传入参数为空");
			return NULL;
		}		
		CCScene* scene = CCScene::create();
		scene->addChild(layer);
		return scene;
	}
	// 移动节点
	static void moveNode(CCNode* n, CCPoint p) {
		float x = n->getPositionX();
		float y = n->getPositionY();
		n->setPosition(ccp(x + p.x, y + p.y));
	}
	// 格式化字符串
	static char* format(int num, const char* prefix, const char* subfix) {
		static char buf[1024];
		memset(buf, 0, sizeof(buf));
		sprintf(buf, "%s%d%s", prefix, num, subfix);
		return buf;
	}
	// 创建动画
	static CCAnimation* createAnimation(const char* file,int begin, 
		int end, int width, float delay) {
		CCTexture2D *img = CCTextureCache::sharedTextureCache()->addImage(file);
		CCArray* arr = CCArray::create();
		for (int i = begin; i <= end; ++i) {
			CCSpriteFrame* frame = CCSpriteFrame::createWithTexture(img,
				CCRectMake(width*i,0,width,img->getContentSize().height));
			arr->addObject(frame);
		}
		return CCAnimation::createWithSpriteFrames(arr, delay);
	}

	// 创建动画2 通过多个文件名创建动画
	static CCAnimation* createAnimation(const char* prefix,
		const char* suffix,
		int begin,
		int end,
		float delay) {
		CCArray* arr = CCArray::create();
		for (int i = begin; begin != end; ++i) {
			char* name = format(begin, prefix, suffix);
			CCSpriteFrame* frame = CCSpriteFrameCache::sharedSpriteFrameCache()->
				spriteFrameByName(name);
			arr->addObject(frame);
		}
		return CCAnimation::createWithSpriteFrames(arr, delay);
	}

	// 获得帧
	static CCSpriteFrame* getFrame(const char* filename, int pos, int width) {
		CCTexture2D* texture = CCTextureCache::sharedTextureCache()->addImage(filename);
		return CCSpriteFrame::createWithTexture(texture, 
			CCRectMake(pos*width,0,width,texture->getContentSize().height));
	}
	// 由像素坐标获得抽象坐标, 格子地图的原点是左上角
	static CCPoint point2tile(CCTMXTiledMap* map, CCPoint pt) {
		int dx = map->getTileSize().width;
		int dy = map->getTileSize().height;
		int x = pt.x / dx;
		int y = pt.y / dy;
		y = map->getMapSize().height - y - 1;
		return ccp(x, y);
	}
	// 由抽象坐标获得像素坐标(格子左下角的坐标值LeftButtom)
	static CCPoint tile2pointLB(CCTMXTiledMap* map, CCPoint pt) {
		pt.y = map->getMapSize().height - 1 - pt.y;
		return ccp(pt.x * map->getTileSize().width,
			pt.y * map->getTileSize().height);
	}
};

#endif
