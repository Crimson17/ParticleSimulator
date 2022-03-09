#include "KrimzLib/window/mouse.h"


// Constructor
kl::mouse::mouse() {

}

// Binds the mouse to the window
void kl::mouse::bind(HWND hwnd) {
	this->hwnd = hwnd;
}

// Hides the mouse cursor
void kl::mouse::hide() {
	while (showCounter > -1) {
		showCounter = ShowCursor(FALSE);
	}
}

// Shows the mouse cursor
void kl::mouse::show() {
	while (showCounter < 0) {
		showCounter = ShowCursor(TRUE);
	}
}

// Moves the mouse pointer
void kl::mouse::move(const kl::int2& pos) {
	POINT clientPos = { pos.x, pos.y };
	ClientToScreen(hwnd, &clientPos);
	SetCursorPos(clientPos.x, clientPos.y);
}

// Return a normalized screen position
kl::float2 kl::mouse::normPos(const kl::int2& frameSize) const {
	kl::float2 pos(
		float(position.x) / frameSize.x,
		float(frameSize.y - position.y) / frameSize.y
	);
	pos *= 2;
	pos -= 1;
	return pos;
}
