#ifndef RAGNAROK_SOURCE_UI_WINDOW_UIPETINFOWND_H
#define RAGNAROK_SOURCE_UI_WINDOW_UIPETINFOWND_H

#include "UI/Window/UIFrameWnd.h"
#include "Contents/UserData/NormalItemSlots.h"

#include <string>

class UIBitmapButton;
class UICheckBox;
class UIEditCtrl;
class UINormalMB;
struct ForbiddenWords;
class UINoneVisibleButton;
class UIPetInfoBarGraph;
struct UIButtonContext;
struct UIEditContext;
struct WindowResponseRecorder;

class UIPetInfoWnd : public UIFrameWnd {
public:
  UIPetInfoWnd(UIWindowContext& windows, UIFrameContext& frames, UIButtonContext& controls);
  ~UIPetInfoWnd() override;

  void Slot11() override;
  void Slot15(int width, int height) override;
  void Paint() override;
  int SendMsg(UIWindow* sender, int message, std::intptr_t a, std::intptr_t b, std::intptr_t c, std::intptr_t d) override;
  void UpdateFriendlyGauge();

  /* 00B4 */ UIEditCtrl* nameEdit;
  /* 00B8 */ PetInfoWndInfo* saved;
  /* 00BC */ UIBitmapButton* unknown_00BC;
  /* 00C0 */ UINoneVisibleButton* unknown_00C0;
  /* 00C4 */ UINormalMB* window_00C4;
  /* 00C8 */ UIPetInfoBarGraph* friendlyGauge;
  /* 00CC */ std::string friendlyText;
  /* 00E4 */ unsigned char unknown_00E4;
  /* 00E8 */ UICheckBox* unknown_00E8;

protected:
  UIButtonContext* m_controls;
  UIEditContext* m_editContext;
  WindowResponseRecorder* m_recorder;
  ForbiddenWords* m_forbiddenWords;
};

#endif
