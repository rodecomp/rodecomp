#ifndef RAGNAROK_SOURCE_UI_WINDOW_UICHANGENAMEWND_H
#define RAGNAROK_SOURCE_UI_WINDOW_UICHANGENAMEWND_H

#include "Client/Mode/CLoginMode.h"
#include "UI/Window/UIFrameWnd.h"

#include <cstdint>

class UIBitmapButton;
class UIBitmapEditCtrl;

class UIChangeNameWnd : public UIFrameWnd {
public:
  UIChangeNameWnd();
  void Slot15(int width, int height) override;
  void Paint() override;
  int SendMsg(UIWindow* sender, int message, std::intptr_t a, std::intptr_t b, std::intptr_t c, std::intptr_t d) override;
  int IsNameWithinLimit(std::string name, int limit);

protected:
  void* unknown_controls;

public:
  /* 00B4 */ UIBitmapButton* m_okButton;
  /* 00B8 */ UIBitmapButton* m_cancelButton;
  /* 00BC */ UIBitmapEditCtrl* m_edit;
  /* 00C0 */ CharacterInfo* m_character;
  /* 00C4 */ CharacterInfo m_info;
};

extern std::uint32_t g_renameGid;
extern char g_renameName[24];

#endif
