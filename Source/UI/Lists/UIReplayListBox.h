#ifndef RAGNAROK_SOURCE_UI_LISTS_UIREPLAYLISTBOX_H
#define RAGNAROK_SOURCE_UI_LISTS_UIREPLAYLISTBOX_H

#include "UI/Lists/UIListBox.h"

class UIReplayListBox : public UIListBox {
public:
  // INLINED
  UIReplayListBox(UIWindowContext& windows, UIListBoxContext& list) : UIListBox(windows, list) {
  }

  void Slot25(int x, int y) override;
};

#endif
