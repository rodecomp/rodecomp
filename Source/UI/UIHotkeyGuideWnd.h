#ifndef RAGNAROK_SOURCE_UI_UIHOTKEYGUIDEWND_H
#define RAGNAROK_SOURCE_UI_UIHOTKEYGUIDEWND_H

#include "UI/Window/UIFrameWnd.h"

#include <map>
#include <string>
#include <vector>

struct UIButtonContext;

struct HotkeyGuideEntry {
  /* 0000 */ int id;
  /* 0004 */ int firstKey;
  /* 0008 */ int secondKey;

  HotkeyGuideEntry(int id, int firstKey, int secondKey) : id(id), firstKey(firstKey), secondKey(secondKey) {
  }
};

struct HotkeyGuideKey {
  /* 0000 */ bool valid;
  /* 0004 */ int x;
  /* 0008 */ int y;
  /* 000C */ unsigned char align;
  /* 000D */ unsigned char width;
  /* 000E */ unsigned char padding;
  /* 000F */ unsigned char height;

  HotkeyGuideKey();
  HotkeyGuideKey(int x, int y, unsigned char align, unsigned char width, unsigned char padding, unsigned char height);
};

struct HotkeyGuideLabel {
  /* 0000 */ std::string text;
  /* 0018 */ HotkeyGuideKey key;
  /* 0028 */ HotkeyGuideKey altKey;

  // INLINED
  HotkeyGuideLabel() {
  }
  HotkeyGuideLabel(std::string text, HotkeyGuideKey key, HotkeyGuideKey altKey);
  ~HotkeyGuideLabel();
};

class UIHotkeyGuideWnd : public UIFrameWnd {
public:
  UIHotkeyGuideWnd(UIWindowContext& windows, UIFrameContext& frames, UIButtonContext& controls);
  void Slot15(int width, int height) override;
  void Slot19() override;
  void Paint() override;

  void DrawKey(const HotkeyGuideKey* key);
  void DrawKeyLabel(std::string text, const HotkeyGuideKey* key);

protected:
  /* 00B4 */ std::map<int, HotkeyGuideLabel> m_labels;
  /* 00BC */ std::vector<HotkeyGuideEntry> m_entries;
  /* 00C8 */ std::vector<UIWindow*> m_tabs;
  /* 00D4 */ int m_current;
  /* 00D8 */ std::vector<int> m_keys;
  UIButtonContext* m_controls;
};

#if defined(_WIN32) && !defined(_WIN64)
static_assert(sizeof(HotkeyGuideKey) == 0x10);
static_assert(sizeof(HotkeyGuideLabel) == 0x38);
#endif

#endif
