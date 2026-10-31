#ifndef RAGNAROK_SOURCE_UI_WINDOW_UIPETINFOWND_H
#define RAGNAROK_SOURCE_UI_WINDOW_UIPETINFOWND_H

#include "UI/Window/UIFrameWnd.h"
#include "Contents/UserData/NormalItemSlots.h"

#include <string>

class UIEditCtrl;
class UIPetInfoBarGraph;
struct UIButtonContext;

class UIPetInfoWnd : public UIFrameWnd {
public:
  void Slot11() override;
  void Paint() override;
  void UpdateFriendlyGauge();

  /* 00B4 */ UIEditCtrl* nameEdit;
  /* 00B8 */ PetInfoWndInfo* saved;
  /* 00BC */ unsigned char unknown_00BC[0x0C];
  /* 00C8 */ UIPetInfoBarGraph* friendlyGauge;
  /* 00CC */ std::string friendlyText;
  /* 00E4 */ unsigned char unknown_00E4;

protected:
  UIButtonContext* m_controls;
};

#endif
