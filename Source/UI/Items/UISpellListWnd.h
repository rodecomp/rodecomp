#ifndef RAGNAROK_SOURCE_UI_ITEMS_UISPELLLISTWND_H
#define RAGNAROK_SOURCE_UI_ITEMS_UISPELLLISTWND_H

#include "UI/Items/UIItemIdentifyWnd.h"

#include <cstdint>
#include <vector>

struct UIButtonContext;

class UISpellListWnd : public UIItemIdentifyWnd {
public:
  /* 00D4 */ unsigned char unknown_00D4[0x1C];
  /* 00F0 */ std::vector<std::uint32_t> m_skills;

  void Paint() override;

protected:
  UIButtonContext* m_controls;
};

#endif
