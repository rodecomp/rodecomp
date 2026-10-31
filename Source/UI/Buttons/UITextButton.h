#ifndef RAGNAROK_SOURCE_UI_BUTTONS_UITEXTBUTTON_H
#define RAGNAROK_SOURCE_UI_BUTTONS_UITEXTBUTTON_H

#include "UI/Buttons/UIButton.h"

#include <cstddef>
#include <string>
#include <vector>

extern int g_screenHeight;
extern int g_screenWidth;

class UITextButton : public UIButton {
  friend class UITextViewer;
  friend class UIPetEvolutionWnd;
  friend class UIMessageBox;
  friend class UIEditCtrl3;
  friend class UIAchTracingWnd;
  friend class UIAchSummaryWnd;
  friend class CCreateLineInfo;
  friend class UISerachStoreInfoWnd;
  friend class UISeekPartyListWnd;

public:
  friend void OpenTextButtonLink(UITextButton* button);
  friend void ShowTextButtonTip(UITextButton* button);
  friend void ShowTextButtonItem(UITextButton* button);
  friend void RequestTextButtonService(UITextButton* button);

  UITextButton(UIWindowContext& windows, UIButtonContext& buttons);
  ~UITextButton() override;
  void Paint() override;
  void Slot27(int x, int y) override;
  void Slot28(int x, int y) override;
  void Slot31(int x, int y) override;
  void Slot32(int x, int y) override;
  void Slot33(int x, int y) override;
  void Slot35(int value) override;
  void Configure(const char* text, std::uint32_t color, int family, int height, unsigned char outline, unsigned char underline);
  void ResizeToText();
  void ResizeToTextClipped(int width, int height);
  void CenterIn(int width, int height);
  const std::string& GetLine(std::size_t index) const;
  int GetLinesCount() const;

protected:
  /* 00B4 */ std::uint32_t m_color;
  /* 00B8 */ std::uint32_t m_normalColor;
  /* 00BC */ std::uint32_t m_hoverColor;
  /* 00C0 */ std::uint32_t m_outlineColor;
  /* 00C4 */ unsigned char m_outline;
  /* 00C5 */ unsigned char m_underline;
  /* 00C8 */ int m_height;
  /* 00CC */ int m_family;
  /* 00D0 */ int m_value;
  /* 00D4 */ std::string m_text;
  /* 00EC */ std::vector<std::string> m_lines;
};

extern std::string g_emptyString;

#if defined(_WIN32) && !defined(_WIN64)
static_assert(sizeof(UITextButton) == 256);
#endif

#endif
