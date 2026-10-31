#ifndef RAGNAROK_SOURCE_UI_ITEMS_UIITEMCOLLECTIONWND_H
#define RAGNAROK_SOURCE_UI_ITEMS_UIITEMCOLLECTIONWND_H

#include "UI/Buttons/UIItemMoveInfo.h"
#include "UI/Window/UIFrameWnd.h"
#include "UI/Window/UIResizer.h"

#include <cstdint>
#include <list>
#include <map>
#include <string>

class UIBitmapButton;
class UISpriteButton;
struct WindowPlacement;

class UIItemCollectionWnd : public UIFrameWnd {
public:
  UIItemCollectionWnd(UIWindowContext& windows, UIFrameContext& frames);
  void Slot11() override;
  void Slot15(int width, int height) override;

  // INLINED
  std::string& ItemIdText() {
    return this->m_item.itemId;
  }

  __declspec(property(get = ItemIdText)) std::string& m_itemIdText;

  /* 00B4 */ unsigned char unknown_00B4[0x4];
  /* 00B8 */ ITEM_INFO m_item;
  /* 01B0 */ UITextViewer* m_viewer;
  /* 01B4 */ UIBitmapButton* m_viewButton;
  /* 01B8 */ UISpriteButton* m_readButtons[2];
  /* 01C0 */ std::uint32_t m_previewTick;
  /* 01C4 */ std::string unknown_01C4;
  /* 01DC */ std::string unknown_01DC;
  /* 01F4 */ unsigned char unknown_01F4[0x4];
  /* 01F8 */ int m_kind;
  /* 01FC */ int unknown_01FC;
  /* 0200 */ WindowPlacement* m_placement;
  /* 0204 */ UIResizer* m_resizer;
  /* 0208 */ UIBitmapButton* m_closeButton;
  /* 020C */ unsigned char m_compare;
  /* 0210 */ UIItemMoveInfo* m_moveInfo;
  /* 0214 */ UIBitmapButton* m_helpButton;
  /* 0218 */ std::int16_t unknown_0218;
  /* 021A */ std::int16_t unknown_021A;
  /* 021C */ std::int16_t unknown_021C;
  /* 021E */ unsigned char unknown_021E[0x18];
  /* 0236 */ std::uint16_t unknown_0236;
  /* 0238 */ int unknown_0238;
  /* 023C */ int unknown_023C;
  /* 0240 */ std::map<int, std::list<std::string>> m_cardTexts;
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
  UICombinedCardItemCollectionWnd(UIWindowContext& windows, UIFrameContext& frames);
  void Slot15(int width, int height) override;
  void Move(int x, int y) override;
  void Slot11() override;
  void Slot31(int x, int y) override;

  /* 0250 */ WindowPlacement* m_cardPlacement;
};

class UIItemCollection_ComparisonWnd : public UIItemCollectionWnd {
public:
  UIItemCollection_ComparisonWnd(UIWindowContext& windows, UIFrameContext& frames);
  void Slot15(int width, int height) override;
  void Move(int x, int y) override;
  void Slot25(int x, int y) override;
  void Slot31(int x, int y) override;
};

class UICombinedCardItemCollection_ComparisonWnd : public UIItemCollectionWnd {
public:
  UICombinedCardItemCollection_ComparisonWnd(UIWindowContext& windows, UIFrameContext& frames);
  void Slot15(int width, int height) override;
  void Slot25(int x, int y) override;
  void Slot31(int x, int y) override;
};

#endif
