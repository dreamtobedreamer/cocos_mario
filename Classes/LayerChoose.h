#ifndef __LayerChoose_H__
#define __LayerChoose_H__

#include "Common.h"

class LayerChoose : public CCLayer
{
public:
	CREATE_FUNC(LayerChoose);
	bool init();
	void back(CCObject*);

	void addScrollMenu();
	void addBackButton();
	bool isClick(CCTouch *pTouch);

	virtual bool ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent);
	virtual void ccTouchEnded(CCTouch *pTouch, CCEvent *pEvent);

	CCScrollView* _view;
};

#endif

