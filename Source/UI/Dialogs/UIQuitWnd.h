#ifndef RAGNAROK_SOURCE_UI_DIALOGS_UIQUITWND_H
#define RAGNAROK_SOURCE_UI_DIALOGS_UIQUITWND_H

#include "UI/Window/UIFrameWnd.h"

struct TextureNames;

class UIQuitWnd : public UIFrameWnd {
public:
  void Paint() override;

protected:
  TextureNames* m_textureNames;
};

#endif
