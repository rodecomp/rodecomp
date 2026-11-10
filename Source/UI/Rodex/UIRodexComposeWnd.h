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
  UIRodexScrollBar();

  void Paint() override;
};

class UIRodexEditCtrl3 : public UIEditCtrl3 {
public:
  UIRodexEditCtrl3();
  void OnCreate(int width, int height) override;
};

class UIRodexComposeWnd : public UIFrameWnd {
public:
  UIRodexComposeWnd();
  ~UIRodexComposeWnd() override;
  void DragAndDrop(int x, int y, std::intptr_t value) override;
  void OnCreate(int width, int height) override;
  void OnProcess() override;
  void OnLBtnDown(int x, int y) override;
  void OnMouseHover(int x, int y) override;
  void OnRBtnDown(int x, int y) override;
  void Paint() override;
  int SendMsg(UIWindow* sender, int message, std::intptr_t argument1, std::intptr_t argument2, std::intptr_t argument3, std::intptr_t argument4) override;
  void InsertItemTag(const ITEM_INFO& item);
  void InsertSkillTag(int tagId);
  ITEM_INFO GetItemAtPoint(int x, int y);
  int MeasureText(const std::string& text, const unsigned& bytes);
  void SetRecipient(int id, unsigned short job, unsigned short level, const char* name);
  __declspec(noinline) std::int64_t GetZeny();

  /* 00B4 */ UIEditCtrl_Custom* m_recipientEdit;
  /* 00B8 */ UIEditCtrl_Custom* m_confirmedRecipientEdit;
  /* 00BC */ UIBmpTextButton* m_confirmRecipientButton;
  /* 00C0 */ UIStaticText* m_recipientLevelText;
  /* 00C4 */ UIStaticText* m_recipientJobText;
  /* 00C8 */ UIStaticText* m_recipientIdText;
  /* 00CC */ int m_recipientId;
  /* 00D0 */ UIEditCtrl_Custom* m_titleEdit;
  /* 00D4 */ UIRodexEditCtrl3* m_contents;
  /* 00D8 */ UIBasicButton* m_sendButton;
  /* 00DC */ UIEditCtrl_Currency* m_zeny;
  /* 00E0 */ int m_reservedStateA;
  /* 00E4 */ int m_reservedStateB;
  /* 00E8 */ std::list<ITEM_INFO> m_items;
  /* 00F0 */ int m_itemIndex;
  /* 00F4 */ int padding_00F4;
  /* 00F8 */ std::int64_t m_tax;
  /* 0100 */ UIEditCtrl* m_tagCtrl;
};

#if defined(_WIN32) && !defined(_WIN64)
static_assert(sizeof(UIRodexComposeWnd) == 0x108);
#endif

#endif
