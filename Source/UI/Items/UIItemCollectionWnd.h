#ifndef RAGNAROK_SOURCE_UI_ITEMS_UIITEMCOLLECTIONWND_H
#define RAGNAROK_SOURCE_UI_ITEMS_UIITEMCOLLECTIONWND_H

#include "UI/Window/UIFrameWnd.h"

class UIItemCollectionWnd : public UIFrameWnd {
public:
  /* 00B4 */ unsigned char unknown_00B4[0x108];
  /* 01BC */ UIWindow* m_preview;
  /* 01C0 */ std::uint32_t m_previewTick;

  void Slot19() override;
};

class UIItemCollection_ComparisonWnd : public UIItemCollectionWnd {
public:
  void Slot25(int x, int y) override;
  void Slot31(int x, int y) override;
};

class UICombinedCardItemCollection_ComparisonWnd : public UIItemCollectionWnd {
public:
  void Slot25(int x, int y) override;
  void Slot31(int x, int y) override;
};

#endif
