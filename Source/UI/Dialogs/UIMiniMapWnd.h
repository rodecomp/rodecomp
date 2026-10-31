#ifndef RAGNAROK_SOURCE_UI_DIALOGS_UIMINIMAPWND_H
#define RAGNAROK_SOURCE_UI_DIALOGS_UIMINIMAPWND_H

#include "UI/Window/UIFrameWnd.h"

#include <map>
#include <vector>

class UIBitmapButton;
class UIHeadsprButton;
struct MiniMapWndInfo;
struct UIButtonContext;

struct MiniMapMark {
  /* 0000 */ int x;
  /* 0004 */ int y;
};

class UIMiniMapWnd : public UIFrameWnd {
public:
  UIMiniMapWnd(UIWindowContext& windows, UIFrameContext& frames, UIButtonContext& controls);
  void Slot11() override;
  void Slot15(int width, int height) override;
  void Paint() override;
  void Slot25(int x, int y) override;
  void ResetCursor(int x, int y) override;
  void AddTownMarks();
  int LoadMemos();
  void UpdatePlayerHead();

  /* 00B4 */ MiniMapWndInfo* m_saved;
  /* 00B8 */ float scale;
  /* 00BC */ float originX;
  /* 00C0 */ float originY;
  /* 00C4 */ UIBitmapButton* memoButtons[20];
  /* 0114 */ int unknown_0114;
  /* 0118 */ unsigned char dragging;
  /* 0119 */ unsigned char unknown_0119[0x3];
  /* 011C */ UIWindow* unknown_011C;
  /* 0120 */ UIWindow* unknown_0120;
  /* 0124 */ UIWindow* unknown_0124;
  /* 0128 */ UIHeadsprButton* m_playerHead;
  /* 012C */ UIWindow* unknown_012C;
  /* 0130 */ std::map<int, UIBitmapButton*> townButtons;
  /* 0138 */ std::map<int, UIWindow*> unknown_0138;
  /* 0140 */ std::map<int, MiniMapMark> unknown_0140;
  /* 0148 */ std::vector<MiniMapMark> partyMarks;
  /* 0154 */ std::vector<MiniMapMark> guildMarks;
  /* 0160 */ std::vector<MiniMapMark> warpMarks;

protected:
  UIButtonContext* m_controls;
};

#endif
