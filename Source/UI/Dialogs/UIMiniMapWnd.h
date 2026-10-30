#ifndef RAGNAROK_SOURCE_UI_DIALOGS_UIMINIMAPWND_H
#define RAGNAROK_SOURCE_UI_DIALOGS_UIMINIMAPWND_H

#include "UI/Window/UIFrameWnd.h"

#include <map>
#include <vector>

class UIBitmapButton;
struct UIButtonContext;

struct MiniMapMark {
  /* 0000 */ int x;
  /* 0004 */ int y;
};

class UIMiniMapWnd : public UIFrameWnd {
public:
  void Slot25(int x, int y) override;
  void ResetCursor(int x, int y) override;
  void AddTownMarks();
  int LoadMemos();

  /* 00B4 */ unsigned char unknown_00B4[0x4];
  /* 00B8 */ float scale;
  /* 00BC */ float originX;
  /* 00C0 */ float originY;
  /* 00C4 */ UIBitmapButton* memoButtons[20];
  /* 0114 */ unsigned char unknown_0114[0x4];
  /* 0118 */ unsigned char dragging;
  /* 0119 */ unsigned char unknown_0119[0x3];
  /* 011C */ UIWindow* unknown_011C;
  /* 0120 */ UIWindow* unknown_0120;
  /* 0124 */ UIWindow* unknown_0124;
  /* 0128 */ UIWindow* unknown_0128;
  /* 012C */ UIWindow* unknown_012C;
  /* 0130 */ std::map<int, UIBitmapButton*> townButtons;
  /* 0138 */ unsigned char unknown_0138[0x10];
  /* 0148 */ std::vector<MiniMapMark> partyMarks;
  /* 0154 */ std::vector<MiniMapMark> guildMarks;
  /* 0160 */ std::vector<MiniMapMark> warpMarks;

protected:
  UIButtonContext* m_controls;
};

#endif
