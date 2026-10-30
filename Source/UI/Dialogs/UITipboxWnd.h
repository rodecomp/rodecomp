#ifndef RAGNAROK_SOURCE_UI_DIALOGS_UITIPBOXWND_H
#define RAGNAROK_SOURCE_UI_DIALOGS_UITIPBOXWND_H

#include "UI/Window/UIFrameWnd.h"

#include <string>

class UITipboxWnd : public UIFrameWnd {
public:
  void ShowTip(int id);
  void TrimAt(std::string& text, const char* marker);
};

#endif
