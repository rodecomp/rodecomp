#ifndef RAGNAROK_SOURCE_UI_ITEMS_UISPELLLISTWND_H
#define RAGNAROK_SOURCE_UI_ITEMS_UISPELLLISTWND_H

#include "UI/Items/UIItemIdentifyWnd.h"

#include <cstdint>
#include <vector>

struct UIButtonContext;

class UISpellListWnd : public UIItemIdentifyWnd {
public:
  UISpellListWnd(UIWindowContext& windows);
  ~UISpellListWnd() override;
  /* 00F0 */ std::vector<std::uint32_t> m_skills;
  /* 00FC */ std::int16_t unknown_00FC;

  void Paint() override;
  int SendMsg(UIWindow* sender, int message, std::intptr_t argument1, std::intptr_t argument2, std::intptr_t argument3, std::intptr_t argument4) override;

protected:
  UIButtonContext* m_controls;
};

#endif
