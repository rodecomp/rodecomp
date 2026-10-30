#ifndef RAGNAROK_SOURCE_UI_WINDOW_UINEWSELECTCHARWND_H
#define RAGNAROK_SOURCE_UI_WINDOW_UINEWSELECTCHARWND_H

#include "UI/Window/UIFrameWnd.h"

#include <string>

class UIBitmapButton;
class UIBmpTextButton;

struct CharSlotInfo {
  CharSlotInfo();
  ~CharSlotInfo();

  /* 0000 */ int unknown_0000;
  /* 0004 */ int unknown_0004;
  /* 0008 */ std::string unknown_0008[5];
  /* 0080 */ std::string unknown_0080[5];
  /* 00F8 */ std::string unknown_00F8;
  /* 0110 */ int unknown_0110;
  /* 0114 */ int unknown_0114;
  /* 0118 */ std::string unknown_0118;
  /* 0130 */ std::string unknown_0130;
  /* 0148 */ unsigned char unknown_0148[20];
};

#if defined(_WIN32) && !defined(_WIN64)
static_assert(sizeof(CharSlotInfo) == 348);
#endif

class UINewSelectCharWnd : public UIFrameWnd {
public:
  void DisableButton(int id);
  void SaveCurrentSlot();

protected:
  /* 00B4 */ UIBmpTextButton* m_textButton;
  /* 00B8 */ UIBitmapButton* m_button416;
  /* 00BC */ UIBitmapButton* m_button417;
  /* 00C0 */ unsigned char unknown_00C0[92];
  /* 011C */ int m_page;
  /* 0120 */ int m_pageSlot;
  /* 0124 */ int m_pageCount;
  /* 0128 */ int m_slotsPerPage;
};

#endif
