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
  UIPetInfoWnd(UIWindowContext& windows, UIFrameContext& frames, UIButtonContext& controls);
  ~UIPetInfoWnd() override;

  void Slot11() override;
  void Paint() override;
  void UpdateFriendlyGauge();

  /* 00B4 */ UIEditCtrl* nameEdit;
  /* 00B8 */ PetInfoWndInfo* saved;
  /* 00BC */ int unknown_00BC;
  /* 00C0 */ int unknown_00C0;
  /* 00C4 */ UIWindow* window_00C4;
  /* 00C8 */ UIPetInfoBarGraph* friendlyGauge;
  /* 00CC */ std::string friendlyText;
  /* 00E4 */ unsigned char unknown_00E4;
  /* 00E8 */ int unknown_00E8;

protected:
  UIButtonContext* m_controls;
};

#endif
