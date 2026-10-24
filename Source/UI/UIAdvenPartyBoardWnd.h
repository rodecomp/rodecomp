#ifndef RAGNAROK_SOURCE_UI_UIADVENPARTYBOARDWND_H
#define RAGNAROK_SOURCE_UI_UIADVENPARTYBOARDWND_H

#include "Contents/AdvenPartyInfo.h"
#include "UI/UIFrameWnd.h"
#include "UI/UIListBoxEx.h"

#include <vector>

class UIStaticText;
class UIBasicButton;
class UIEditCtrl;
class UIJobSlotButton;
class UIComboBoxDef2Wnd;
class UIPartyInfoWnd;
struct UIEditContext;
struct UIComboBoxHeaderContext;
struct PartyRowContext;
struct ExtendedListLine;
struct PartyBoardPlacement {
  int x;
  int y;
  int width;
  int height;
};

struct AgencyRequestContext;
struct StopRecruitingContext;
struct WindowOrdering;

struct UIAdvenPartyBoardContext {
  UIButtonContext& buttons;
  UIEditContext& edits;
  UIComboBoxHeaderContext& combos;
  PartyRowContext& rows;
  AgencyRequestContext& requests;
  StopRecruitingContext& stopCompletion;
  WindowOrdering& ordering;
  const int& playerLevel;
  const int& partyMember;
  const int& screenWidth;
  const int& screenHeight;
  const std::string& characterName;
};

// RTTI UIAdvenPartyBoardWnd, original x86 size 340.
class UIAdvenPartyBoardWnd : public UIFrameWnd {
public:
  UIAdvenPartyBoardWnd(UIWindowContext& windows, UIFrameContext& frames, UIAdvenPartyBoardContext& board);
  ~UIAdvenPartyBoardWnd() override;
  void Slot11() override;
  void Slot15(int width, int height) override;
  void Paint() override;
  int SendMsg(
    UIWindow* sender,
    int message,
    std::intptr_t a,
    std::intptr_t b,
    std::intptr_t c,
    std::intptr_t d
  ) override;
  void LoadPage(int page);
  void Registration(bool registered);
  void SetOwnParty(CAdvenPartyInfo value);

  /* 00B4 */ UIStaticText* pageText;
  /* 00B8 */ UIWindow* unknown_00B8;
  /* 00BC */ UIBitmapButton* refresh;
  /* 00C0 */ UIEditCtrl* memo;
  /* 00C4 */ UIPartyInfoWnd* rows[10];
  /* 00EC */ UIPartyInfoWnd* ownParty;
  /* 00F0 */ UIBitmapButton* recruit;
  /* 00F4 */ UIBasicButton* stop;
  /* 00F8 */ UIBitmapButton* previous;
  /* 00FC */ UIBitmapButton* next;
  /* 0100 */ int unknown_0100;
  /* 0104 */ int unknown_0104;
  /* 0108 */ int unknown_0108;
  /* 010C */ int page;
  /* 0110 */ int totalPages;
  /* 0114 */ unsigned char unknown_0114;
  /* 0118 */ int unknown_0118;
  /* 011C */ std::uint32_t unknown_011C;
  /* 0120 */ PartyBoardPlacement* saved;
  /* 0124 */ UIEditCtrl* minimum;
  /* 0128 */ UIEditCtrl* maximum;
  /* 012C */ UIJobSlotButton* jobs[4];
  /* 013C */ UIComboBoxDef2Wnd* types;
  /* 0140 */ std::vector<ExtendedListLine> unknown_0140;
  /* 014C */ int selectedType;
  /* 0150 */ unsigned char searching;

private:
  UIAdvenPartyBoardContext* m_board;
  void RefreshRows();
  void UpdatePagination();
  void ResetSearch();
  void Search(int trigger);
};

#if defined(_WIN32) && !defined(_WIN64)
static_assert(sizeof(UIAdvenPartyBoardWnd) == 352);
#endif

#endif
