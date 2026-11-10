#ifndef RAGNAROK_SOURCE_UI_WINDOW_UIREPLAYRECCONTROLWND_H
#define RAGNAROK_SOURCE_UI_WINDOW_UIREPLAYRECCONTROLWND_H

#include "Contents/UserData/NormalItemSlots.h"
#include "UI/Window/UIFrameWnd.h"

#include <cstdint>

class UIBitmapButton;
class UIButton;
class UICheckBox;
class UIEditCtrl;
class UIStaticBitmap;
class UIStaticText;

class UIReplayRECControlWnd : public UIFrameWnd {
public:
  UIReplayRECControlWnd(int mode);
  ~UIReplayRECControlWnd() override;

  bool NeedsFrameRefresh() override;
  void StoreInfo() override;
  void OnCreate(int width, int height) override;
  void Paint() override;
  int SendMsg(UIWindow* sender, int message, std::intptr_t a, std::intptr_t b, std::intptr_t c, std::intptr_t d) override;
  void ApplyMode();
  void UpdateRecordTime();
  void AnimateLoading();

  /* 00B4 */ int mode;
  /* 00B8 */ int padding_00B8;
  union {
    struct {
      /* 00BC */ UIWindow* recordStartButton;
      /* 00C0 */ UIWindow* recordStopButton;
      /* 00C4 */ UIWindow* confirmFilenameButton;
      /* 00C8 */ UIButton* button_00C8;
      /* 00CC */ UIWindow* closeOptionsButton;
      /* 00D0 */ UIButton* button_00D0;
    };
    /* 00BC */ UIBitmapButton* buttons[6];
  };
  /* 00D4 */ UIWindow* tabs[5];
  union {
    struct {
      /* 00E8 */ UIStaticText* recordTimeText;
      /* 00EC */ UIWindow* optionsTitleText;
    };
    /* 00E8 */ UIStaticText* texts[2];
  };
  /* 00F0 */ std::uint32_t recordStart;
  /* 00F4 */ UIWindow* options[10];
  /* 011C */ unsigned char recordOptions[10];
  /* 0126 */ unsigned char padding_0126[2];
  union {
    struct {
      /* 0128 */ UIWindow* userFilenameCheckBox;
      /* 012C */ UIWindow* rejectExistingNameCheckBox;
    };
    /* 0128 */ UICheckBox* fileOptions[2];
  };
  /* 0130 */ bool replayPathEnabled;
  /* 0131 */ bool replayOption119;
  /* 0132 */ unsigned char padding_0132[2];
  /* 0134 */ UIEditCtrl* filenameEdit;
  /* 0138 */ ReplayRECControlWndInfo* saved;
};

#endif
