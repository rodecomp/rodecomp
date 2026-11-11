#ifndef RAGNAROK_SOURCE_UI_ITEMS_UIPREVIEWEQUIPWND_H
#define RAGNAROK_SOURCE_UI_ITEMS_UIPREVIEWEQUIPWND_H

#include "UI/Window/UIFrameWnd.h"

class CCharacterRender;
class UIBasicButton;
class UIBitmapButton;
struct PreviewEquipWndInfo;

class UIPreviewEquipWnd : public UIFrameWnd {
public:
  UIPreviewEquipWnd();
  ~UIPreviewEquipWnd() override;
  void StoreInfo() override;
  void Paint() override;
  void OnCreate(int width, int height) override;
  void OnProcess() override;
  int SendMsg(UIWindow* sender, int message, std::intptr_t a, std::intptr_t b, std::intptr_t c, std::intptr_t d) override;
  void DrawCharacter();
  void RefreshHeadgearLocations();
  void SyncFromPlayer();

  /* 00B4 */ PreviewEquipWndInfo* saved;
  /* 00B8 */ CCharacterRender* m_render;
  /* 00BC */ UIBitmapButton* m_leftButton;
  /* 00C0 */ UIBitmapButton* m_rightButton;
  /* 00C4 */ UIBasicButton* m_actionButton;
};

#endif
