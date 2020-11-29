#include "ButtonManagerParent.h"

ButtonManagerParent::ButtonManagerParent(Button* buttonWait, bool boolWait) {
	this->buttonWait = buttonWait;
	this->boolWait = boolWait;
}