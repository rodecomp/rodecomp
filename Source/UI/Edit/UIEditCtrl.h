#ifndef RAGNAROK_SOURCE_UI_EDIT_UIEDITCTRL_H
#define RAGNAROK_SOURCE_UI_EDIT_UIEDITCTRL_H

#include "UI/Window/UIWindow.h"

#include <list>
#include <string>
#include <vector>

struct InputFocus;
struct CLanguage;
struct InputCharset;
struct UIButtonContext;
struct BitmapImage;
class WindowManagerWindows;
class NavigationDestination;
class UINaviLinkButton;
class UIItemTagButton;
struct ITEM_INFO;
class UIItemTagOnChat;

struct UIEditContext {
  InputFocus& focus;
  CLanguage& input;
  unsigned char& password;
  const InputCharset& charset;
  UIButtonContext& buttons;
  WindowManagerWindows& windows;
  NavigationDestination& navigation;
  const BitmapImage& colorChip;
  const int& candidateCursor;
  UIWindow* const& candidateWindow;
  UIWindow* const& readingWindow;
  UIWindow* const& otherChatInput;
  const bool& linkInputRestricted;
  UIWindow* const& permittedLinkInput;
  const int& screenWidth;
  const int& screenHeight;
};

class UIEditCtrl : public UIWindow {
  friend class UIMacroDetectorWnd;

public:
  UIEditCtrl();
  UIEditCtrl(const UIEditCtrl& other);
  UIEditCtrl(UIWindowContext& windows, UIEditContext& edit);
  ~UIEditCtrl() override;
  bool Slot05() override;
  void Paint() override;
  void Slot23() override;
  void Slot24(int retained) override;
  void Slot25(int x, int y) override;
  void Slot28(int x, int y) override;
  int SendMsg(UIWindow* sender, int message, std::intptr_t a, std::intptr_t b, std::intptr_t c, std::intptr_t d) override;
  int Slot46() override;
  virtual void SetText(const char* value);
  virtual void SelectAll();
  virtual void PaintText();
  virtual void Synchronize();
  virtual void Trim();
  int Length() const;

  const char* Text();
  std::string LinkedText();
  void Background(int red, int green, int blue);
  void Border(int red, int green, int blue);
  void TextColor(std::uint32_t primary, std::uint32_t secondary);
  void UpdateLinks();
  void SetCursor(std::size_t position);
  void MoveCursorToEnd();
  void DrawSelectionAt(int x, int y, int width);
  void DrawSelection(int x, int width);
  void MoveCandidateWindow(int x);
  void MoveReadingWindow(int x);
  bool DecodeItemLink(const std::vector<int>& values, ITEM_INFO& item);

  /* 007C */ int anchor;
  /* 0080 */ int cursor;
  /* 0084 */ int mask;
  /* 0088 */ int maximum;
  /* 008C */ int flat;
  /* 0090 */ int red;
  /* 0094 */ int green;
  /* 0098 */ int blue;
  /* 009C */ int borderRed;
  /* 00A0 */ int borderGreen;
  /* 00A4 */ int borderBlue;
  /* 00A8 */ std::uint32_t textColor;
  /* 00AC */ std::uint32_t secondColor;
  /* 00B0 */ int firstVisible;
  /* 00B4 */ int textY;
  /* 00B8 */ int invalidatesParent;
  /* 00BC */ std::string tooltip;
  /* 00D4 */ unsigned char disabled;
  /* 00D8 */ std::string text;
  /* 00F0 */ int unknown_00F0;
  /* 00F4 */ int topPadding;
  /* 00F8 */ unsigned char preserveInput;
  /* 00FC */ std::list<UIWindow*> links;
  /* 0104 */ std::string linkedText;

protected:
  UIEditContext* m_edit;
  void PaintTextWithPolicy(bool alternate, const std::string* placeholder, std::uint32_t placeholderColor, bool placeholderEnabled);
  bool RemoveInvalidLink(std::list<UIWindow*>::iterator& entry);
  void SnapLinkSelection(std::size_t prefix, std::size_t bytes);

private:
  void InsertLink(const char* value);
  std::string BracketFittedText(const std::string& name);
};

class UIEditCtrlWheel : public UIEditCtrl {
public:
  UIEditCtrlWheel(UIWindowContext& windows, UIEditContext& edit);
  ~UIEditCtrlWheel() override;
  void Slot35(int value) override;
};

class UIEditCtrl2 : public UIEditCtrl {
public:
  UIEditCtrl2(UIWindowContext& windows, UIEditContext& edit);
  ~UIEditCtrl2() override;
  int SendMsg(UIWindow* sender, int message, std::intptr_t a, std::intptr_t b, std::intptr_t c, std::intptr_t d) override;
};

struct UIEditCtrl3Table {
  /* 0000 */ std::vector<int> items;
  /* 000C */ int count = 0;
};

class UIEditCtrl3 : public UIEditCtrl2 {
public:
  UIEditCtrl3(UIWindowContext& windows, UIEditContext& edit);
  UIEditCtrl3(const UIEditCtrl3& other);
  void Slot15(int width, int height) override;
  void Resize(int width, int height) override;
  void Paint() override;
  int SendMsg(UIWindow* sender, int message, std::intptr_t a, std::intptr_t b, std::intptr_t c, std::intptr_t d) override;
  void SetText(const char* value) override;
  void Slot35(int value) override;
  void Trim() override;
  virtual void UpdateScrollBar();
  void HandleInputMessage(unsigned key, std::intptr_t data);

  void PlaceTags(const int& x, const int& y, const int& firstLine, const int& endLine, const int& line, const int& unused);
  void PlaceLinks(const std::string& value, int y, int line, int firstLine, int endLine);
  std::string TakePendingText();

  /* 011C */ std::list<std::vector<UIItemTagButton*>> entries;
  /* 0124 */ std::vector<std::string> pendingText;
  /* 0130 */ UIWindow* unknown_0130;
  /* 0134 */ unsigned char unknown_0134;
  /* 0138 */ int unknown_0138;
  /* 013C */ int unknown_013C;
  /* 0140 */ int unknown_0140;
  /* 0144 */ int unknown_0144;
  /* 0148 */ int unknown_0148;
  /* 014C */ int unknown_014C;
  /* 0150 */ int unknown_0150;
  /* 0154 */ int unknown_0154;
  /* 0158 */ int unknown_0158;
  /* 015C */ short unknown_015C;
  /* 015E */ unsigned char unknown_015E;
  /* 0160 */ char* unknown_0160;
  /* 0164 */ char* unknown_0164;
  /* 0168 */ std::vector<std::string> unknown_0168;
  /* 0174 */ UIEditCtrl3Table unknown_0174;
  /* 0184 */ short unknown_0184;
};

class CUIEditCtrl : public UIEditCtrl3 {
public:
  void Paint() override;
  void Invalidate() override;
};

class UIEditCtrl4 : public UIEditCtrl {
public:
  UIEditCtrl4(UIWindowContext& windows, UIEditContext& edit);
  ~UIEditCtrl4() override;
  void Slot23() override;
  void PaintText() override;
  void SetPlaceholderEnabled(bool enabled);
  void SetPlaceholderColor(std::uint32_t color);
  void SetPlaceholder(const char* value);

  /* 011C */ std::uint32_t placeholderColor;
  /* 0120 */ std::string placeholder;
  /* 0138 */ unsigned char placeholderEnabled;
};

class UIEditCtrl5 : public UIEditCtrl4 {
public:
  UIEditCtrl5(UIWindowContext& windows, UIEditContext& edit);
  ~UIEditCtrl5() override;
  int SendMsg(UIWindow* sender, int message, std::intptr_t a, std::intptr_t b, std::intptr_t c, std::intptr_t d) override;
};

class UIBitmapEditCtrl : public UIEditCtrl {
public:
  void Paint() override;
  void SetBitmapName(const char* name);

  /* 011C */ std::string bitmapName;
};

class UIChatEditCtrl : public UIEditCtrl {
public:
  UIChatEditCtrl(UIWindowContext& windows, UIEditContext& edit);
  void Slot25(int x, int y) override;
  int SendMsg(UIWindow* sender, int message, std::intptr_t a, std::intptr_t b, std::intptr_t c, std::intptr_t d) override;
  void InsertItemTag(int tagId);

  /* 011C */ std::vector<std::string> history;
  /* 0128 */ std::string pending;
  /* 0140 */ int unknown_0140;
  /* 0144 */ UIItemTagOnChat* unknown_0144;
  /* 0148 */ int historyLimit;
};

class UIEditCtrl_OldList : public UIEditCtrl {
public:
  void Paint() override;
  void Slot35(int value) override;
  int SendMsg(UIWindow* sender, int message, std::intptr_t argument1, std::intptr_t argument2, std::intptr_t argument3, std::intptr_t argument4) override;
  void RecallHistory(int step);

  /* 011C */ std::string bitmapName;
  /* 0134 */ std::vector<std::string> history;
  /* 0140 */ int historyIndex;
};

class UIWhisperEditCtrl : public UIEditCtrl {
public:
  void Slot23() override;
  int SendMsg(UIWindow* sender, int message, std::intptr_t a, std::intptr_t b, std::intptr_t c, std::intptr_t d) override;
  void SetText(const char* value) override;

  /* 011C */ std::vector<std::string> history;
  /* 0128 */ int historyIndex;
};

class UIEditCtrlVarId : public UIEditCtrl {
public:
  // INLINED
  UIEditCtrlVarId(UIWindowContext& windows, UIEditContext& edit) : UIEditCtrl(windows, edit) {}
  void Slot24(int retained) override;
  void Slot35(int value) override;
  int SendMsg(UIWindow* sender, int message, std::intptr_t argument1, std::intptr_t argument2, std::intptr_t argument3, std::intptr_t argument4) override;
  void SetText(const char* value) override;
};

bool HasItemLinkTag(const std::string& text);
bool HasNaviLinkTag(const std::string& text);

class UIShowAndEditTimeCtrl : public UIWindow {
public:
  void Paint() override;
  void Slot15(int width, int height) override;
  int SendMsg(UIWindow* sender, int message, std::intptr_t argument1, std::intptr_t argument2, std::intptr_t argument3, std::intptr_t argument4) override;

  /* 007C */ UIEditCtrlVarId* fields[2];

protected:
  UIEditContext* m_edit;
};

#if defined(_WIN32) && !defined(_WIN64)
static_assert(sizeof(UIEditCtrl) == 292);
static_assert(sizeof(UIEditCtrlWheel) == 292);
static_assert(sizeof(UIEditCtrl2) == 292);
static_assert(sizeof(UIEditCtrl4) == 324);
static_assert(sizeof(UIEditCtrl5) == 324);
#endif

extern bool g_inputSuggestionsEnabled;

bool IsInputSuggestionAllowed();

#endif
