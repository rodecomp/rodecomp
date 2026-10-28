#ifndef RAGNAROK_SOURCE_UI_DIALOGS_UIRESTARTWND_H
#define RAGNAROK_SOURCE_UI_DIALOGS_UIRESTARTWND_H

#include "UI/Window/UIFrameWnd.h"

struct TextureNames;

class UIRestartWnd : public UIFrameWnd {
public:
  void Paint() override;

protected:
  TextureNames* m_textureNames;
};

#endif
