#ifndef RAGNAROK_SOURCE_UI_WINDOW_UISEEKPARTYLISTWND_H
#define RAGNAROK_SOURCE_UI_WINDOW_UISEEKPARTYLISTWND_H

#include "Contents/Party/PartyBookingInfo.h"
#include "Contents/Party/PartyBookingMgr.h"
#include "UI/Window/UIFrameWnd.h"

#include <map>
#include <string>
#include <vector>

class UIBitmapButton;
class UICheckBox;
class UIComboBoxDefWnd;
class UIEditCtrl;
class UIScrollBar;
class UIStaticText;
class UITextButton;
struct SeekPartyListWndInfo;

extern std::map<int, PartyBookingEntry> g_friendRows;

struct SeekPartyMapEntry {
  /* 0000 */ std::string name;
  /* 0018 */ unsigned int color;
};

class UISeekPartyListWnd : public UIFrameWnd {
public:
  void Slot11() override;
  void Slot15(int width, int height) override;
  void Paint() override;
  void Slot25(int x, int y) override;
  void Slot26(int x, int y) override;
  void Slot35(int delta) override;
  int SendMsg(UIWindow* sender, int message, std::intptr_t argument1, std::intptr_t argument2, std::intptr_t argument3, std::intptr_t argument4) override;
  void NotifyNewAd();
  void UpdateScrollBar();
  bool MatchesFilter(PARTY_BOOKING_AD_INFO info);
  void InitRegionFilter();
  void InitJobFilter();
  void SelectRegion(int region);
  void SelectJobCategory(int category);

protected:
  int HitTestRow(int x, int y);

  /* 00B4 */ UIStaticText* m_title;
  /* 00B8 */ UIStaticText* m_levelLabel;
  /* 00BC */ UIStaticText* m_mapLabel;
  /* 00C0 */ UIStaticText* m_jobLabel;
  /* 00C4 */ UIStaticText* m_filterLabel0;
  /* 00C8 */ UIStaticText* m_filterLabel1;
  /* 00CC */ UIStaticText* m_filterLabel2;
  /* 00D0 */ UIStaticText* m_filterLabel3;
  /* 00DC */ UIEditCtrl* m_searchEdit;
  /* 00E0 */ UIComboBoxDefWnd* m_regionCombo;
  /* 00E4 */ UIComboBoxDefWnd* m_mapCombo;
  /* 00E8 */ UIComboBoxDefWnd* m_jobCategoryCombo;
  /* 00EC */ UIComboBoxDefWnd* m_jobCombo;
  /* 00F0 */ UICheckBox* m_alarmCheck;
  /* 00F4 */ UIBitmapButton* m_findButton;
  /* 00F8 */ UIBitmapButton* m_okButton;
  /* 00FC */ UIBitmapButton* m_chatButton;
  /* 0100 */ UITextButton* m_refreshButton;
  /* 0104 */ UIBitmapButton* m_tabButton;
  /* 0108 */ UIScrollBar* m_scrollBar;
  /* 010C */ unsigned char unknown_010C[0x4];
  /* 0110 */ std::map<int, std::vector<SeekPartyMapEntry>> m_regionMaps;
  /* 0118 */ std::vector<std::string> m_jobOptions[9];
  /* 0184 */ int m_region;
  /* 0188 */ int m_mapIndex;
  /* 018C */ int m_jobCategory;
  /* 0190 */ int m_jobCategoryChanged;
  /* 0194 */ unsigned char unknown_0194[0x10];
  /* 01A4 */ int m_firstRow;
  /* 01A8 */ int m_hoverIndex;
  /* 01AC */ int m_columnCount;
  /* 01B0 */ int m_visibleRows;
  /* 01B4 */ int unknown_01B4;
  /* 01B8 */ int m_scrollable;
  /* 01BC */ int m_restoreHeight;
  /* 01C0 */ unsigned char m_minimized;
  /* 01C1 */ unsigned char unknown_01C1[0x3];
  /* 01C4 */ short m_filterLevel;
  /* 01C6 */ short m_filterMap;
  /* 01C8 */ short m_filterJob;
  /* 01CA */ unsigned char unknown_01CA[6];
  /* 01D0 */ SeekPartyListWndInfo* m_saved;
};

#endif
