#ifndef RAGNAROK_SOURCE_UI_UISCROLLWND_H
#define RAGNAROK_SOURCE_UI_UISCROLLWND_H

#include "UI/Window/UIWindow.h"

#include <map>
#include <utility>

class CTexture;
class UIScrollBar;

class UIScrollWnd : public UIWindow {
public:
  ~UIScrollWnd() override;
  void Slot15(int width, int height) override;
  void Slot35(int delta) override;
  RenderPrimitive* DrawPrimitive() override;

  RenderPrimitive* BuildPrimitive(int x, int y, int sourceX, int sourceY, int width, int height, CTexture* texture, std::uint32_t color);
  void Clear();
  void RemoveChild(UIWindow* child, unsigned char destroy);

  /* 0080 */ int viewHeight;
  /* 0084 */ int contentWidth;
  /* 0088 */ int contentHeight;
  /* 008C */ UIScrollBar* horizontalBar;
  /* 0090 */ UIScrollBar* scrollBar;
  /* 0094 */ int scrollBarWidth;
  /* 0098 */ int scrollBarHeight;
  /* 009C */ int unknown_009C;
  /* 00A0 */ int scrollPosition;
  /* 00A4 */ unsigned char unknown_00A4;
  /* 00A5 */ unsigned char wheelScroll;
  /* 00A8 */ std::map<UIWindow*, std::pair<int, int>> childOrigins;
};

#endif
