#ifndef RAGNAROK_SOURCE_UI_ITEMS_UIEQUIPMENTPROPERTIESWND_H
#define RAGNAROK_SOURCE_UI_ITEMS_UIEQUIPMENTPROPERTIESWND_H

#include "UI/Window/FrameSkin.h"
#include "UI/Lists/UIListBoxTemplate.h"

#include <cstddef>
#include <cstdint>
#include <string>

namespace EquipmentPropertiesWnd {
class CUIItemEquipmentPropertiesLine : public CUIListItem {
public:
  CUIItemEquipmentPropertiesLine();

  /* 00E8 */ CUIStaticText* labelText;
  /* 00EC */ CUIStaticText* valueText;
  /* 00F0 */ std::uint32_t evenRowColor;
  /* 00F4 */ std::uint32_t oddRowColor;

  void OnCreate(int width, int height) override;
  void OnInit() override;
  void Paint() override;
  void LoadSkin(bool resolveSkin) override;
};
}

class CUIEquipmentPropertiesWnd : public CUIFrameWnd {
public:
  CUIEquipmentPropertiesWnd();
  ~CUIEquipmentPropertiesWnd() override;
  /* 00D8 */ CUITitleBar* titleBar;
  /* 00DC */ CUIListBoxEx<EquipmentPropertiesWnd::CUIItemEquipmentPropertiesLine, std::string>* list;
  /* 00E0 */ CUITab* tabs;
  /* 00E4 */ CUIResizer* resizer;
  /* 00E8 */ int currentTab;

  bool IsFrameWnd() override;
  bool NeedsFrameRefresh() override;
  bool IsTransmitMouseInput() override;
  void OnCreate(int width, int height) override;
  virtual void Initialize();
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
  void OnInit() override;
  int SendMsg(UIWindow* sender, int message, std::intptr_t argument1, std::intptr_t argument2, std::intptr_t argument3, std::intptr_t argument4) override;
  void SetTab(const int& tab);
  void OnChangeTab(CUITabButton* button);
};

template <>
CUIListBoxEx<EquipmentPropertiesWnd::CUIItemEquipmentPropertiesLine, std::string>::CUIListBoxEx();
template <>
CUIListBoxEx<EquipmentPropertiesWnd::CUIItemEquipmentPropertiesLine, std::string>::~CUIListBoxEx();
template <>
void CUIListBoxEx<EquipmentPropertiesWnd::CUIItemEquipmentPropertiesLine, std::string>::OnCreate(int width, int height);
template <>
void CUIListBoxEx<EquipmentPropertiesWnd::CUIItemEquipmentPropertiesLine, std::string>::AddNewItem();
template <>
void CUIListBoxEx<EquipmentPropertiesWnd::CUIItemEquipmentPropertiesLine, std::string>::Resize(int width, int height);
template <>
void CUIListBoxEx<EquipmentPropertiesWnd::CUIItemEquipmentPropertiesLine, std::string>::LoadSkin(bool resolveSkin);

#if defined(_WIN32) && !defined(_WIN64)
static_assert(sizeof(EquipmentPropertiesWnd::CUIItemEquipmentPropertiesLine) == 0xF8);
static_assert(offsetof(EquipmentPropertiesWnd::CUIItemEquipmentPropertiesLine, evenRowColor) == 0xF0);
static_assert(offsetof(EquipmentPropertiesWnd::CUIItemEquipmentPropertiesLine, oddRowColor) == 0xF4);
#endif

#endif
