#include "ItemFlagPoint.h"
#include "Mario.h"
bool ItemFlagPoint::init(CCDictionary* dict) {
	Item::init();
	setPos(dict);
	return true;
}


void ItemFlagPoint::collision() {
	if (!isInWindow())
		return;
	CCTMXLayer* layer = getMap()->layerNamed("flagpole");
	float x = _mario->boundingBox().getMaxX();
	float y = _mario->boundingBox().getMinY();
	CCPoint pt = Common::point2tile(getMap(), ccp(x,y));
	int gid = layer->tileGIDAt(pt);
	if (gid != 0) {
		Common::moveNode(_mario, ccp(8, 0));
		_mario->autoRun();
	}
}