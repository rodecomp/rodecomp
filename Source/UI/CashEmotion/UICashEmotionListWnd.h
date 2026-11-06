#ifndef RAGNAROK_SOURCE_UI_CASHEMOTION_UICASHEMOTIONLISTWND_H
#define RAGNAROK_SOURCE_UI_CASHEMOTION_UICASHEMOTIONLISTWND_H

#include "Contents/UserData/NormalItemSlots.h"
#include "UI/Window/UIFrameWnd.h"

#include <cstdint>
#include <map>

class Timestamp;
class UIBitmapButton;
class UICashEmotionListChildWnd_Abstract;
class UICashEmotionListChildWnd_Basic;
class UICashEmotionListChildWnd_Purchase;
class UICashEmotionListChildWnd_Search;
class UICashEmotionListChildWnd_UnPurchased;
class UIEditCtrl;
class UIStaticText;

class UICashEmotionListWnd : public UIFrameWnd {
public:
  UICashEmotionListWnd();
  void Slot11() override;
  void Slot15(int width, int height) override;
  void Slot19() override;
  void Paint() override;
  int SendMsg(UIWindow* sender, int message, std::intptr_t argument1, std::intptr_t argument2, std::intptr_t argument3, std::intptr_t argument4) override;

  UIBitmapButton* CreateButton(int x, int y, int width, int height, int id, const char* format, bool hasDisabled);
  void RefreshCurrentList();
  __declspec(noinline) void ShowPack(int packId);
  void RefreshPackLists();
  __declspec(noinline) void ResetLists();
  int SetPeriod(int packId);
  void SetTab(char tab);
  void StartExpirySchedule();
  __declspec(noinline) void UpdateExpiry(int packId, Timestamp* expiry);
  __declspec(noinline) void RefreshSelection();
  void RestartExpirySchedule();

protected:
  /* 00B4 */ CashEmotionListWndInfo* m_placement;
  /* 00B8 */ UICashEmotionListChildWnd_Basic* m_basicList;
  /* 00BC */ UICashEmotionListChildWnd_Purchase* m_purchaseList;
  /* 00C0 */ UICashEmotionListChildWnd_UnPurchased* m_unpurchasedList;
  /* 00C4 */ UICashEmotionListChildWnd_Search* m_searchList;
  /* 00C8 */ std::map<char, UIWindow*> m_tabs;
  /* 00D0 */ char m_tab;
  /* 00D4 */ UIBitmapButton* unknown_00D4;
  /* 00D8 */ UIEditCtrl* m_searchEdit;
  /* 00DC */ UIBitmapButton* unknown_00DC;
  /* 00E0 */ UIBitmapButton* unknown_00E0;
  /* 00E4 */ UIBitmapButton* unknown_00E4;
  /* 00E8 */ UICashEmotionListChildWnd_Abstract* m_currentList;
  /* 00EC */ UIStaticText* m_period;
};

#if defined(_WIN32) && !defined(_WIN64)
static_assert(sizeof(UICashEmotionListWnd) == 0xF0);
#endif

#endif
