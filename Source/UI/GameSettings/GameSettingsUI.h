#ifndef RAGNAROK_SOURCE_UI_GAMESETTINGS_GAMESETTINGSUI_H
#define RAGNAROK_SOURCE_UI_GAMESETTINGS_GAMESETTINGSUI_H

#include "UI/Lists/UIListBoxTemplate.h"
#include "UI/Window/UIControl.h"

namespace GameSettingsUI {
class CUIGroup : public CUIControl {
public:
  virtual void Apply();
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
  void Paint() override;
  virtual void Apply();
};

class CUIListPage : public CUIControl {
public:
  void Paint() override;
};

class CUIComboItem : public CUIListItem {
public:
  void Slot16() override;
};

class CUIOptionItem : public CUIListItem {
public:
  void Paint() override;
};
}

#endif
