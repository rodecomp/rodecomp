#ifndef RAGNAROK_SOURCE_UI_ITEMS_UIITEMCOLLECTIONWND_H
#define RAGNAROK_SOURCE_UI_ITEMS_UIITEMCOLLECTIONWND_H

#include "UI/Buttons/UIItemMoveInfo.h"
#include "UI/Window/UIFrameWnd.h"
#include "UI/Window/UIResizer.h"

struct WindowPlacement;

class UIItemCollectionWnd : public UIFrameWnd {
public:
  void Slot11() override;
  /* 00B4 */ unsigned char unknown_00B4[0x30];
  /* 00E4 */ std::string m_itemIdText;
  /* 00FC */ unsigned char unknown_00FC[0xB4];
  /* 01B0 */ UITextViewer* m_viewer;
  /* 01B4 */ unsigned char unknown_01B4[0x8];
  /* 01BC */ UIWindow* m_preview;
  /* 01C0 */ std::uint32_t m_previewTick;
  /* 01C4 */ unsigned char unknown_01C4[0x34];
  /* 01F8 */ int m_kind;
  /* 01FC */ unsigned char unknown_01FC[0x4];
  /* 0200 */ WindowPlacement* m_placement;
  /* 0204 */ UIResizer* m_resizer;
  /* 0208 */ unsigned char unknown_0208[0x4];
  /* 020C */ unsigned char m_compare;
  /* 0210 */ UIItemMoveInfo* m_moveInfo;
  /* 0214 */ unsigned char unknown_0214[0x34];
  /* 0248 */ int m_extraHeight;
  /* 024C */ unsigned char unknown_024C[0x4];

  void Move(int x, int y) override;
  void Slot19() override;
  void Slot25(int x, int y) override;
  void Slot31(int x, int y) override;
  void SetHeight(int height);
};

class UICombinedCardItemCollectionWnd : public UIItemCollectionWnd {
public:
  void Move(int x, int y) override;
  void Slot11() override;
  void Slot31(int x, int y) override;

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
