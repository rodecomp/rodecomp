#ifndef RAGNAROK_SOURCE_UI_LISTS_UITREEVIEW_H
#define RAGNAROK_SOURCE_UI_LISTS_UITREEVIEW_H

#include "UI/Window/UIWindow.h"

#include <list>

class UIScrollBar;
class UIEditCtrl2;
struct UIEditContext;
class UITVBitmapButton;
class UITVTextButton;
struct UIButtonContext;

struct UITreeNode {
  /* 0000 */ unsigned char unknown_0000[0xC];
  /* 000C */ char name[0x100];
  /* 010C */ unsigned char unknown_010C[0x64];
  /* 0170 */ UITreeNode* unknown_0170;
  /* 0174 */ UITVBitmapButton* expandButton;
  /* 0178 */ UITVBitmapButton* collapseButton;
  /* 017C */ UITVTextButton* labelButton;
  /* 0180 */ std::list<UITreeNode*> children;
};

#if defined(_WIN32) && !defined(_WIN64)
static_assert(offsetof(UITreeNode, children) == 0x180);
#endif

class UITreeView : public UIWindow {
public:
  void Resize(int width, int height) override;
  void Slot15(int width, int height) override;
  void Slot25(int x, int y) override;
  void Slot27(int x, int y) override;
  void Slot31(int x, int y) override;
  void Slot35(int value) override;
  virtual void Slot53();

  void HideNodes(std::list<UITreeNode*>& nodes);
  void AddNodeButtons(UITreeNode* node, int id);
  void AddNodeLabel(UITreeNode* node, int id);

protected:
  /* 007C */ UIScrollBar* verticalBar;
  /* 0080 */ int verticalOverflow;
  /* 0084 */ unsigned char unknown_0084[0x4];
  /* 0088 */ int unknown_0088;
  /* 008C */ int unknown_008C;
  /* 0090 */ int unknown_0090;
  /* 0094 */ unsigned char unknown_0094[0x4];
  /* 0098 */ UIEditCtrl2* edit;
  /* 009C */ int itemPadding;
  /* 00A0 */ int itemHeight;
  /* 00A4 */ int itemCount;
  /* 00A8 */ int unknown_00A8;
  /* 00AC */ unsigned char unknown_00AC;

  UIEditContext* m_edit;
  UIButtonContext* m_controls;
};

#endif
