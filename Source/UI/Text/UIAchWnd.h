#ifndef RAGNAROK_SOURCE_UI_TEXT_UIACHWND_H
#define RAGNAROK_SOURCE_UI_TEXT_UIACHWND_H

#include "UI/Text/UITextViewer.h"
#include "UI/Window/UIWindow.h"

struct UIAchListInfo {
  /* 0000 */ int unknown_0000;
  /* 0004 */ int count;
};

class UIAchListWnd : public UIWindow {
public:
  /* 0080 */ int unknown_0080;
  /* 0084 */ int unknown_0084;
  /* 0088 */ int unknown_0088;
  /* 008C */ int unknown_008C;
  /* 0090 */ int unknown_0090;
  /* 0094 */ int unknown_0094;
  /* 0098 */ unsigned char unknown_0098[0x14];
  /* 00AC */ UIAchListInfo* unknown_00AC;

  void sub_7B37F0(int index);

  void Slot25(int x, int y) override;
  void Slot31(int x, int y) override;
};

class UIAchTab : public UIWindow {
public:
  /* 007C */ int unknown_007C;
  /* 0080 */ int unknown_0080;

  bool sub_7B4370(int x, int y, int* a, int* b);
  void sub_7B7F10();

  void Slot25(int x, int y) override;
  void Slot31(int x, int y) override;
};

class UIAchTV : public UITextViewer {
public:
  /* 00EC */ unsigned char unknown_00EC;

  void Slot25(int x, int y) override;
};

#endif
