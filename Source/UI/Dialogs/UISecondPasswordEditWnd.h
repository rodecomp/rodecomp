#ifndef RAGNAROK_SOURCE_UI_DIALOGS_UISECONDPASSWORDEDITWND_H
#define RAGNAROK_SOURCE_UI_DIALOGS_UISECONDPASSWORDEDITWND_H

#include "UI/Dialogs/UIPasswordWnd.h"

#include <cstdint>

class UISecondPasswordEditWnd : public UISecondPasswordInputWnd {
public:
  int SendMsg(UIWindow* sender, int message, std::intptr_t a, std::intptr_t b, std::intptr_t c, std::intptr_t d) override;
};

#endif
