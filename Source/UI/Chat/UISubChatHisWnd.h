#ifndef RAGNAROK_SOURCE_UI_CHAT_UISUBCHATHISWND_H
#define RAGNAROK_SOURCE_UI_CHAT_UISUBCHATHISWND_H

#include "UI/Window/UIFrameWnd.h"

class UIEditCtrl;

class UISubChatHisWnd : public UIFrameWnd {
public:
  void Slot25(int x, int y) override;
  void Slot26(int x, int y) override;
  void Slot31(int x, int y) override;

private:
  /* 00B4 */ unsigned char unknown_00B4[0x34];
  /* 00E8 */ int m_inputHeight;
  /* 00EC */ int m_inputWidth;
  /* 00F0 */ int unknown_00F0;
  /* 00F4 */ int m_index;
  /* 00F8 */ unsigned char unknown_00F8[0x34];
  /* 012C */ UIEditCtrl* m_edit;
};

#endif
