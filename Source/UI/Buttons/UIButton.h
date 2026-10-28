#ifndef RAGNAROK_SOURCE_UI_BUTTONS_UIBUTTON_H
#define RAGNAROK_SOURCE_UI_BUTTONS_UIBUTTON_H

#include "UI/Window/UIWindow.h"

#include <string>

class ResourceMgr;
class LuaScript;
class FontCache;
struct TextLocale;

class ButtonServices {
public:
  virtual void Tooltip(const char* text, int x, int y, int color, bool option, bool convert) = 0;
  virtual const char* MessageText(int id) = 0;
  virtual int Confirm(const char* text, int type, int option1, int option2, const char* caption, int width, int height) = 0;
  virtual void Submit(UIWindow* window) = 0;

protected:
  ~ButtonServices() = default;
};

struct UIButtonContext {
  ResourceMgr& resources;
  LuaScript& behavior;
  FontCache& fonts;
  const TextLocale& locale;
  ButtonServices& services;
  const int* policy;
  const bool& restricted;
  const int* activeModeType;
  UIWindow* const& hoverParent;
};

class UIButton : public UIWindow {
  friend class UIAdvenPartyBoardWnd;

public:
  UIButton(UIWindowContext& windowContext, UIButtonContext& buttonContext);
  ~UIButton() override;
  void Paint() override;
  void Slot25(int x, int y) override;
  void Slot26(int x, int y) override;
  void Slot27(int x, int y) override;
  void Slot28(int x, int y) override;
  void Slot29(int x, int y) override;
  void ResetCursor(int x, int y) override;
  void Slot31(int x, int y) override;
  virtual void Enable(unsigned char value);

  void SetTooltip(const char* text);

  // INLINED
  void SetDisabled(unsigned char value) {
    this->m_disabled = value;
  }
  // INLINED
  unsigned char Disabled() const {
    return this->m_disabled;
  }
  std::string label;

protected:
  /* 0094 */ std::string m_tooltip;
  /* 00AC */ unsigned char m_disabled;
  /* 00AD */ unsigned char m_drawLabel;
  /* 00B0 */ int m_argument;
  UIButtonContext* m_buttonContext;
};

#if defined(_WIN32) && !defined(_WIN64)
static_assert(sizeof(UIButton) == 188);
#endif

#endif
