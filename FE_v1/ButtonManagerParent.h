#pragma once

#include "Button.h"

class ButtonManagerParent {
public:
	ButtonManagerParent(Button* buttonWait, bool boolWait);

	virtual void init() {}
	virtual void draw() {}

	virtual bool click(float motionX, float motionY) { return false; }
	virtual void unselectButtonPaint() {}

	Button* buttonWait;
	bool boolWait;
};