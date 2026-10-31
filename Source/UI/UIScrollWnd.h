#ifndef RAGNAROK_SOURCE_UI_UISCROLLWND_H
#define RAGNAROK_SOURCE_UI_UISCROLLWND_H

#include "UI/Window/UIWindow.h"

#include <map>
#include <utility>

class CTexture;
class UIScrollBar;

class UIScrollWnd : public UIWindow {
public:
  // INLINED
  UIScrollWnd(UIWindowContext& windows, int width, int height, unsigned char cached)
      : UIWindow(windows, cached), viewHeight(0), contentWidth(0), contentHeight(0), horizontalBar(nullptr), scrollBar(nullptr),
        scrollBarWidth(13), scrollBarHeight(13), unknown_009C(0), scrollPosition(0), unknown_00A4(0), wheelScroll(0) {
    this->UIWindow::Resize(width, height);
    this->Slot15(width, height);
  }
  ~UIScrollWnd() override;
  void Resize(int width, int height) override;
  void Slot15(int width, int height) override;
  void Slot35(int delta) override;
  int SendMsg(UIWindow* sender, int message, std::intptr_t argument1, std::intptr_t argument2, std::intptr_t argument3, std::intptr_t argument4) override;
  RenderPrimitive* DrawPrimitive() override;
  virtual void Slot53();

  RenderPrimitive* BuildPrimitive(int x, int y, int sourceX, int sourceY, int width, int height, CTexture* texture, std::uint32_t color);
  void Clear();
  void RemoveChild(UIWindow* child, unsigned char destroy);
  void AddScrollChild(UIWindow* child);
  void MoveScrollChild(UIWindow* child, int x, int y);

  // INLINED
  int& ViewWidth() {
    return reinterpret_cast<int&>(this->m_context);
  }

  bool Update(int redraw) override;
  bool UpdateCached(int redraw);
  bool UpdateChild(UIWindow* child, int redraw);

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
