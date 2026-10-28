#ifndef RAGNAROK_SOURCE_UI_WINDOW_UICHATROOMWND_H
#define RAGNAROK_SOURCE_UI_WINDOW_UICHATROOMWND_H

#include "UI/Window/UIFrameWnd.h"

struct UIButtonContext;
struct TextureNames;

class UIChatRoomWnd : public UIFrameWnd {
public:
  void Paint() override;

protected:
  UIButtonContext* m_controls;
  TextureNames* m_textureNames;
};

#endif
