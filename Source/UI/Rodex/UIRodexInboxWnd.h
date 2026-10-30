#ifndef RAGNAROK_SOURCE_UI_RODEX_UIRODEXINBOXWND_H
#define RAGNAROK_SOURCE_UI_RODEX_UIRODEXINBOXWND_H

#include "UI/Item/UIItemWnd.h"

class UIRodexInboxWnd : public UIItemWnd {
public:
  bool Slot03() override;

  void DrawMailIcons(int row, unsigned char attachments, unsigned char read, unsigned char returned);
};

#endif
