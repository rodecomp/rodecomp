#ifndef RAGNAROK_SOURCE_UI_WINDOW_UICNTWND_H
#define RAGNAROK_SOURCE_UI_WINDOW_UICNTWND_H

#include "UI/Window/UIFrameWnd.h"

struct TextureNames;

class UICntWnd : public UIFrameWnd {
public:
  void Paint() override;
  void DrawBorder(int x, int y, int width, int height);

protected:
  TextureNames* m_textureNames;
};

#endif
