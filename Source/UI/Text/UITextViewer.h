#ifndef RAGNAROK_SOURCE_UI_TEXT_UITEXTVIEWER_H
#define RAGNAROK_SOURCE_UI_TEXT_UITEXTVIEWER_H

#include "UI/Lists/UIListBox.h"

#include <windows.h>

class UITextButton;
struct UIButtonContext;
struct ItemNaming;
struct TipBoxTitles;
struct ItemLinkDisplay;

class TextViewerActions {
public:
  virtual void Navigate(std::string map, int type, unsigned char flags, bool option, int x, int y, int target) = 0;
  virtual void NavigateNamed(std::string map, int type, int option) = 0;
  virtual void OpenTip(UIWindow& window, int id) = 0;
  virtual void OpenQuest(int id) = 0;

protected:
  ~TextViewerActions() = default;
};

struct UITextViewerContext {
  UIListBoxContext& list;
  UIButtonContext& buttons;
  ItemNaming& items;
  const TipBoxTitles& tips;
  TextViewerActions& actions;
  HWND& applicationWindow;

  const std::vector<ItemLinkDisplay*>* const& chatConversions;
};

class UITextViewer : public UIListBox {
  friend class UIWindow;

public:
  UITextViewer(UIWindowContext& windows, UITextViewerContext& text);
  ~UITextViewer() override;
  void Slot15(int width, int height) override;
  void Paint() override;
  void Slot25(int x, int y) override;
  void Slot26(int x, int y) override;
  void Slot31(int x, int y) override;
  void Configure(int x, int y, int width, int height, unsigned char transparent) override;
  void AddLine(const char* text) override;
  void Clear() override;
  int SendMsg(UIWindow* sender, int message, std::intptr_t argument1, std::intptr_t argument2, std::intptr_t argument3, std::intptr_t argument4) override;
  std::string ItemLinkNumberText(int value);

  /* 00D4 */ std::vector<UITextButton*> links;
  /* 00E0 */ std::uint32_t backgroundColor;
  /* 00E4 */ int unknown_00E4;
  /* 00E8 */ unsigned char splitTipLinks;
  /* 00E9 */ unsigned char outline;
  /* 00EA */ unsigned char bold;

private:
  UITextViewerContext* m_text;
  std::string ConvertChat(std::string text);
  std::pair<int, int> LinkPosition(unsigned position, unsigned addedLines);
  void AddLinks(const char* text, unsigned addedLines, int kind);
  bool AddItemLinks(std::string text, unsigned addedLines);
  bool AddUrlLinks(std::string text, unsigned addedLines);
  bool AddNaviLinks(std::string text, unsigned addedLines);
  bool AddQuestLinks(std::string text, unsigned addedLines);
  void AddTipLinks(std::string text, unsigned addedLines);
  void AppendText(char* text, int width, bool carriageReturns, bool pixels);
  void Activate(UITextButton* button, int id);
};

class UITextViewEX : public UITextViewer {
public:
  void Configure(int x, int y, int width, int height, unsigned char transparent) override;
  void Slot15(int width, int height) override;
  void Clear() override;
  void Refresh() override;

  std::vector<UIWindow*> extraLinks;
  std::vector<std::vector<UITextButton*>> lineLinks;
};

#if defined(_WIN32) && !defined(_WIN64)
static_assert(sizeof(UITextViewer) == 248);
#endif

#endif
