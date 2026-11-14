#ifndef RAGNAROK_SOURCE_UI_DIALOGS_UINPCEDITDIALOGWND_H
#define RAGNAROK_SOURCE_UI_DIALOGS_UINPCEDITDIALOGWND_H

#include "UI/Window/UIFrameWnd.h"

#include <string>

class UIEditCtrl;
class UIItemTagOnMegaPhone;
struct ITEM_INFO;

class UINpcEditDialogWnd : public UIFrameWnd {
public:
  UINpcEditDialogWnd();
  void Paint() override;
  void OnCreate(int width, int height) override;
  int SendMsg(UIWindow* sender, int message, std::intptr_t argument1, std::intptr_t argument2, std::intptr_t argument3, std::intptr_t argument4) override;

protected:

public:
  /* 00B4 */ UIEditCtrl* edit;
  /* 00B8 */ int npcId;
};

class UINpcTextEditDialogWnd : public UINpcEditDialogWnd {
public:
  UINpcTextEditDialogWnd();
  void OnCreate(int width, int height) override;
  void InsertItemTag(const ITEM_INFO& item);
  void InsertSkillTag(int tagId);
  int MeasureText(const std::string& text, const unsigned& bytes);
  void Paint() override;
  int SendMsg(UIWindow* sender, int message, std::intptr_t argument1, std::intptr_t argument2, std::intptr_t argument3, std::intptr_t argument4) override;

  /* 00BC */ UIItemTagOnMegaPhone* m_tagCtrl;
};

#endif
