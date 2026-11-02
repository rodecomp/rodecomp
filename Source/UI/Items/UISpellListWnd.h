#ifndef RAGNAROK_SOURCE_UI_ITEMS_UISPELLLISTWND_H
#define RAGNAROK_SOURCE_UI_ITEMS_UISPELLLISTWND_H

#include "UI/Items/UIItemIdentifyWnd.h"

#include <cstdint>
#include <vector>

struct UIButtonContext;

class UISpellListWnd : public UIItemIdentifyWnd {
public:
  UISpellListWnd(UIWindowContext& windows, UIFrameContext& frames);
  ~UISpellListWnd() override;
  /* 00F0 */ std::vector<std::uint32_t> m_skills;
  /* 00FC */ std::int16_t unknown_00FC;

  void Paint() override;

protected:
  UIButtonContext* m_controls;
};

#endif
