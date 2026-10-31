#ifndef RAGNAROK_SOURCE_UI_LISTS_UITREEVIEW_H
#define RAGNAROK_SOURCE_UI_LISTS_UITREEVIEW_H

#include "UI/Window/UIWindow.h"

#include <list>
#include <string>
#include <vector>

class UIScrollBar;
class UIEditCtrl2;
struct UIEditContext;
class UITVBitmapButton;
class UITVTextButton;
struct UIButtonContext;

struct UITreeNode {
  /* 0000 */ int id;
  /* 0004 */ int expanded;
  /* 0008 */ int icon;
  /* 000C */ char name[0x100];
  /* 010C */ unsigned char unknown_010C[0x5E];
  /* 016A */ unsigned char draggable;
  /* 016B */ unsigned char container;
  /* 016C */ unsigned char unknown_016C[0x4];
  /* 0170 */ UITreeNode* unknown_0170;
  /* 0174 */ UITVBitmapButton* expandButton;
  /* 0178 */ UITVBitmapButton* collapseButton;
  /* 017C */ UITVTextButton* labelButton;
  /* 0180 */ std::list<UITreeNode*> children;
  /* 0188 */ std::string* icons;
  /* 018C */ unsigned char unknown_018C[0x8];
};

#if defined(_WIN32) && !defined(_WIN64)
static_assert(offsetof(UITreeNode, children) == 0x180);
#endif

class UITreeView : public UIWindow {
public:
  UITreeView(UIWindowContext& windows, UIEditContext& edit, UIButtonContext& controls);
  void Resize(int width, int height) override;
  void Slot15(int width, int height) override;
  void Paint() override;
  void Slot25(int x, int y) override;
  void Slot27(int x, int y) override;
  void Slot31(int x, int y) override;
  void Slot35(int value) override;
  int SendMsg(UIWindow* sender, int message, std::intptr_t a, std::intptr_t b, std::intptr_t c, std::intptr_t d) override;
  virtual void Slot53();

  void HideNodes(std::list<UITreeNode*>& nodes);
  bool DeleteChildNodes(UITreeNode* node);
  int RemoveChildNode(UITreeNode* parent, int id, bool keepButtons);
  void AddNodeButtons(UITreeNode* node, int id);
  void AddNodeLabel(UITreeNode* node, int id);
  int LayoutNodes(std::list<UITreeNode*>& nodes, int x, int y);

protected:
  /* 007C */ UIScrollBar* verticalBar;
  /* 0080 */ int verticalOverflow;
  /* 0084 */ UITreeNode* root;
  /* 0088 */ UITreeNode* unknown_0088;
  /* 008C */ UITreeNode* unknown_008C;
  /* 0090 */ UITreeNode* unknown_0090;
  /* 0094 */ UITreeNode* editNode;
  /* 0098 */ UIEditCtrl2* edit;
  /* 009C */ int itemPadding;
  /* 00A0 */ int itemHeight;
  /* 00A4 */ int itemCount;
  /* 00A8 */ int unknown_00A8;
  /* 00AC */ unsigned char unknown_00AC;

  UIEditContext* m_edit;
  UIButtonContext* m_controls;
  /* 00B0 */ int unknown_00B0;
};

#endif
