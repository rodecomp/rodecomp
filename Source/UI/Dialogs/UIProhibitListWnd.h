#ifndef RAGNAROK_SOURCE_UI_DIALOGS_UIPROHIBITLISTWND_H
#define RAGNAROK_SOURCE_UI_DIALOGS_UIPROHIBITLISTWND_H

#include "UI/Window/UIFrameWnd.h"

struct UIButtonContext;

class UIProhibitListWnd : public UIFrameWnd {
public:
  /* 00B4 */ int m_count;
  /* 00B8 */ std::uint32_t m_aid;

  void Paint() override;
  void Slot19() override;

protected:
  UIButtonContext* m_controls;
};

#endif
