#ifndef RAGNAROK_SOURCE_UI_WINDOW_UISEEKPARTYLISTWND_H
#define RAGNAROK_SOURCE_UI_WINDOW_UISEEKPARTYLISTWND_H

#include "Contents/Party/PartyBookingInfo.h"
#include "UI/Window/UIFrameWnd.h"

class UICheckBox;
class UIScrollBar;

class UISeekPartyListWnd : public UIFrameWnd {
public:
  void Slot35(int delta) override;
  void NotifyNewAd();
  bool MatchesFilter(PARTY_BOOKING_AD_INFO info);

protected:
  /* 00B4 */ unsigned char unknown_00B4[0x3C];
  /* 00F0 */ UICheckBox* m_alarmCheck;
  /* 00F4 */ unsigned char unknown_00F4[0x14];
  /* 0108 */ UIScrollBar* m_scrollBar;
  /* 010C */ unsigned char unknown_010C[0xA4];
  /* 01B0 */ int m_visibleRows;
  /* 01B4 */ int unknown_01B4;
  /* 01B8 */ int m_scrollable;
  /* 01BC */ unsigned char unknown_01BC[0x8];
  /* 01C4 */ short m_filterLevel;
  /* 01C6 */ short m_filterMap;
  /* 01C8 */ short m_filterJob;
};

#endif
