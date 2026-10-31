#ifndef RAGNAROK_SOURCE_UI_WINDOW_UIBASICINFOWND_H
#define RAGNAROK_SOURCE_UI_WINDOW_UIBASICINFOWND_H

#include "Contents/UserData/NormalItemSlots.h"
#include "UI/Window/UIFrameWnd.h"

class UIStaticText;

class UIBasicInfoWnd : public UIFrameWnd {
public:
  bool Slot03() override;
  void Slot11() override;
  void Slot25(int x, int y) override;
  void Slot26(int x, int y) override;
  void Slot27(int x, int y) override;
  void Slot28(int x, int y) override;

  void SetHeight(int height);
  void RefreshLayout();

  /* 00B4 */ int collapsedHeight;
  /* 00B8 */ int expandedHeight;
  /* 00BC */ unsigned char mode;
  /* 00BD */ unsigned char unknown_00BD[0x3];
  /* 00C0 */ UIStaticText* nameText;
  /* 00C4 */ BasicInfoWndInfo* saved;
  /* 00C8 */ unsigned char unknown_00C8[0x20];
  /* 00E8 */ UIWindow* hpBar;
  /* 00EC */ UIWindow* spBar;
  /* 00F0 */ UIWindow* baseExpBar;
  /* 00F4 */ UIWindow* jobExpBar;
  /* 00F8 */ int layoutOffset;
  /* 00FC */ UIWindow* apBar;
  /* 0100 */ int normalOffset;
  /* 0104 */ int normalCollapsedHeight;
  /* 0108 */ int normalExpandedHeight;
  /* 010C */ int extendedOffset;
  /* 0110 */ int extendedCollapsedHeight;
  /* 0114 */ int extendedExpandedHeight;

protected:
  UIButtonContext* m_controls;
};

#endif
