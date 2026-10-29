#ifndef RAGNAROK_SOURCE_UI_UIHOTKEYGUIDEWND_H
#define RAGNAROK_SOURCE_UI_UIHOTKEYGUIDEWND_H

#include "UI/Window/UIFrameWnd.h"

#include <string>
#include <vector>

struct UIButtonContext;

struct HotkeyGuideEntry {
  /* 0000 */ int id;
  /* 0004 */ int firstKey;
  /* 0008 */ int secondKey;
};

struct HotkeyGuideKey {
  /* 0000 */ int id;
  /* 0004 */ int x;
  /* 0008 */ int y;
  /* 000C */ unsigned char align;
  /* 000D */ unsigned char width;
  /* 000E */ unsigned char padding;
  /* 000F */ unsigned char height;
};

class UIHotkeyGuideWnd : public UIFrameWnd {
public:
  void Slot19() override;

  void DrawKey(const HotkeyGuideKey* key);
  void DrawKeyLabel(std::string text, const HotkeyGuideKey* key);

protected:
  /* 00B4 */ int unknown_00B4;
  /* 00B8 */ int unknown_00B8;
  /* 00BC */ std::vector<HotkeyGuideEntry> m_entries;
  /* 00C8 */ std::vector<UIWindow*> m_tabs;
  /* 00D4 */ int m_current;
  /* 00D8 */ std::vector<int> m_keys;
  UIButtonContext* m_controls;
};

#endif
