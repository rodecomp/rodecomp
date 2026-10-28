#ifndef RAGNAROK_SOURCE_UI_DIALOGS_UINOTICECONFIRMWND_H
#define RAGNAROK_SOURCE_UI_DIALOGS_UINOTICECONFIRMWND_H

#include "UI/Window/UIFrameWnd.h"

struct TextureNames;

class UINoticeConfirmWnd : public UIFrameWnd {
public:
  void Paint() override;

protected:
  TextureNames* m_textureNames;
};

#endif
