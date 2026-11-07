#ifndef RAGNAROK_SOURCE_UI_DIALOGS_UICHOOSEWND_H
#define RAGNAROK_SOURCE_UI_DIALOGS_UICHOOSEWND_H

#include "UI/Window/UIFrameWnd.h"

#include <vector>

struct ChooseWndInfo;
class UIBitmapButton;
class UIListBox;

class UIChooseWnd : public UIFrameWnd {
public:
  UIChooseWnd();
  ~UIChooseWnd() override;
  void StoreInfo() override;
  void OnCreate(int width, int height) override;
  void Paint() override;
  int SendMsg(UIWindow* sender, int message, std::intptr_t a, std::intptr_t b, std::intptr_t c, std::intptr_t d) override;
  std::vector<unsigned char> GetReplayData() override;

  /* 00B4 */ int unknown_00B4;
  /* 00B8 */ std::vector<int> m_items;
  /* 00C4 */ UIListBox* m_list;
  /* 00C8 */ int unknown_00C8;
  /* 00CC */ ChooseWndInfo* m_placement;
  /* 00D0 */ UIBitmapButton* m_okButton;
  /* 00D4 */ UIBitmapButton* m_cancelButton;
};

class UIChoose3Wnd : public UIChooseWnd {
public:
  // INLINED
  UIChoose3Wnd() : m_minimumLines(4), m_maximumLines(15) {
  }

  int SendMsg(UIWindow* sender, int message, std::intptr_t a, std::intptr_t b, std::intptr_t c, std::intptr_t d) override;

  /* 00D8 */ int m_minimumLines;
  /* 00DC */ int m_maximumLines;
};

#endif
