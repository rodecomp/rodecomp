#ifndef RAGNAROK_SOURCE_UI_PROBABILITYTABLE_UIPROBABILITYTABLE_H
#define RAGNAROK_SOURCE_UI_PROBABILITYTABLE_UIPROBABILITYTABLE_H

#include "UI/FrameSkin.h"
#include "UI/Lists/UIListBoxTemplate.h"
#include "UI/Text/UITextViewerControl.h"

#include <cstdint>
#include <string>

namespace ProbabilityTable {

class CItemData {
public:
  /* 0000 */ std::uint32_t color;
  /* 0004 */ std::string name;
  /* 001C */ int unknown_001C;
  /* 0020 */ double probability;
};

class CUIItem : public CUIListItem {
public:
  void Slot15(int width, int height) override;
  void Slot16() override;
  void Paint() override;
  void Slot25(int x, int y) override;

  /* 00E8 */ CUITextViewer* nameText;
  /* 00EC */ CUITextViewer* probabilityText;
  /* 00F0 */ std::uint32_t backgroundColor;
  /* 00F4 */ unsigned char unknown_00F4[0x04];
};

}

class CUIProbabilityTable : public CUIFrameWnd {
public:
  void Paint() override;
  virtual void Slot53();

  void OnButtonClick();
  void OnPrepareItem(ProbabilityTable::CUIItem* item, const ProbabilityTable::CItemData& data);
};

#if defined(_WIN32) && !defined(_WIN64)
static_assert(sizeof(ProbabilityTable::CItemData) == 0x28);
static_assert(sizeof(ProbabilityTable::CUIItem) == 0xF8);
#endif

#endif
