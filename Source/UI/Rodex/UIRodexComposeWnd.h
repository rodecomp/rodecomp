#ifndef RAGNAROK_SOURCE_UI_RODEX_UIRODEXCOMPOSEWND_H
#define RAGNAROK_SOURCE_UI_RODEX_UIRODEXCOMPOSEWND_H

#include "Contents/Items/ItemInfo.h"
#include "UI/Edit/UIEditCtrl.h"
#include "UI/Lists/UIScrollBar.h"
#include "UI/Window/UIFrameWnd.h"

#include <cstdint>
#include <list>
#include <string>

class UIEditCtrl_Custom;
class UIEditCtrl_Currency;
class UIBmpTextButton;
class UIBasicButton;
class UIStaticText;

class UIRodexScrollBar : public UIScrollBar {
public:
  // INLINED
  explicit UIRodexScrollBar(UIWindowContext& windows) : UIScrollBar(windows) {
    this->arrowLength = 5;
  }

  void Paint() override;
};

class UIRodexEditCtrl3 : public UIEditCtrl3 {
public:
  void Slot15(int width, int height) override;
};

class UIRodexComposeWnd : public UIFrameWnd {
public:
  UIRodexComposeWnd(UIWindowContext& windows, UIFrameContext& frames);
  ~UIRodexComposeWnd() override;
  void Slot10(int x, int y, std::intptr_t value) override;
  void Slot19() override;
  void Slot25(int x, int y) override;
  void Slot32(int x, int y) override;
  ITEM_INFO GetItemAtPoint(int x, int y);
  int MeasureText(const std::string& text, const unsigned& bytes);
  void SetRecipient(int id, unsigned short job, unsigned short level, const char* name);
  std::int64_t GetZeny();

  /* 00B4 */ UIEditCtrl_Custom* unknown_00B4;
  /* 00B8 */ UIEditCtrl_Custom* unknown_00B8;
  /* 00BC */ UIBmpTextButton* unknown_00BC;
  /* 00C0 */ UIStaticText* unknown_00C0;
  /* 00C4 */ UIStaticText* unknown_00C4;
  /* 00C8 */ UIStaticText* unknown_00C8;
  /* 00CC */ int unknown_00CC;
  /* 00D0 */ UIEditCtrl_Custom* unknown_00D0;
  /* 00D4 */ UIRodexEditCtrl3* m_contents;
  /* 00D8 */ UIBasicButton* unknown_00D8;
  /* 00DC */ UIEditCtrl_Currency* m_zeny;
  /* 00E0 */ int unknown_00E0;
  /* 00E4 */ int unknown_00E4;
  /* 00E8 */ std::list<ITEM_INFO> m_items;
  /* 00F0 */ int m_itemIndex;
  /* 00F4 */ int unknown_00F4;
  /* 00F8 */ std::int64_t unknown_00F8;
  /* 0100 */ UIEditCtrl* unknown_0100;
};

#if defined(_WIN32) && !defined(_WIN64)
static_assert(sizeof(UIRodexComposeWnd) == 272);
#endif

#endif
