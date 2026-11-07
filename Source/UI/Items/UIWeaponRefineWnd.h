#ifndef RAGNAROK_SOURCE_UI_ITEMS_UIWEAPONREFINEWND_H
#define RAGNAROK_SOURCE_UI_ITEMS_UIWEAPONREFINEWND_H

#include "UI/Window/UIFrameWnd.h"

#include <vector>

class UIListBox;

class UIWeaponRefineWnd : public UIFrameWnd {
public:
  UIWeaponRefineWnd();
  std::vector<unsigned char> GetReplayData() override;
  void OnCreate(int width, int height) override;
  void Paint() override;
  int SendMsg(UIWindow* sender, int message, std::intptr_t a, std::intptr_t b, std::intptr_t c, std::intptr_t d) override;

  /* 00B4 */ UIListBox* m_list;

protected:

public:
  /* 00B8 */ std::vector<int> itemIds;
  /* 00C4 */ std::vector<int> indices;
};

#endif
