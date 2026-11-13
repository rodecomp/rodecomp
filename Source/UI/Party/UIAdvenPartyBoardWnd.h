#ifndef RAGNAROK_SOURCE_UI_PARTY_UIADVENPARTYBOARDWND_H
#define RAGNAROK_SOURCE_UI_PARTY_UIADVENPARTYBOARDWND_H

#include "Contents/Party/AdvenPartyInfo.h"
#include "UI/Window/UIFrameWnd.h"
#include "UI/Lists/UIListBoxEx.h"

#include <cstddef>
#include <vector>

enum AdvenPartyBoardButtonId : int {
  ADVENPARTY_BUTTON_STOP_RECRUIT = 410,
};

class UIStaticText;
class UIBasicButton;
class UIEditCtrl;
class UIJobSlotButton;
class UIComboBoxDef2Wnd;
class UIPartyInfoWnd;
struct AdvenPartyBoardWndInfo;
struct ExtendedListLine;
struct PartyBoardPlacement {
  /* 0000 */ int x;
  /* 0004 */ int y;
  /* 0008 */ int width;
  /* 000C */ int height;
};

class UIAdvenPartyBoardWnd : public UIFrameWnd {
public:
  UIAdvenPartyBoardWnd();
  ~UIAdvenPartyBoardWnd() override;
  void StoreInfo() override;
  void OnCreate(int width, int height) override;
  void Paint() override;
  int SendMsg(UIWindow* sender, int message, std::intptr_t a, std::intptr_t b, std::intptr_t c, std::intptr_t d) override;
  void LoadPage(int page);
  void Registration(bool registered);
  void SetOwnParty(CAdvenPartyInfo value);

  /* 00B4 */ UIStaticText* pageText;
  /* 00B8 */ UIWindow* padding_00B8;
  /* 00BC */ UIBitmapButton* refresh;
  /* 00C0 */ UIEditCtrl* memo;
  /* 00C4 */ UIPartyInfoWnd* rows[10];
  /* 00EC */ UIPartyInfoWnd* ownParty;
  /* 00F0 */ UIBitmapButton* recruit;
  /* 00F4 */ UIBasicButton* stop;
  /* 00F8 */ UIBitmapButton* previous;
  /* 00FC */ UIBitmapButton* next;
  /* 0100 */ int padding_0100;
  /* 0104 */ int padding_0104;
  /* 0108 */ int padding_0108;
  /* 010C */ int page;
  /* 0110 */ int totalPages;
  /* 0114 */ unsigned char padding_0114;
  /* 0118 */ int padding_0118;
  /* 011C */ std::uint32_t padding_011C;
  /* 0120 */ AdvenPartyBoardWndInfo* saved;
  /* 0124 */ UIEditCtrl* minimum;
  /* 0128 */ UIEditCtrl* maximum;
  /* 012C */ UIJobSlotButton* jobs[4];
  /* 013C */ UIComboBoxDef2Wnd* types;
  /* 0140 */ std::vector<ExtendedListLine> padding_0140;
  /* 014C */ int selectedType;
  /* 0150 */ unsigned char searching;

private:
  void RefreshRows();
  void UpdatePagination();
  void ResetSearch();
  void Search(int trigger);
};

#if defined(_WIN32) && !defined(_WIN64)
static_assert(offsetof(PartyBoardPlacement, y) == 0x04);
static_assert(offsetof(PartyBoardPlacement, width) == 0x08);
static_assert(offsetof(PartyBoardPlacement, height) == 0x0C);
static_assert(sizeof(UIAdvenPartyBoardWnd) == 0x154);
#endif

#endif
