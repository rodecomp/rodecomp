#ifndef RAGNAROK_SOURCE_UI_GAMESETTINGS_GAMESETTINGSUI_H
#define RAGNAROK_SOURCE_UI_GAMESETTINGS_GAMESETTINGSUI_H

#include "UI/Lists/UIListBoxTemplate.h"
#include "UI/Window/UIControl.h"
#include "UI/Window/UIFrameWnd.h"

namespace GameSettingsUI {
class CUIGroup : public CUIControl {
public:
  virtual void Apply();
  virtual void Refresh() = 0;
};

class CUIGroupEmblem : public CUIGroup {
public:
  void Apply() override;
};

class CUIGroupLogInOut : public CUIGroup {
public:
  void Apply() override;
};

class CUIGroupRodexSpam : public CUIGroup {
public:
  void Apply() override;
};

class CUIGroupSkin : public CUIGroup {
public:
  void Apply() override;
};

class CUIGroupSound : public CUIGroup {
public:
  void Paint() override;
};

class CUIGroupSpriteDetailLevel : public CUIGroup {
public:
  void Paint() override;
};

class CUIGroupTextureDetailLevel : public CUIGroup {
public:
  void Paint() override;
};

class CUIPage : public CUIControl {
public:
  void Slot16() override;
  void Paint() override;
  virtual void Apply();
  virtual void Refresh();
};

class CUIListPage : public CUIControl {
public:
  void Slot16() override;
  void Paint() override;
};

class CUIComboItem : public CUIListItem {
public:
  void Slot16() override;
};

class CUIOptionItem : public CUIListItem {
public:
  void Slot16() override;
  void Paint() override;
};
}

class CUIGameSettingsUI : public UIFrameWnd {
public:
  void Slot25(int x, int y) override;

  /* 00B8 */ unsigned char unknown_00B8[0x1C];
  /* 00D8 */ CUIControl* m_titleBar;
};

#endif
