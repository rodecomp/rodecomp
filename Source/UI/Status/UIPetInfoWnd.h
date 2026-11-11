#ifndef RAGNAROK_SOURCE_UI_STATUS_UIPETINFOWND_H
#define RAGNAROK_SOURCE_UI_STATUS_UIPETINFOWND_H

#include "UI/Window/UIFrameWnd.h"
#include "Contents/UserData/NormalItemSlots.h"

#include <string>

class UIBitmapButton;
class UICheckBox;
class UIEditCtrl;
class UINormalMB;
class UINoneVisibleButton;
class UIPetInfoBarGraph;

class UIPetInfoWnd : public UIFrameWnd {
public:
  UIPetInfoWnd();
  ~UIPetInfoWnd() override;

  void StoreInfo() override;
  void OnCreate(int width, int height) override;
  void Paint() override;
  int SendMsg(UIWindow* sender, int message, std::intptr_t a, std::intptr_t b, std::intptr_t c, std::intptr_t d) override;
  void UpdateFriendlyGauge();

  /* 00B4 */ UIEditCtrl* nameEdit;
  /* 00B8 */ PetInfoWndInfo* saved;
  /* 00BC */ UIBitmapButton* renameButton;
  /* 00C0 */ UINoneVisibleButton* statusButton;
  /* 00C4 */ UINormalMB* feedConfirmationWnd;
  /* 00C8 */ UIPetInfoBarGraph* friendlyGauge;
  /* 00CC */ std::string friendlyText;
  /* 00E4 */ unsigned char autoFeedAvailable;
  /* 00E8 */ UICheckBox* autoFeedCheck;

};

#endif
