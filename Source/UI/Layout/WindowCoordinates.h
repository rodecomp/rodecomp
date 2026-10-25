#ifndef RAGNAROK_SOURCE_UI_LAYOUT_WINDOWCOORDINATES_H
#define RAGNAROK_SOURCE_UI_LAYOUT_WINDOWCOORDINATES_H

class UIWindow;

void WindowScreenPosition(const UIWindow* window, int& x, int& y);

void WindowScreenBounds(const UIWindow* window, int& left, int& top, int& right, int& bottom);

bool WindowBoundsOverlap(const int first[4], const int second[4]);

#endif
