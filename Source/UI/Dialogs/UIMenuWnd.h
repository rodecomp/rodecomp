#ifndef RAGNAROK_SOURCE_UI_DIALOGS_UIMENUWND_H
#define RAGNAROK_SOURCE_UI_DIALOGS_UIMENUWND_H

#include "UI/Window/UIFrameWnd.h"

#include <cstdint>
#include <string>
#include <vector>

struct UIButtonContext;
class UIMenuWnd : public UIFrameWnd {
public:
  /* 00B4 */ int m_menuId;
  /* 00B8 */ int m_selected;
  /* 00BC */ int m_itemHeight;
  /* 00C0 */ std::vector<std::string> m_items;
  /* 00CC */ std::uint32_t unknown_00CC;
  /* 00D0 */ std::vector<std::uint32_t> m_colors;

  void Move(int x, int y) override;
  void Slot15(int width, int height) override;
  void Paint() override;
  void Slot27(int x, int y) override;
  void Slot28(int x, int y) override;
  void Slot31(int x, int y) override;

  void AddItem(const char* text, const std::uint32_t& color);
  int ItemAt(int x, int y);

protected:
  UIButtonContext* m_controls;
};

extern const char* g_menuSeparator;

#endif
