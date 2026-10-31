#ifndef RAGNAROK_SOURCE_UI_CHAT_UISUBCHATMINIWND_H
#define RAGNAROK_SOURCE_UI_CHAT_UISUBCHATMINIWND_H

#include "UI/Window/UIFrameWnd.h"

class UISubChatMiniWnd : public UIFrameWnd {
public:
  UISubChatMiniWnd(UIWindowContext& windows, UIFrameContext& frames);
  void Slot15(int width, int height) override;
  void Paint() override;
  void Slot25(int x, int y) override;
  int SendMsg(UIWindow* sender, int message, std::intptr_t a, std::intptr_t b, std::intptr_t c, std::intptr_t d) override;

private:
  /* 00B4 */ int unknown_00B4;
  /* 00B8 */ int m_rowHeight;
  /* 00BC */ int m_offsetY;
};

#endif
