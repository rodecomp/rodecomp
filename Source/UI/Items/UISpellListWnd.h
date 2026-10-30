#ifndef RAGNAROK_SOURCE_UI_ITEMS_UISPELLLISTWND_H
#define RAGNAROK_SOURCE_UI_ITEMS_UISPELLLISTWND_H

#include "UI/Items/UIItemIdentifyWnd.h"

#include <cstdint>
#include <vector>

struct UIButtonContext;

class UISpellListWnd : public UIItemIdentifyWnd {
public:
  /* 00F0 */ std::vector<std::uint32_t> m_skills;

  void Paint() override;

protected:
  UIButtonContext* m_controls;
};

#endif
