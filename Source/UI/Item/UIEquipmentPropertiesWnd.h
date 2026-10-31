#ifndef RAGNAROK_SOURCE_UI_ITEM_UIEQUIPMENTPROPERTIESWND_H
#define RAGNAROK_SOURCE_UI_ITEM_UIEQUIPMENTPROPERTIESWND_H

#include "UI/FrameSkin.h"
#include "UI/Lists/UIListBoxTemplate.h"

#include <cstddef>
#include <cstdint>
#include <string>

namespace EquipmentPropertiesWnd {
class CUIItemEquipmentPropertiesLine : public CUIListItem {
public:
  explicit CUIItemEquipmentPropertiesLine(UIWindowContext& context);

  /* 00E8 */ CUIStaticText* unknown_00E8;
  /* 00EC */ CUIStaticText* unknown_00EC;
  /* 00F0 */ std::uint32_t evenRowColor;
  /* 00F4 */ std::uint32_t oddRowColor;

  void Slot15(int width, int height) override;
  void Slot16() override;
  void Paint() override;
  void Slot53(int resolveSkin) override;
};
}

class CUIEquipmentPropertiesWnd : public CUIFrameWnd {
public:
  CUIEquipmentPropertiesWnd();
  /* 00D8 */ CUITitleBar* titleBar;
  /* 00DC */ CUIListBoxEx<EquipmentPropertiesWnd::CUIItemEquipmentPropertiesLine, std::string>* list;
  /* 00E0 */ CUITab* tabs;
  /* 00E4 */ CUIResizer* resizer;
  /* 00E8 */ int currentTab;

  bool Slot02() override;
  bool Slot03() override;
  bool Slot08() override;
  void Slot15(int width, int height) override;
  virtual void Slot53();
  void Grow(int width, int height) override;
  void Paint() override;
  void OnClickButton1();
  void OnClickButton2();
  void OnClickButton3();
  void OnClickButton4();
  void OnClickButton5();
  void OnClickButton6();
  void OnClickButton7();
  void OnClickButton8();
  void OnClickButton9();
  void OnClickClose();
  void OnSelectLine(EquipmentPropertiesWnd::CUIItemEquipmentPropertiesLine* line, const std::string& text);
  void BindLine(EquipmentPropertiesWnd::CUIItemEquipmentPropertiesLine* line, const std::string& text);
  void Slot16() override;
  int SendMsg(UIWindow* sender, int message, std::intptr_t argument1, std::intptr_t argument2, std::intptr_t argument3, std::intptr_t argument4) override;
  void SetTab(const int& tab);
  void OnChangeTab(CUITabButton* button);
};

template <>
CUIListBoxEx<EquipmentPropertiesWnd::CUIItemEquipmentPropertiesLine, std::string>::CUIListBoxEx(UIWindowContext& context);
template <>
CUIListBoxEx<EquipmentPropertiesWnd::CUIItemEquipmentPropertiesLine, std::string>::~CUIListBoxEx();
template <>
void CUIListBoxEx<EquipmentPropertiesWnd::CUIItemEquipmentPropertiesLine, std::string>::Slot15(int width, int height);
template <>
void CUIListBoxEx<EquipmentPropertiesWnd::CUIItemEquipmentPropertiesLine, std::string>::AddNewItem();
template <>
void CUIListBoxEx<EquipmentPropertiesWnd::CUIItemEquipmentPropertiesLine, std::string>::Resize(int width, int height);
template <>
void CUIListBoxEx<EquipmentPropertiesWnd::CUIItemEquipmentPropertiesLine, std::string>::Slot53(int resolveSkin);

#if defined(_WIN32) && !defined(_WIN64)
static_assert(sizeof(EquipmentPropertiesWnd::CUIItemEquipmentPropertiesLine) == 0xF8);
static_assert(offsetof(EquipmentPropertiesWnd::CUIItemEquipmentPropertiesLine, evenRowColor) == 0xF0);
static_assert(offsetof(EquipmentPropertiesWnd::CUIItemEquipmentPropertiesLine, oddRowColor) == 0xF4);
#endif

#endif
