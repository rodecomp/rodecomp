#ifndef RAGNAROK_SOURCE_UI_DIALOGS_UIMINIMAPWND_H
#define RAGNAROK_SOURCE_UI_DIALOGS_UIMINIMAPWND_H

#include "UI/Window/UIFrameWnd.h"

#include <map>
#include <vector>

class UIBitmapButton;
class UIButton;
class UICheckBox;
class UIHeadsprButton;
struct MiniMapWndInfo;

struct MiniMapMark {
  /* 0000 */ int x;
  /* 0004 */ int y;
};

class UIMiniMapWnd : public UIFrameWnd {
public:
  UIMiniMapWnd();
  void StoreInfo() override;
  void OnCreate(int width, int height) override;
  void Paint() override;
  void OnLBtnDown(int x, int y) override;
  void ResetCursor(int x, int y) override;
  int SendMsg(UIWindow* sender, int message, std::intptr_t argument1, std::intptr_t argument2, std::intptr_t argument3, std::intptr_t argument4) override;
  void AddTownMarks();
  int LoadMemos();
  void UpdatePlayerHead();

  /* 00B4 */ MiniMapWndInfo* m_saved;
  /* 00B8 */ float scale;
  /* 00BC */ float originX;
  /* 00C0 */ float originY;
  /* 00C4 */ UIBitmapButton* memoButtons[20];
  /* 0114 */ int memoCount;
  /* 0118 */ unsigned char dragging;
  /* 0119 */ unsigned char unknown_0119[0x3];
  /* 011C */ UICheckBox* questCheckBox;
  /* 0120 */ UICheckBox* townCheckBox;
  /* 0124 */ UICheckBox* guildCheckBox;
  /* 0128 */ UIHeadsprButton* m_playerHead;
  /* 012C */ UIWindow* bossButton;
  /* 0130 */ std::map<int, UIBitmapButton*> townButtons;
  /* 0138 */ std::map<int, UIButton*> questButtons;
  /* 0140 */ std::map<int, MiniMapMark> questMarks;
  /* 0148 */ std::vector<MiniMapMark> partyMarks;
  /* 0154 */ std::vector<MiniMapMark> guildMarks;
  /* 0160 */ std::vector<MiniMapMark> warpMarks;
};

#endif
