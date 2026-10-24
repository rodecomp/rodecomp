#ifndef RAGNAROK_SOURCE_UI_WINDOWBORDER_H
#define RAGNAROK_SOURCE_UI_WINDOWBORDER_H

#include <cstdint>

class UIWindow;

void DrawRoundedBorder(UIWindow& window, int x, int y, int width, int height, std::uint32_t color);
// Ordered top/left/right/bottom strips. Signed clipping follows wrapping
// endpoint arithmetic; empty/inverted rectangles are still sent to the DC.
void DrawRectangleBorder(UIWindow& window, int x, int y, int width, int height, std::uint32_t color);

#endif
