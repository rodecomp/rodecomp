#ifndef RAGNAROK_SOURCE_UI_ACHIEVEMENT_UIACHWND_H
#define RAGNAROK_SOURCE_UI_ACHIEVEMENT_UIACHWND_H

#include "Contents/Achievement/AchievementMgr.h"
#include "Contents/Jobs/JobInheritance.h"
#include "Script/LuaScript.h"
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
class UITextButton;

class UIAchScrollBar : public UIScrollBar {
public:
  UIAchScrollBar();

  void Paint() override;
};

class UICustomGageBar : public UIWindow {
public:
  __declspec(noinline) UICustomGageBar();

  /* 007C */ int value;
  /* 0080 */ int maximum;
  /* 0084 */ std::string text;
  /* 009C */ std::uint32_t barColor;
  /* 00A0 */ std::uint32_t backgroundColor;
  /* 00A4 */ std::uint32_t outerBorderColor;
  /* 00A8 */ std::uint32_t innerBorderColor;

  void Paint() override;
  void OnMouseHover(int x, int y) override;
};

class UIAchListWnd : public UIWindow {
public:
  UIAchListWnd();
  ~UIAchListWnd() override;

  /* 007C */ UIAchScrollBar* scrollBar;
  /* 0080 */ UIButton** buttons;
  /* 0084 */ int rowHeight;
  /* 0088 */ int rowCount;
  /* 008C */ int firstRow;
  /* 0090 */ int selectedIndex;
  /* 0094 */ int scrollBarWidth;
  /* 0098 */ unsigned char scrollable;
  /* 009C */ int listState;
  /* 00A0 */ std::vector<int> ids;
  /* 00AC */ std::map<int, AchievementInfo>* achievementList;

  void Create(int width, int height, int rows);
  void SelectAchievement(int index);
  __declspec(noinline) void SetAchievements(std::map<int, AchievementInfo>* achievements, int mode);
  __declspec(noinline) void UpdateScroll();

  void Paint() override;
  void OnLBtnDown(int x, int y) override;
  void OnLBtnUp(int x, int y) override;
  void OnWheel(int delta) override;
  int SendMsg(UIWindow* sender, int message, std::intptr_t argument1, std::intptr_t argument2, std::intptr_t argument3, std::intptr_t argument4) override;
};

struct UIAchTabGroup {
  /* 0000 */ std::string name;
  /* 0018 */ std::map<int, std::string> subs;
};

class UIAchTab : public UIWindow {
public:
  // INLINED
  UIAchTab()
      : UIWindow(0), selectedGroup(-1), selectedSub(-1), fontHeight(0), hoverGroup(0), hoverSub(0) {
    this->scrollBar = nullptr;
    this->scrollable = 0;
    this->scrollBarX = 13;
    this->visibleSubs = 3;
    this->fontHeight = 12;
    this->scrollPos = 0;
  }

  /* 007C */ int selectedGroup;
  /* 0080 */ int selectedSub;
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
  /* 00AC */ unsigned char tabState;
  /* 00B0 */ std::map<int, UIAchTabGroup> groups;
  /* 00B8 */ UIAchScrollBar* scrollBar;
  /* 00BC */ int scrollPos;
  /* 00C0 */ int scrollBarX;
  /* 00C4 */ int visibleSubs;
  /* 00C8 */ unsigned char scrollable;

  // INLINED
  void Select(int group, int sub) {
    this->selectedGroup = group;
    this->selectedSub = sub;
    this->Invalidate();
  }

  // INLINED
  int SubCount(int group) {
    return static_cast<int>(this->groups[group].subs.size());
  }

  void ClearGroups();
  void DrawGroup(int group);
  __declspec(noinline) std::string Label(int group, int sub);
  __declspec(noinline) bool HitTestTab(int x, int y, int* a, int* b);
  __declspec(noinline) void UpdateScrollBar();
  void Refresh();
  void SetLabel(const char* text, int group, int sub);

  void Paint() override;
  void OnLBtnDown(int x, int y) override;
  void OnMouseMove(int x, int y) override;
  void OnLBtnUp(int x, int y) override;
  void OnWheel(int delta) override;
  int SendMsg(UIWindow* sender, int message, std::intptr_t argument1, std::intptr_t argument2, std::intptr_t argument3, std::intptr_t argument4) override;
};

class UIAchTV : public UITextViewer {
public:
  // INLINED
  UIAchTV() : selectionDisabled(0), textColor(0), outlineColor(0), underline(0) {
  }

  /* 00EC */ unsigned char selectionDisabled;
  /* 00F0 */ std::uint32_t textColor;
  /* 00F4 */ std::uint32_t outlineColor;
  /* 00F8 */ unsigned char underline;

  void Paint() override;
  void OnLBtnDown(int x, int y) override;
  void Clear() override;
};

class UIToolTipBitmapWnd : public UIWindow {
public:
  UIToolTipBitmapWnd(WindowManagerWindows& managed);

  /* 007C */ std::string tooltip;
  /* 0094 */ CBitmapRes* bitmap;
  /* 0098 */ std::string bitmapName;
  /* 00B0 */ int itemId;
  /* 00B4 */ int iconX;
  /* 00B8 */ int iconY;

  void Paint() override;
  void OnMouseHover(int x, int y) override;
  void OnRBtnDown(int x, int y) override;

  void Set(const char* name, const char* text, int item);

};

class UIAchViewWnd : public UIWindow {
public:
  UIAchViewWnd();

  /* 007C */ UIBitmapButton* receiveButton;
  /* 0080 */ UIAchTV* description;
  /* 0084 */ UIAchTV* conditions[10];
  /* 00AC */ UICustomGageBar* gauges[10];
  /* 00D4 */ UIToolTipBitmapWnd* rewards[3];
  /* 00E0 */ int achievementId;
  /* 00E4 */ AchievementInfo* info;

  __declspec(noinline) void UpdateInfo();

  // INLINED
  void SetAchievement(int id, AchievementInfo* achievement) {
    this->achievementId = id;
    this->info = achievement;
    this->UpdateInfo();
  }

  void OnCreate(int width, int height) override;
  void Paint() override;
  int SendMsg(UIWindow* sender, int message, std::intptr_t argument1, std::intptr_t argument2, std::intptr_t argument3, std::intptr_t argument4) override;
};

class UIAchUpperWnd : public UIWindow {
public:
  UIAchUpperWnd();

  /* 007C */ std::string title;
  /* 0094 */ int achievementPoints;
  /* 0098 */ int nextLevelPoints;
  /* 009C */ int completedCount;
  /* 00A0 */ int achievementCount;
  /* 00A4 */ UICustomGageBar* levelGauge;
  /* 00A8 */ UICustomGageBar* completionGauge;
  /* 00AC */ UIWindow* buttons[2];
  /* 00B4 */ int selected;

  // INLINED
  void SetTitle(const std::string& text) {
    this->title = text;
  }

  void OnCreate(int width, int height) override;
  void Paint() override;
  int SendMsg(UIWindow* sender, int message, std::intptr_t argument1, std::intptr_t argument2, std::intptr_t argument3, std::intptr_t argument4) override;
};

class UIAchSummaryWnd : public UIWindow {
public:
  UIAchSummaryWnd();

  /* 007C */ int visibleCount;
  /* 0080 */ std::vector<UICustomGageBar*> gauges;
  /* 008C */ std::vector<std::pair<int, AchievementInfo>> recent;
  /* 0098 */ UITextButton* buttons[2];

  __declspec(noinline) void UpdateInfo();

  void Show(int visible) override;
  void OnCreate(int width, int height) override;
  void Paint() override;
  int SendMsg(UIWindow* sender, int message, std::intptr_t argument1, std::intptr_t argument2, std::intptr_t argument3, std::intptr_t argument4) override;
};

class UIAchievementWnd : public UIFrameWnd {
public:
  UIAchievementWnd();
  ~UIAchievementWnd() override;

  /* 00B4 */ UIAchUpperWnd* upper;
  /* 00B8 */ UIAchTab* tab;
  /* 00BC */ UIAchListWnd* list;
  /* 00C0 */ UIAchViewWnd* view;
  /* 00C4 */ UIAchSummaryWnd* summary;
  /* 00C8 */ int group;
  /* 00CC */ int sub;
  /* 00D0 */ std::map<int, AchievementInfo> achievements;

  void OnCreate(int width, int height) override;
  void Paint() override;
  int SendMsg(UIWindow* sender, int message, std::intptr_t argument1, std::intptr_t argument2, std::intptr_t argument3, std::intptr_t argument4) override;

  void CreateTab();
  void SelectAchievement(int id);
  __declspec(noinline) void SelectTab(int group, int sub);
};

struct AchTracingCondition {
  /* 0000 */ std::string text;
  /* 0018 */ unsigned char completed;
};

struct AchTracingEntry {
  /* 0000 */ unsigned char expanded;
  /* 0001 */ unsigned char pinned;
  /* 0004 */ int id;
  /* 0008 */ std::string name;
  /* 0020 */ std::vector<AchTracingCondition> conditions;
};

struct AchTracingWndInfo;

class UIAchTracingWnd : public UIFrameWnd {
public:
  /* 00B4 */ AchTracingWndInfo* info;
  /* 00B8 */ UITextButton* toggleButtons[3];
  /* 00C4 */ UITextButton* openButtons[3];

  UIAchTracingWnd();
  bool IsTransmitMouseInput() override;
  void StoreInfo() override;
  void OnCreate(int width, int height) override;
  void Paint() override;
  int SendMsg(UIWindow* sender, int message, std::intptr_t argument1, std::intptr_t argument2, std::intptr_t argument3, std::intptr_t argument4) override;
};

extern std::vector<AchTracingEntry> g_achievementTracking;

#endif
