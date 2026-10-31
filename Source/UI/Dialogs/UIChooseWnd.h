#ifndef RAGNAROK_SOURCE_UI_DIALOGS_UICHOOSEWND_H
#define RAGNAROK_SOURCE_UI_DIALOGS_UICHOOSEWND_H

#include "UI/Window/UIFrameWnd.h"

#include <vector>

struct WindowPlacement;
struct UIButtonContext;
struct UIListBoxContext;
class UIBitmapButton;
class UIListBox;

class UIChooseWnd : public UIFrameWnd {
public:
  UIChooseWnd(UIWindowContext& windows, UIFrameContext& frames, UIButtonContext& controls, UIListBoxContext& lists);
  ~UIChooseWnd() override;
  void Slot11() override;
  void Slot15(int width, int height) override;
  void Paint() override;

  /* 00B4 */ int unknown_00B4;
  /* 00B8 */ std::vector<int> m_items;
  /* 00C4 */ UIListBox* m_list;
  /* 00C8 */ unsigned char unknown_00C8[0x4];
  /* 00CC */ WindowPlacement* m_placement;
  /* 00D0 */ UIBitmapButton* m_okButton;
  /* 00D4 */ UIBitmapButton* m_cancelButton;

protected:
  UIButtonContext* m_controls;
  UIListBoxContext* m_listContext;
};

#endif
