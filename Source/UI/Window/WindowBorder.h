#ifndef RAGNAROK_SOURCE_UI_WINDOW_WINDOWBORDER_H
#define RAGNAROK_SOURCE_UI_WINDOW_WINDOWBORDER_H

#include <cstdint>

class UIWindow;

void DrawRoundedBorder(UIWindow& window, int x, int y, int width, int height, std::uint32_t color);

void DrawRectangleBorder(UIWindow& window, int x, int y, int width, int height, std::uint32_t color);

#endif
