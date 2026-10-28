#ifndef RAGNAROK_SOURCE_UI_DIALOGS_UIDISCONNECTEDSERVERMSGWND_H
#define RAGNAROK_SOURCE_UI_DIALOGS_UIDISCONNECTEDSERVERMSGWND_H

#include "UI/Window/UIFrameWnd.h"

struct TextureNames;

class UIDisconnectedServerMsgWnd : public UIFrameWnd {
public:
  void Paint() override;

protected:
  TextureNames* m_textureNames;
};

#endif
