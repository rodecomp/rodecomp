#ifndef RAGNAROK_SOURCE_UI_ITEMS_UIITEMCOLLECTIONWND_H
#define RAGNAROK_SOURCE_UI_ITEMS_UIITEMCOLLECTIONWND_H

#include "UI/Window/UIFrameWnd.h"

class UIItemCollectionWnd : public UIFrameWnd {
public:
  /* 00B4 */ unsigned char unknown_00B4[0x108];
  /* 01BC */ UIWindow* m_preview;
  /* 01C0 */ std::uint32_t m_previewTick;
  /* 01C4 */ unsigned char unknown_01C4[0x34];
  /* 01F8 */ int m_kind;

  void Move(int x, int y) override;
  void Slot19() override;
  void Slot25(int x, int y) override;
  void Slot31(int x, int y) override;
};

class UICombinedCardItemCollectionWnd : public UIItemCollectionWnd {
public:
  void Move(int x, int y) override;
  void Slot31(int x, int y) override;
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
