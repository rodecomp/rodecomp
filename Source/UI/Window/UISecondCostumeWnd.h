#ifndef RAGNAROK_SOURCE_UI_WINDOW_UISECONDCOSTUMEWND_H
#define RAGNAROK_SOURCE_UI_WINDOW_UISECONDCOSTUMEWND_H

#include "UI/Window/UIFrameWnd.h"

#include <string>
#include <vector>

class UIListBox;

struct SecondCostumeEntry {
  /* 0000 */ std::string name;
  /* 0018 */ int unknown_0018;
};

class UISecondCostumeWnd : public UIFrameWnd {
public:
  bool Slot03() override;
  void Paint() override;
  int SendMsg(UIWindow* sender, int message, std::intptr_t a, std::intptr_t b, std::intptr_t c, std::intptr_t d) override;
  __declspec(noinline) void RefreshList(int tab);
  int sub_8D2CE0(bool closeButton, const char* title, int width);
  void sub_948D20(int x, int y);

protected:
  /* 00B4 */ std::uint32_t unknown_00B4;
  /* 00B8 */ UIListBox* m_list;
  /* 00BC */ int m_tab;
  /* 00C0 */ int m_selected[4];
  /* 00D0 */ std::vector<SecondCostumeEntry> m_entries[4];
  /* 0100 */ std::uint32_t unknown_0100;
  /* 0104 */ int m_page;
};

#if defined(_WIN32) && !defined(_WIN64)
static_assert(sizeof(SecondCostumeEntry) == 0x1C);
#endif

#endif
