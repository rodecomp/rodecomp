#ifndef RAGNAROK_SOURCE_UI_WINDOW_WINDOWBORDER_H
#define RAGNAROK_SOURCE_UI_WINDOW_WINDOWBORDER_H

#include <cstdint>

class UIWindow;

void DrawRoundedBorder(UIWindow& window, int x, int y, int width, int height, std::uint32_t color);

void DrawRectangleBorder(UIWindow& window, int x, int y, int width, int height, std::uint32_t color);

void DrawFilledRoundedBorder(UIWindow& window, int x, int y, int width, int height, std::uint32_t color);

void FillTriangle(UIWindow& window, int x0, int y0, int x1, int y1, int x2, int y2, std::uint32_t color);

#endif
