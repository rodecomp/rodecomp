#ifndef RAGNAROK_SOURCE_UI_WINDOW_UISEEKPARTYLISTWND_H
#define RAGNAROK_SOURCE_UI_WINDOW_UISEEKPARTYLISTWND_H

#include "Contents/Party/PartyBookingInfo.h"
#include "UI/Window/UIFrameWnd.h"

class UICheckBox;

class UISeekPartyListWnd : public UIFrameWnd {
public:
  void NotifyNewAd();
  bool MatchesFilter(PARTY_BOOKING_AD_INFO info);

protected:
  /* 00B4 */ unsigned char unknown_00B4[0x3C];
  /* 00F0 */ UICheckBox* m_alarmCheck;
  /* 00F4 */ unsigned char unknown_00F4[0xD0];
  /* 01C4 */ short m_filterLevel;
  /* 01C6 */ short m_filterMap;
  /* 01C8 */ short m_filterJob;
};

#endif
