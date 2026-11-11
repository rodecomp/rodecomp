#ifndef RAGNAROK_SOURCE_UI_REPLAY_UIREPLAYLISTBOX_H
#define RAGNAROK_SOURCE_UI_REPLAY_UIREPLAYLISTBOX_H

#include "UI/Lists/UIListBox.h"

class UIReplayListBox : public UIListBox {
public:
  // INLINED
  UIReplayListBox() : UIListBox() {
  }

  void OnLBtnDown(int x, int y) override;
};

#endif
