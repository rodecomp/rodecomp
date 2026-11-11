#ifndef RAGNAROK_SOURCE_UI_PROBABILITYTABLE_UIPROBABILITYTABLE_H
#define RAGNAROK_SOURCE_UI_PROBABILITYTABLE_UIPROBABILITYTABLE_H

#include "UI/Window/FrameSkin.h"
#include "UI/Lists/UIListBoxTemplate.h"
#include "UI/Text/UITextViewerControl.h"

#include <cstdint>
#include <string>
#include <string_view>

namespace ProbabilityTable {

class CItemData {
public:
  /* 0000 */ std::uint32_t color = 0xFFFFFFFF;
  /* 0004 */ std::string name;
  /* 001C */ int padding_001C;
  /* 0020 */ double probability = 0.0;
};

class CUIItem : public CUIListItem {
public:
  CUIItem();
  void OnCreate(int width, int height) override;
  void OnInit() override;
  void Paint() override;
  void OnLBtnDown(int x, int y) override;

  /* 00E8 */ CUITextViewer* nameText;
  /* 00EC */ CUITextViewer* probabilityText;
  /* 00F0 */ std::uint32_t backgroundColor;
  /* 00F4 */ unsigned char padding_00F4[0x04];
};

}
class CUIEditBoxEx;
class CUIImageButton;
class CUITitleBar;
class CUIStaticText;
class CUIProbabilityTable : public CUIFrameWnd {
public:
  __declspec(noinline) CUIProbabilityTable();

  void OnCreate(int width, int height) override;
  void OnInit() override;
  void Paint() override;
  virtual void Initialize();
  int SendMsg(UIWindow* sender, int message, std::intptr_t argument1, std::intptr_t argument2, std::intptr_t argument3, std::intptr_t argument4) override;
  void OnButtonClick();
  void OnClose();
  void OnPrepareItem(ProbabilityTable::CUIItem* item, const ProbabilityTable::CItemData& data);
  void BindItem(ProbabilityTable::CUIItem* item, const ProbabilityTable::CItemData& data);
  void Search();
  void SetItem(int itemId);

  /* 00D8 */ CUITitleBar* m_titleBar;
  /* 00DC */ CUIStaticText* m_nameHeader;
  /* 00E0 */ CUIEditBoxEx* m_searchEdit;
  union {
    /* 00E4 */ unsigned char m_searchButtonStorage[0x04];
    /* 00E4 */ CUIImageButton* m_searchButton;
  };
  /* 00E8 */ CUIStaticText* m_itemName;
  /* 00EC */ CUIStaticText* m_probabilityHeader;
  /* 00F0 */ CUIListBoxEx<ProbabilityTable::CUIItem, ProbabilityTable::CItemData>* m_list;
  /* 00F4 */ std::string m_lastSearch;
  /* 010C */ int m_itemId;
};

int HighlightKeyword(std::string& text, std::string_view keyword, std::string_view prefix, std::string_view suffix);

#if defined(_WIN32) && !defined(_WIN64)
static_assert(sizeof(ProbabilityTable::CItemData) == 0x28);
static_assert(sizeof(ProbabilityTable::CUIItem) == 0xF8);
#endif

#endif
