#ifndef RAGNAROK_SOURCE_UI_ITEMS_UIITEMCOLLECTIONWND_H
#define RAGNAROK_SOURCE_UI_ITEMS_UIITEMCOLLECTIONWND_H

#include "UI/Window/UIFrameWnd.h"

struct WindowPlacement;

class UIItemCollectionWnd : public UIFrameWnd {
public:
  void Slot11() override;
  /* 00B4 */ unsigned char unknown_00B4[0x108];
  /* 01BC */ UIWindow* m_preview;
  /* 01C0 */ std::uint32_t m_previewTick;
  /* 01C4 */ unsigned char unknown_01C4[0x34];
  /* 01F8 */ int m_kind;
  /* 01FC */ unsigned char unknown_01FC[0x4];
  /* 0200 */ WindowPlacement* m_placement;

  void Move(int x, int y) override;
  void Slot19() override;
  void Slot25(int x, int y) override;
  void Slot31(int x, int y) override;
};

class UICombinedCardItemCollectionWnd : public UIItemCollectionWnd {
public:
  void Move(int x, int y) override;
  void Slot11() override;
  void Slot31(int x, int y) override;

  /* 0204 */ unsigned char unknown_0204[0x4C];
  /* 0250 */ WindowPlacement* m_cardPlacement;
};

class UIItemCollection_ComparisonWnd : public UIItemCollectionWnd {
public:
  void Move(int x, int y) override;
  void Slot25(int x, int y) override;
  void Slot31(int x, int y) override;
};

class UICombinedCardItemCollection_ComparisonWnd : public UIItemCollectionWnd {
public:
  void Slot25(int x, int y) override;
  void Slot31(int x, int y) override;
};

#endif
