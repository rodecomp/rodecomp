#ifndef RAGNAROK_SOURCE_UI_LISTS_UITREEVIEW_H
#define RAGNAROK_SOURCE_UI_LISTS_UITREEVIEW_H

#include "UI/Window/UIWindow.h"

#include <list>
#include <string>
#include <vector>

class UIScrollBar;
class UIEditCtrl2;
class UITVBitmapButton;
class UITVTextButton;

struct UITreeNode {
  /* 0000 */ int id;
  /* 0004 */ int expanded;
  /* 0008 */ int icon;
  /* 000C */ char name[0x100];
  /* 010C */ unsigned char padding_010C[0x5E];
  /* 016A */ unsigned char draggable;
  /* 016B */ unsigned char container;
  /* 016C */ unsigned char padding_016C[0x4];
  /* 0170 */ UITreeNode* parent;
  /* 0174 */ UITVBitmapButton* expandButton;
  /* 0178 */ UITVBitmapButton* collapseButton;
  /* 017C */ UITVTextButton* labelButton;
  /* 0180 */ std::list<UITreeNode*> children;
  /* 0188 */ std::vector<std::string> icons;
};

#if defined(_WIN32) && !defined(_WIN64)
static_assert(offsetof(UITreeNode, children) == 0x180);
#endif

class UITreeView : public UIWindow {
public:
  UITreeView();
  void Resize(int width, int height) override;
  void OnCreate(int width, int height) override;
  void Paint() override;
  void OnLBtnDown(int x, int y) override;
  void OnMouseMove(int x, int y) override;
  void OnLBtnUp(int x, int y) override;
  void OnWheel(int value) override;
  int SendMsg(UIWindow* sender, int message, std::intptr_t a, std::intptr_t b, std::intptr_t c, std::intptr_t d) override;
  virtual void UpdateScrollBar();

  void HideNodes(std::list<UITreeNode*>& nodes);
  bool DeleteChildNodes(UITreeNode* node);
  bool RemoveChildNode(UITreeNode* parent, int id, bool keepButtons);
  void AddNodeButtons(UITreeNode* node, int id);
  void AddNodeLabel(UITreeNode* node, int id);
  int LayoutNodes(std::list<UITreeNode*>& nodes, int x, int y);

protected:
  /* 007C */ UIScrollBar* verticalBar;
  /* 0080 */ int verticalOverflow;
  /* 0084 */ UITreeNode* root;
  /* 0088 */ UITreeNode* selectedNode;
  /* 008C */ UITreeNode* dragNode;
  /* 0090 */ UITreeNode* dropNode;
  /* 0094 */ UITreeNode* editNode;
  /* 0098 */ UIEditCtrl2* edit;
  /* 009C */ int itemPadding;
  /* 00A0 */ int itemHeight;
  /* 00A4 */ int itemCount;
  /* 00A8 */ int scrollRow;
  /* 00AC */ unsigned char mouseInside;

  /* 00B0 */ int highlightColor;
};

#endif
