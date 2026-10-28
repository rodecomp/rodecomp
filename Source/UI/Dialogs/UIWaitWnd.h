#ifndef RAGNAROK_SOURCE_UI_DIALOGS_UIWAITWND_H
#define RAGNAROK_SOURCE_UI_DIALOGS_UIWAITWND_H

#include "UI/Window/UIFrameWnd.h"

struct TextureNames;

class UIWaitWnd : public UIFrameWnd {
public:
  void Paint() override;

protected:
  TextureNames* m_textureNames;
};

#endif
