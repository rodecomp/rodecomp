#ifndef RAGNAROK_SOURCE_UI_WINDOW_UISERACHSTOREINFOWND_H
#define RAGNAROK_SOURCE_UI_WINDOW_UISERACHSTOREINFOWND_H

#include "UI/Window/UIFrameWnd.h"

#include <string>

class UIBitmapButton;
class UIEditCtrl;
class UIRadioBtn;
class UITextButton;

class UISerachStoreInfoWnd : public UIFrameWnd {
public:
  UISerachStoreInfoWnd();
  void OnCreate(int width, int height) override;
  void Paint() override;
  void OnLBtnDblClk(int x, int y) override;
  void OnMouseHover(int x, int y) override;
  void OnRBtnDown(int x, int y) override;
  int SendMsg(UIWindow* sender, int message, std::intptr_t argument1, std::intptr_t argument2, std::intptr_t argument3, std::intptr_t argument4) override;
  __declspec(noinline) ITEM_INFO ItemAt(int x, int y);

protected:
  /* 00B4 */ UIRadioBtn* m_radios[2];
  /* 00BC */ UIEditCtrl* m_edits[4];
  /* 00CC */ UIBitmapButton* m_findButton;
  /* 00D0 */ UITextButton* m_prevButton;
  /* 00D4 */ UITextButton* m_nextButton;
  /* 00D8 */ UIWindow* padding_00D8;
  /* 00DC */ int m_page;
  /* 00E0 */ int m_hoverRow;
  /* 00E4 */ int m_searchType;
  /* 00E8 */ int m_rowHeight;
  /* 00EC */ unsigned char m_searchPending;
  /* 00ED */ unsigned char m_matchOption;
  /* 00F0 */ std::string m_statusText;

};

#endif
