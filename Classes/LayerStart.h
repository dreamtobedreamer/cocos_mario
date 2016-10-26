#ifndef __LayerStart_H__
#define __LayerStart_H__
#include "Common.h"

class LayerStart : public CCLayer
{
public:
	CREATE_FUNC(LayerStart);
	bool init();

	void addBg();
	void addMenu();

	void start(CCObject *);
	void set(CCObject *);
	void about(CCObject *);
	void quit(CCObject*);
};

#endif

