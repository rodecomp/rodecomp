#ifndef RAGNAROK_SOURCE_UI_WINDOW_UINEWSELECTCHARWND_H
#define RAGNAROK_SOURCE_UI_WINDOW_UINEWSELECTCHARWND_H

#include "UI/Window/UIFrameWnd.h"
#include <windows.h>
#include <map>
#include <string>
#include <vector>

class UIBitmapButton;
class UIBmpTextButton;
class UIStaticText;
class CUIControl;

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
  UINewSelectCharWnd();
  ~UINewSelectCharWnd() override;
  void DisableButtonById(int id);
  void EnableButtonById(int id);
  void SaveCurrentSlot();

  void Slot15(int width, int height) override;
  void Slot19() override;
  void Paint() override;
  int SendMsg(UIWindow* sender, int message, std::intptr_t argument1, std::intptr_t argument2, std::intptr_t argument3, std::intptr_t argument4) override;

  void sub_7D90E0();
  void sub_7D9260();
  void sub_7D9310();

protected:
  /* 00B4 */ UIBmpTextButton* m_textButton;
  /* 00B8 */ UIBitmapButton* m_button416;
  /* 00BC */ UIBitmapButton* m_button417;
  /* 00C0 */ UIWindow* m_windowC0;
  /* 00C4 */ UIWindow* m_windowC4;
  /* 00C8 */ std::map<int, int> m_slotMap;
  /* 00D0 */ std::vector<UIWindow*> m_slotButtons;
  /* 00DC */ std::vector<UIStaticText*> m_characterWindows;
  /* 00E8 */ std::vector<UIWindow*> m_slotButtons2;
  /* 00F4 */ unsigned char unknown_00F4[8];
  /* 00FC */ UIWindow* m_windowFC;
  /* 0100 */ UIWindow* m_window100;
  /* 0104 */ int m_unknown0104;
  /* 0108 */ CharSlotInfo* m_charSlotInfos;
  /* 010C */ char* m_slotStatus;
  /* 0110 */ short* m_slotShorts;
  /* 0114 */ DWORD m_lastTick;
  /* 0118 */ bool m_unknown118;
  /* 011C */ int m_page;
  /* 0120 */ int m_pageSlot;
  /* 0124 */ int m_pageCount;
  /* 0128 */ int m_slotsPerPage;
  /* 012C */ int m_totalSlots;
  /* 0130 */ int m_totalCharacters;
  /* 0134 */ int m_maxCharacters;
  /* 0138 */ int m_unknown138;
  /* 013C */ bool m_unknown13C;
};

#endif
