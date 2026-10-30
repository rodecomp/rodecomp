#ifndef RAGNAROK_SOURCE_UI_TEXT_UIACHWND_H
#define RAGNAROK_SOURCE_UI_TEXT_UIACHWND_H

#include "Contents/AchievementMgr.h"
#include "UI/Buttons/UIButton.h"
#include "UI/Lists/UIScrollBar.h"
#include "UI/Text/UITextViewer.h"
#include "UI/Window/UIFrameWnd.h"
#include "UI/Window/UIWindow.h"
#include "UI/Window/WindowManagerWindows.h"

#include <cstdint>
#include <map>
#include <string>
#include <utility>
#include <vector>

class CBitmapRes;
class UIBitmapButton;

struct UIAchListInfo {
  /* 0000 */ int unknown_0000;
  /* 0004 */ int count;
};

class UIAchScrollBar : public UIScrollBar {
public:
  // INLINED
  explicit UIAchScrollBar(UIWindowContext& windows) : UIScrollBar(windows) {
    this->arrowLength = 11;
    this->thickness = 11;
  }

  void Paint() override;
};

class UICustomGageBar : public UIWindow {
public:
  /* 007C */ int value;
  /* 0080 */ int maximum;
  /* 0084 */ std::string text;
  /* 009C */ std::uint32_t barColor;
  /* 00A0 */ std::uint32_t backgroundColor;
  /* 00A4 */ std::uint32_t outerBorderColor;
  /* 00A8 */ std::uint32_t innerBorderColor;

  void Paint() override;
};

class UIAchListWnd : public UIWindow {
public:
  // INLINED
  UIAchListWnd(UIWindowContext& windows, UIButtonContext& controls)
      : UIWindow(windows, 0), buttons(nullptr), unknown_0084(0), rowCount(0), unknown_008C(0), selectedIndex(0), unknown_0094(11), scrollable(0),
        unknown_009C(0), unknown_00AC(nullptr), scrollBar(nullptr), m_controls(&controls) {
  }
  ~UIAchListWnd() override;

  /* 0080 */ UIButton** buttons;
  /* 0084 */ int unknown_0084;
  /* 0088 */ int rowCount;
  /* 008C */ int unknown_008C;
  /* 0090 */ int selectedIndex;
  /* 0094 */ int unknown_0094;
  /* 0098 */ unsigned char scrollable;
  /* 009C */ int unknown_009C;
  /* 00A0 */ std::vector<int> ids;
  /* 00AC */ UIAchListInfo* unknown_00AC;
  /* 007C */ UIAchScrollBar* scrollBar;

  void Create(int width, int height, int rows);
  void sub_7B37F0(int index);
  void SetAchievements(std::map<int, AchievementInfo>* achievements, int mode);
  void UpdateScroll();

  void Slot25(int x, int y) override;
  void Slot31(int x, int y) override;
  void Slot35(int delta) override;

private:
  UIButtonContext* m_controls;
};

struct UIAchTabGroup {
  /* 0000 */ std::string name;
  /* 0018 */ std::map<int, std::string> subs;
};

class UIAchTab : public UIWindow {
public:
  // INLINED
  UIAchTab(UIWindowContext& windows, UIButtonContext& controls)
      : UIWindow(windows, 0), unknown_007C(-1), unknown_0080(-1), fontHeight(0), hoverGroup(0), hoverSub(0), scrollBar(nullptr), scrollable(0),
        scrollBarX(13), visibleSubs(3), m_controls(&controls) {
    this->fontHeight = 12;
    this->scrollPos = 0;
  }

  /* 007C */ int unknown_007C;
  /* 0080 */ int unknown_0080;
  /* 0084 */ int tabWidth;
  /* 0088 */ int groupHeight;
  /* 008C */ int subHeight;
  /* 0090 */ int fontHeight;
  /* 0094 */ int hoverGroup;
  /* 0098 */ int hoverSub;
  /* 009C */ std::uint32_t selectedGroupColor;
  /* 00A0 */ std::uint32_t groupColor;
  /* 00A4 */ std::uint32_t selectedSubColor;
  /* 00A8 */ std::uint32_t subColor;
  /* 00AC */ unsigned char unknown_00AC;
  /* 00B0 */ std::map<int, UIAchTabGroup> groups;
  /* 00B8 */ UIAchScrollBar* scrollBar;
  /* 00BC */ int scrollPos;
  /* 00C0 */ int scrollBarX;
  /* 00C4 */ int visibleSubs;
  /* 00C8 */ unsigned char scrollable;

  void ClearGroups();
  void DrawGroup(int group);
  std::string Label(int group, int sub);
  bool sub_7B4370(int x, int y, int* a, int* b);
  void sub_7B7F10();
  void Refresh();
  void SetLabel(const char* text, int group, int sub);

  void Paint() override;
  void Slot25(int x, int y) override;
  void Slot27(int x, int y) override;
  void Slot31(int x, int y) override;
  void Slot35(int delta) override;
  int SendMsg(UIWindow* sender, int message, std::intptr_t argument1, std::intptr_t argument2, std::intptr_t argument3, std::intptr_t argument4) override;

private:
  UIButtonContext* m_controls;
};

class UIAchTV : public UITextViewer {
public:
  /* 00EC */ unsigned char unknown_00EC;
  /* 00F0 */ std::uint32_t textColor;
  /* 00F4 */ std::uint32_t outlineColor;
  /* 00F8 */ unsigned char underline;

  void Paint() override;
  void Slot25(int x, int y) override;
  void Clear() override;
};

class UIToolTipBitmapWnd : public UIWindow {
public:
  /* 007C */ std::string tooltip;
  /* 0094 */ CBitmapRes* bitmap;
  /* 0098 */ std::string bitmapName;
  /* 00B0 */ int itemId;
  /* 00B4 */ int unknown_00B4;
  /* 00B8 */ int unknown_00B8;

  void Paint() override;
  void Slot32(int x, int y) override;

  void Set(const char* name, const char* text, int item);

private:
  WindowManagerWindows* m_windows;
};

class UIAchViewWnd : public UIWindow {
public:
  // INLINED
  UIAchViewWnd(UIWindowContext& windows, UIButtonContext& controls) : UIWindow(windows, 0), achievementId(-1), info(nullptr), m_controls(&controls) {
    this->receiveButton = nullptr;
    this->description = nullptr;
    for (int index = 0; index < 10; ++index) {
      this->conditions[index] = nullptr;
      this->gauges[index] = nullptr;
    }
    for (UIToolTipBitmapWnd*& reward : this->rewards) {
      reward = nullptr;
    }
  }

  /* 007C */ UIBitmapButton* receiveButton;
  /* 0080 */ UIAchTV* description;
  /* 0084 */ UIAchTV* conditions[10];
  /* 00AC */ UICustomGageBar* gauges[10];
  /* 00D4 */ UIToolTipBitmapWnd* rewards[3];
  /* 00E0 */ int achievementId;
  /* 00E4 */ AchievementInfo* info;

  void Paint() override;
  int SendMsg(UIWindow* sender, int message, std::intptr_t argument1, std::intptr_t argument2, std::intptr_t argument3, std::intptr_t argument4) override;

private:
  UIButtonContext* m_controls;
};

class UIAchUpperWnd : public UIWindow {
public:
  // INLINED
  UIAchUpperWnd(UIWindowContext& windows, UIButtonContext& controls)
      : UIWindow(windows, 0), unknown_0094(0), unknown_0098(0), unknown_009C(0), unknown_00A0(0), unknown_00A4(nullptr), unknown_00A8(nullptr), buttons{},
        selected(0), m_controls(&controls) {
  }

  /* 007C */ std::string title;
  /* 0094 */ int unknown_0094;
  /* 0098 */ int unknown_0098;
  /* 009C */ int unknown_009C;
  /* 00A0 */ int unknown_00A0;
  /* 00A4 */ UICustomGageBar* unknown_00A4;
  /* 00A8 */ UICustomGageBar* unknown_00A8;
  /* 00AC */ UIWindow* buttons[2];
  /* 00B4 */ int selected;

  void Paint() override;
  int SendMsg(UIWindow* sender, int message, std::intptr_t argument1, std::intptr_t argument2, std::intptr_t argument3, std::intptr_t argument4) override;

private:
  UIButtonContext* m_controls;
};

class UIAchSummaryWnd : public UIWindow {
public:
  // INLINED
  UIAchSummaryWnd(UIWindowContext& windows, UIButtonContext& controls) : UIWindow(windows, 0), unknown_007C(7), m_controls(&controls) {
  }

  /* 007C */ int unknown_007C;
  /* 0080 */ std::vector<UICustomGageBar*> gauges;
  /* 008C */ std::vector<std::pair<int, AchievementInfo>> recent;
  /* 0098 */ UIWindow* buttons[2];

  void Show(int visible) override;
  void Paint() override;
  int SendMsg(UIWindow* sender, int message, std::intptr_t argument1, std::intptr_t argument2, std::intptr_t argument3, std::intptr_t argument4) override;

private:
  UIButtonContext* m_controls;
};

class UIAchievementWnd : public UIFrameWnd {
public:
  UIAchievementWnd(UIWindowContext& windows, UIFrameContext& frame, UIButtonContext& controls);

  /* 00B4 */ UIAchUpperWnd* upper;
  /* 00B8 */ UIAchTab* tab;
  /* 00BC */ UIAchListWnd* list;
  /* 00C0 */ UIAchViewWnd* view;
  /* 00C4 */ UIAchSummaryWnd* summary;
  /* 00C8 */ int group;
  /* 00CC */ int sub;
  /* 00D0 */ std::map<int, AchievementInfo> achievements;

  void Slot15(int width, int height) override;
  void Paint() override;

  void CreateTab();
  void SelectAchievement(int id);
  void SelectTab(int group, int sub);

private:
  UIButtonContext* m_controls;
};

#endif
