#ifndef RAGNAROK_SOURCE_UI_GUILD_UIAGITWND_H
#define RAGNAROK_SOURCE_UI_GUILD_UIAGITWND_H

#include "UI/Lists/UIListBox.h"
#include "UI/Window/UIFrameWnd.h"

#include <cstdint>

struct UIButtonContext;
struct WindowResponseRecorder;

struct UIAgitMessageBoxContext {
  UIButtonContext& buttons;
  WindowResponseRecorder& recorder;
};

class UIAgitMessageBox : public UIFrameWnd {
public:
  void Paint() override;
  int SendMsg(UIWindow* sender, int message, std::intptr_t a, std::intptr_t b, std::intptr_t c, std::intptr_t d) override;

protected:
  /* 00B4 */ std::uint16_t m_agitId;
  UIAgitMessageBoxContext* m_dialog;
};

class UIListBoxAgit : public UIListBox {
public:
  void Slot15(int width, int height) override;
  void Slot25(int x, int y) override;
  void Slot32(int x, int y) override;
};

class UIListBoxAgitMenu : public UIListBox {
public:
  void Paint() override;
  void Slot25(int x, int y) override;
  void Slot28(int x, int y) override;
};

#endif
