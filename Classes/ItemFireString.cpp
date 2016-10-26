#include "ItemFireString.h"

bool ItemFireString::init(CCDictionary* dict) {
	Item::init();
	_speedH = 100;
	_speedAcc = 10;
	_speedDown = 0;


	return true;
}

void ItemFireString::move(float dt) {

}

void ItemFireString::collision() {

}

void ItemFireString::createAnimationAndFrame() {
}