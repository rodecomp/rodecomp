#ifndef RAGNAROK_SOURCE_UI_WINDOWCOORDINATES_H
#define RAGNAROK_SOURCE_UI_WINDOWCOORDINATES_H

class UIWindow;

// Original 32-bit coordinate accumulation includes the window itself.
void WindowScreenPosition(const UIWindow* window, int& x, int& y);
// Writes all outputs zero first, then left/top/right/bottom in original order.
// Outputs may alias each other or the window geometry.
void WindowScreenBounds(const UIWindow* window, int& left, int& top, int& right, int& bottom);

// Tests whether two [left, top, right, bottom] bounds rectangles overlap.
bool WindowBoundsOverlap(const int first[4], const int second[4]);

#endif
