#ifndef RAGNAROK_SOURCE_UI_DIALOGS_UIPROHIBITLISTWND_H
#define RAGNAROK_SOURCE_UI_DIALOGS_UIPROHIBITLISTWND_H

#include "UI/Window/UIFrameWnd.h"

struct UIButtonContext;
struct UIEditContext;

class UIProhibitListWnd : public UIFrameWnd {
public:
  UIProhibitListWnd(UIWindowContext& windows, UIFrameContext& frames, UIButtonContext& controls, UIEditContext& edits);
  /* 00B4 */ int m_count;
  /* 00B8 */ std::uint32_t m_aid;

  void Slot15(int width, int height) override;
  void Paint() override;
  void Slot19() override;
  int SendMsg(UIWindow* sender, int message, std::intptr_t a, std::intptr_t b, std::intptr_t c, std::intptr_t d) override;

protected:
  UIButtonContext* m_controls;
  UIEditContext* m_edits;
};

#endif
