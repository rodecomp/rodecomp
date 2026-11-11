#ifndef RAGNAROK_SOURCE_UI_TEXT_UITEXTVIEWER_H
#define RAGNAROK_SOURCE_UI_TEXT_UITEXTVIEWER_H

#include "UI/Lists/UIListBox.h"

#include <windows.h>

class UITextButton;

class UITextViewer : public UIListBox {
  friend class UIWindow;
  friend class UIQuestDisplay;
  friend class CGameMode;

public:
  UITextViewer();
  ~UITextViewer() override;
  void OnCreate(int width, int height) override;
  void Paint() override;
  void OnLBtnDown(int x, int y) override;
  void OnLBtnDblClk(int x, int y) override;
  void OnLBtnUp(int x, int y) override;
  void Configure(int x, int y, int width, int height, unsigned char transparent) override;
  void AddLine(const char* text) override;
  void Clear() override;
  int SendMsg(UIWindow* sender, int message, std::intptr_t argument1, std::intptr_t argument2, std::intptr_t argument3, std::intptr_t argument4) override;
  std::string ItemLinkNumberText(int value);
  // INLINED
  void SetBackgroundColor(std::uint32_t color) {
    this->backgroundColor = color;
    this->Invalidate();
  }

  /* 00D4 */ std::vector<UITextButton*> links;
  /* 00E0 */ std::uint32_t backgroundColor;
  /* 00E4 */ int padding_00E4;
  /* 00E8 */ unsigned char splitTipLinks;
  /* 00E9 */ unsigned char outline;
  /* 00EA */ unsigned char bold;

private:
  std::pair<int, int> LinkPosition(unsigned position, unsigned addedLines);
  void AddLinks(const char* text, unsigned addedLines, int kind);
  bool AddItemLinks(std::string text, unsigned addedLines);
  bool AddUrlLinks(std::string text, unsigned addedLines);
  bool AddNaviLinks(std::string text, unsigned addedLines);
  bool AddQuestLinks(std::string text, unsigned addedLines);
  void AddTipLinks(std::string text, unsigned addedLines);
  void SplitTextLinkArguments(std::string text, UITextButton* button);
  void AppendText(char* text, int width, int carriageReturns, int pixels);
  void Activate(UITextButton* button, int id);
};

class UITextViewEX : public UITextViewer {
public:
  void Configure(int x, int y, int width, int height, unsigned char transparent) override;
  void OnCreate(int width, int height) override;
  void Paint() override;
  void Clear() override;
  void Refresh() override;
  int SendMsg(UIWindow* sender, int message, std::intptr_t argument1, std::intptr_t argument2, std::intptr_t argument3, std::intptr_t argument4) override;
  void SetText(std::string text);
  void AddLineLinks(UITextButton* const* first, UITextButton* const* last);
  int AddWords(std::string text, int attached, int red, int green, int blue);

  std::vector<UIWindow*> extraLinks;
  std::vector<std::vector<UITextButton*>> lineLinks;
};

#if defined(_WIN32) && !defined(_WIN64)
static_assert(sizeof(UITextViewer) == 0xEC);
#endif

#endif
