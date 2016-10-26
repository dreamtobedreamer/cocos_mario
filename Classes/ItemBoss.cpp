#include "ItemBoss.h"

bool ItemBoss::init(CCDictionary* dict) {
	Item::init();
	_speedH = 100;
	_speedAcc = 10;
	_speedDown = 0;


	return true;
}

void ItemBoss::move(float dt) {

}

void ItemBoss::collision() {

}

void ItemBoss::createAnimationAndFrame() {
}