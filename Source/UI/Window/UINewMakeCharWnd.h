#ifndef RAGNAROK_SOURCE_UI_WINDOW_UINEWMAKECHARWND_H
#define RAGNAROK_SOURCE_UI_WINDOW_UINEWMAKECHARWND_H

#include "UI/Buttons/UIBitmapButton.h"

class UIHairBitmap : public UIBitmapButton {
public:
  void Slot25(int x, int y) override;
  void Slot26(int x, int y) override;
  void Slot27(int x, int y) override;
  void Slot28(int x, int y) override;
  void Slot29(int x, int y) override;
  void ResetCursor(int x, int y) override;
  void Slot31(int x, int y) override;
};

#endif
