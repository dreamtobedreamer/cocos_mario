#include "Mario.h"
#include "LayerChoose.h"

bool Mario::init() {
	CCSprite::init();
	// 初始化
	_status._dirFace = Common::RIGHT;
	_status._dirRun = Common::NONE;
	_status._isBig = false;
	_status._life = 3;
	_status._isFly = false;
	_status._isGod = false;
	_backup = _status;

	_isDead = false;
	_isOnLadder = false;
	_dir = Common::NONE;
	_speedH = 100;
	_speedAccH = 0;
	_speedUp = 0;
	_speedDown = 10;
	_speedAccV = 10;
	_isAutoRun = false;
	_isAutoRunFinish = false;
	_isMissionComplete = false;

	setZOrder(100);
	createAnimate();
	updateStatus();
	schedule(schedule_selector(Mario::move));
	schedule(schedule_selector(Mario::eatCoin));
	return true;
}

void Mario::eatCoin(float dt) {
	if (boundingBox().getMaxY() > getMap()->getContentSize().height)
		return;
	CCTMXLayer* layer = getMap()->layerNamed("coin");
	CCPoint pt[4];
	pt[0] = ccp(boundingBox().getMaxX(), boundingBox().getMaxY());
	pt[1] = ccp(boundingBox().getMinX(), boundingBox().getMaxY());
	pt[2] = ccp(boundingBox().getMaxX(), boundingBox().getMinY());
	pt[3] = ccp(boundingBox().getMinX(), boundingBox().getMinY());
	for (int i = 0; i < 4; ++i) {
		CCPoint p = Common::point2tile(getMap(), pt[i]);
		if (layer->tileGIDAt(p) != 0) {
			layer->setTileGID(0, p);
			break;
		}
	}
	
}

void Mario::autoRun() {
	_isAutoRun = true;
	schedule(schedule_selector(Mario::autoMove));
}



void Mario::jump(float dt) {
	if (_isAutoRun)
		return;

	if (!_status._isFly)
	{
		SimpleAudioEngine::sharedEngine()->playEffect("Jump.ogg");
		_speedUp = 300;
		_status._isFly = true;
	}
}

void Mario::move(float dt) {
	if (!_isAutoRun) {
#ifdef WIN32
		short key;

		key = GetKeyState('F');
		if (key < 0) _dir = Common::RIGHT;

		key = GetKeyState('D');
		if (key < 0) _dir = Common::LEFT;

		key = GetKeyState('J');
		if (key < 0) jump(dt);
#endif
		if (_dir == Common::RIGHT)
		{	
			moveRight(dt);
			_dir = Common::NONE;
		}
		else if (_dir == Common::LEFT)
		{
			moveLeft(dt);
			_dir = Common::NONE;
		}
		else
		{
			_status._dirRun = Common::NONE;
		}
		moveUp(dt);
		moveDown(dt);
	}
	if (memcmp(&_status,&_backup,sizeof(struct status)) != 0)
	{
		updateStatus();
		_backup = _status;
	}
	//if (_isDead)
	//	scheduleOnce(schedule_selector(Mario::dead), 2.0f);
}

void Mario::moveLeft(float dt) {
	if (_status._dirRun != Common::LEFT) {
		_status._dirFace = Common::LEFT;
		_status._dirRun = Common::LEFT;
	}
	
	if (!canMoveLeft(dt))
		return;
	Common::moveNode(this, ccp(-dt*_speedH, 0));
}

void Mario::moveRight(float dt) {
	if (_status._dirRun != Common::RIGHT) {
		_status._dirFace = Common::RIGHT;
		_status._dirRun = Common::RIGHT;
	}
	if (!canMoveRight(dt))
		return;
	Common::moveNode(this, ccp(dt*_speedH, 0));

	// 卷动地图
	CCPoint ptInWorld = getMap()->convertToWorldSpace(getPosition());
	if (ptInWorld.x > winSize.width / 2)
		Common::moveNode(getMap(), ccp(-dt*_speedH, 0));
}

void Mario::moveUp(float dt) {
	if (_speedUp <= 0)
		return;
	if (!canMoveUp(dt)) {
		_speedDown = _speedUp;
		_speedUp = 0;
		return;
	}
		
	Common::moveNode(this, ccp(0, _speedUp*dt));
	_speedUp -= _speedAccV;
}

void  Mario::moveDown(float dt) {
	if (_speedUp <= 0)
	{
		if (canMoveDown(dt)) // 可以往下降
		{	
			if (_status._isFly == false) // 若可以往下降，就证明是在空中，将_isFly赋值true
			{
				_status._isFly = true;
			}
			Common::moveNode(this, ccp(0, -_speedDown*dt));
			_speedDown += _speedAccV;
		}
		else {
			if (_status._isFly)
			{
				_status._isFly = false;
				_speedDown = _speedAccV;
			}
		}		
	}
}

bool Mario::canMoveLeft(float dt) {
	// 控制不能向左走出地图
	CCPoint ptInWrold = getMap()->convertToWorldSpace(getPosition());
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
		// 出地图则不判断(地板层 石头块 管道)
		if (pt[i].y > getMap()->getContentSize().height)
			break;
		if (pt[i].y < 0)
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

bool Mario::canMoveRight(float dt) {
	CCRect rect = boundingBox();
	CCPoint pt[3];
	pt[0] = ccp(rect.getMaxX() + dt*_speedH, rect.getMaxY());
	pt[1] = ccp(rect.getMaxX() + dt*_speedH, rect.getMidY());
	pt[2] = ccp(rect.getMaxX() + dt*_speedH, rect.getMinY());
	// 地板层 石头块 管道
	const char* layerName[] = {"land","block","pipe"};
	for (int i = 0; i < 3; ++i) {
		// 出地图则不判断(地板层 石头块 管道)
		if (pt[i].y > getMap()->getContentSize().height)
			break;
		if (pt[i].y < 0)
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

bool Mario::canMoveUp(float dt) {
	CCRect rect = boundingBox();
	CCPoint pt[3];
	pt[0] = ccp(rect.getMinX(), rect.getMaxY() + dt*_speedUp);
	pt[1] = ccp(rect.getMidX(), rect.getMaxY() + dt*_speedUp);
	pt[2] = ccp(rect.getMaxX(), rect.getMaxY() + dt*_speedUp);
	// 地板层 石头块 管道
	const char* layerName[] = { "land", "block", "pipe" };
	for (int i = 0; i < 3; ++i) {
		// 出地图则不判断(地板层 石头块 管道)
		if (pt[i].y > getMap()->getContentSize().height)
			break;
		CCPoint p = Common::point2tile(getMap(), pt[i]);
		for (int j = 0; j < sizeof(layerName) / sizeof(*layerName); ++j) {
			CCTMXLayer* layer = getMap()->layerNamed(layerName[j]);
			int gid = layer->tileGIDAt(p);
			if (gid != 0) {//碰到物体
				// 微调
				CCPoint ptLB = Common::tile2pointLB(getMap(), p + ccp(0,1));
				setPositionY(ptLB.y);
				// 顶砖头
				hit(layer,layerName[i],p,gid);

				CCLog("gid = %d", gid);
				return false;
			}
		}
	}
	return true;
}

bool Mario::canMoveDown(float dt) {
	if (_isOnLadder)
		return false;

	CCRect rect = boundingBox();
	CCPoint pt[3];
	pt[0] = ccp(rect.getMinX(), rect.getMinY() - dt*_speedDown);
	pt[1] = ccp(rect.getMidX(), rect.getMinY() - dt*_speedDown);
	pt[2] = ccp(rect.getMaxX(), rect.getMinY() - dt*_speedDown);
	// 地板层 石头块 管道
	const char* layerName[] = { "land", "block", "pipe" };
	for (int i = 0; i < 3; ++i) {
		// 出地图则不判断(地板层 石头块 管道)
		if (pt[i].y > getMap()->getContentSize().height)
			break;
		if (pt[i].y < 0)
		{// 游戏结束
			_status._life = 0;
			_isDead = true;
			gameOver();
			break;
		}
		CCPoint p = Common::point2tile(getMap(), pt[i]);
		for (int j = 0; j < sizeof(layerName) / sizeof(*layerName); ++j) {
			CCTMXLayer* layer = getMap()->layerNamed(layerName[j]);
			int gid = layer->tileGIDAt(p);
			if (gid != 0) {//碰到物体
				// 微调
				CCPoint ptLB = Common::tile2pointLB(getMap(), p+ccp(0,-1));
				setPositionY(ptLB.y);
				return false;
			}
		}
	}
	return true;
}

void Mario::hit(CCTMXLayer* layer, const char* name, CCPoint ptTile, int gid) {
	CCSprite* s;
	switch (gid) {
	case 1: // 普通砖头
		s = layer->tileAt(ptTile);
		//s->runAction(CCJumpBy::create(.1f, ccp(0, 0), 20, 1));
		layer->setTileGID(0, ptTile);
		break;
	case 601: // 问号砖头
		s = layer->tileAt(ptTile);
		s->runAction(CCJumpBy::create(.1f, ccp(0, 0), 20, 1));
		layer->setTileGID(31, ptTile);
		break;
	case 846: // 隐形砖头
		layer->setTileGID(31, ptTile);
		s = layer->tileAt(ptTile);
		s->runAction(CCJumpBy::create(.1f, ccp(0, 0), 20, 1));
		break;
	}

}

void Mario::autoMove(float dt) {
	if (canMoveDown(dt)) {
		Common::moveNode(this, ccp(0, -50 * dt));
	}
	else {	
		if (canMoveRight(dt)) {
			_status._isFly = false;
			_status._dirRun = Common::RIGHT;
			_status._dirFace = Common::RIGHT;
			Common::moveNode(this, ccp(50*dt, 0));
		}
	}

	if (_isAutoRunFinish) {
		unschedule(schedule_selector(Mario::autoMove));
		this->setVisible(false);
		_isMissionComplete = true; // 该关卡已通过
	}
}

void Mario::updateStatus() {
	stopAllActions();
	if (_status._life <= 0) // 播放死亡动画
	{
		CCAnimate* animate;
		if (!_status._isBig)
		{
			animate = CCAnimate::create(
				CCAnimationCache::sharedAnimationCache()->animationByName("SmallDie"));
		}
		else {
			animate = CCAnimate::create(
				CCAnimationCache::sharedAnimationCache()->animationByName("BigDie"));
		}
		CCJumpBy* act1 = CCJumpBy::create(.3f, ccp(0, 0), 20, 1);
		CCMoveBy* act2 = CCMoveBy::create(1.0f, ccp(0, -winSize.height));
		runAction(CCSequence::create(animate, act1, act2, NULL));
		return;
	}
	if (_status._isBig) {// 大马里奥
		// 在空中
		if (_status._isFly) {
			if (_status._dirFace == Common::LEFT)
				setDisplayFrame(CCSpriteFrameCache::sharedSpriteFrameCache()->
				spriteFrameByName("UpLeft"));
			else
				setDisplayFrame(CCSpriteFrameCache::sharedSpriteFrameCache()->
				spriteFrameByName("UpRight"));
			return;
		}
		// 奔跑状态
		if (_status._dirRun == Common::RIGHT)
		{
			CCAnimate* animate = CCAnimate::create(
				CCAnimationCache::sharedAnimationCache()->animationByName("WalkRight"));
			this->runAction(CCRepeatForever::create(animate));
		}
		else if (_status._dirRun == Common::LEFT)
		{
			CCAnimate* animate = CCAnimate::create(
				CCAnimationCache::sharedAnimationCache()->animationByName("WalkLeft"));
			this->runAction(CCRepeatForever::create(animate));
		}
		else // 非奔跑状态
		{
			if (_status._dirFace == Common::LEFT) {
				setDisplayFrame(CCSpriteFrameCache::sharedSpriteFrameCache()->
					spriteFrameByName("FaceLeft"));
			}
			else {
				setDisplayFrame(CCSpriteFrameCache::sharedSpriteFrameCache()->
					spriteFrameByName("FaceRight"));
			}
		}
	}
	else { // 小马里奥
		// 在空中
		if (_status._isFly) {
			if (_status._dirFace == Common::LEFT)
				setDisplayFrame(CCSpriteFrameCache::sharedSpriteFrameCache()->
					spriteFrameByName("SmallUpLeft"));
			else
				setDisplayFrame(CCSpriteFrameCache::sharedSpriteFrameCache()->
				spriteFrameByName("SmallUpRight"));
			return;
		}
		// 奔跑状态
		if (_status._dirRun == Common::RIGHT)
		{
			CCAnimate* animate = CCAnimate::create(
				CCAnimationCache::sharedAnimationCache()->animationByName("SmallWalkRight"));
			this->runAction(CCRepeatForever::create(animate));
		}
		else if (_status._dirRun == Common::LEFT)
		{
			CCAnimate* animate = CCAnimate::create(
				CCAnimationCache::sharedAnimationCache()->animationByName("SmallWalkLeft"));
			this->runAction(CCRepeatForever::create(animate));
		}
		else // 非奔跑状态
		{
			if (_status._dirFace == Common::LEFT) {
				setDisplayFrame(CCSpriteFrameCache::sharedSpriteFrameCache()->
					spriteFrameByName("SmallFaceLeft"));
			}
			else {
				setDisplayFrame(CCSpriteFrameCache::sharedSpriteFrameCache()->
					spriteFrameByName("SmallFaceRight"));
			}
		}
	}
}

// 加载动画和帧
void Mario::createAnimate() {
	// 马里奥动画
	// SmallWalkLeft    小马里奥 左
	// SmallWalkRight   小马里奥 右
	// WalkLeft         大马里奥 左
	// WalkRight        大马里奥 右
	// WalkLeft_fire    大马里奥 左 有子弹
	// WalkRight_fire   大马里奥 右 有子弹
	CCAnimationCache::sharedAnimationCache()->addAnimation(
		Common::createAnimation("smallWalkLeft.png",0,9,14,0.05f),"SmallWalkLeft");
	CCAnimationCache::sharedAnimationCache()->addAnimation(
		Common::createAnimation("smallWalkRight.png", 0, 9, 14, 0.05f), "SmallWalkRight");
	CCAnimationCache::sharedAnimationCache()->addAnimation(
		Common::createAnimation("walkLeft.png", 0, 9, 18, 0.05f), "WalkLeft");
	CCAnimationCache::sharedAnimationCache()->addAnimation(
		Common::createAnimation("walkRight.png", 0, 9, 18, 0.05f), "WalkRight");
	CCAnimationCache::sharedAnimationCache()->addAnimation(
		Common::createAnimation("WalkLeft_fire.png", 0, 9, 18, 0.05f), "WalkLeft_fire");
	CCAnimationCache::sharedAnimationCache()->addAnimation(
		Common::createAnimation("WalkRight_fire.png", 0, 9, 18, 0.05f), "WalkRight_fire");
	// SmallFaceLeft   马里奥静止时候的帧
	// SmallFaceRight
	// FaceLeft
	// FaceRight
	// FaceLeft_fire
	// FaceRight_fire
	CCSpriteFrameCache::sharedSpriteFrameCache()->addSpriteFrame(
		Common::getFrame("smallWalkLeft.png", 0, 14), "SmallFaceLeft");
	CCSpriteFrameCache::sharedSpriteFrameCache()->addSpriteFrame(
		Common::getFrame("smallWalkRight.png", 0, 14), "SmallFaceRight");
	CCSpriteFrameCache::sharedSpriteFrameCache()->addSpriteFrame(
		Common::getFrame("walkLeft.png", 0, 18), "FaceLeft");
	CCSpriteFrameCache::sharedSpriteFrameCache()->addSpriteFrame(
		Common::getFrame("walkRight.png", 0, 18), "FaceRight");
	CCSpriteFrameCache::sharedSpriteFrameCache()->addSpriteFrame(
		Common::getFrame("WalkLeft_fire.png", 0, 18), "FaceLeft_fire");
	CCSpriteFrameCache::sharedSpriteFrameCache()->addSpriteFrame(
		Common::getFrame("WalkRight_fire.png", 0, 18), "FaceRight_fire");
	// SmallUpLeft   马里奥跳起来的帧
	// SmallUpRight
	// UpLeft
	// UpRight
	// UpLeft_fire
	// UpRight_fire
	CCSpriteFrameCache::sharedSpriteFrameCache()->addSpriteFrame(
		Common::getFrame("smallWalkLeft.png", 10, 14), "SmallUpLeft");
	CCSpriteFrameCache::sharedSpriteFrameCache()->addSpriteFrame(
		Common::getFrame("smallWalkRight.png", 10, 14), "SmallUpRight");
	CCSpriteFrameCache::sharedSpriteFrameCache()->addSpriteFrame(
		Common::getFrame("walkLeft.png", 10, 18), "UpLeft");
	CCSpriteFrameCache::sharedSpriteFrameCache()->addSpriteFrame(
		Common::getFrame("walkRight.png", 10, 18), "UpRight");
	CCSpriteFrameCache::sharedSpriteFrameCache()->addSpriteFrame(
		Common::getFrame("WalkLeft_fire.png", 10, 18), "UpLeft_fire");
	CCSpriteFrameCache::sharedSpriteFrameCache()->addSpriteFrame(
		Common::getFrame("WalkRight_fire.png", 10, 18), "UpRight_fire");
	// SmallDie 死亡动画
	// BigDie
	// FireDie
	CCAnimationCache::sharedAnimationCache()->addAnimation(
		Common::createAnimation("small_die.png", 0, 6, 16, .1f), "SmallDie");
	CCAnimationCache::sharedAnimationCache()->addAnimation(
		Common::createAnimation("normal_die.png", 0, 6, 24, .1f), "BigDie");
	CCAnimationCache::sharedAnimationCache()->addAnimation(
		Common::createAnimation("fire_die.png", 0, 6, 24, .1f), "FireDie");
}

void Mario::die(bool tf) {
	if (!_status._isGod)
	{
		setGod(true);
		scheduleOnce(schedule_selector(Mario::godTime), 1.0f);
		if (tf) {
			_status._life = 0;
			_isDead = true;
			gameOver();
		}
		else {
			if (_status._isBig)
				_status._isBig = false;
			else {
				if (_status._life > 1)
					--_status._life;
				else
				{
					_status._life = 0;
					_isDead = true;
					gameOver();
				}				
			}
		}
	}
}

void Mario::gameOver() {
	// 结束调度器
	unscheduleAllSelectors();
	// 开始切换场景定时
	scheduleOnce(schedule_selector(Mario::dead), 2.0f);
	// 播放死亡动画
	updateStatus();
}

void Mario::dead(float) {
	CCDirector::sharedDirector()->
		replaceScene(Common::scene(LayerChoose::create()));
}