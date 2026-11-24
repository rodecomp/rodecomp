#define UITREEVIEW_NODE_DESTRUCTOR
#include "UI/Lists/UITreeView.h"

#include <cstring>

// OFFSET: 0x0086F180
// FUNCTION: fn_11087
UITreeView::UITreeView() : UIWindow(0) {
  this->itemPadding = 5;
  this->itemHeight = 20;
  this->itemCount = 0;
  this->verticalBar = nullptr;
  this->verticalOverflow = 0;
  this->scrollRow = 0;
  this->selectedNode = nullptr;
  this->dragNode = nullptr;
  this->dropNode = nullptr;
  this->mouseInside = 0;
  this->highlightColor = 0;
}

// OFFSET: 0x00871F00
// FUNCTION: fn_45813
UITreeView::~UITreeView() {
  this->DeleteChildNodes(this->root);
  delete this->root;
}

// OFFSET: 0x00879D90
// FUNCTION: fn_45604
void UITreeView::ResetTree() {
  this->HideNodes(this->root->children);
  this->selectedNode = nullptr;
  this->dragNode = nullptr;
  this->dropNode = nullptr;
  this->editNode = nullptr;
  this->root = new UITreeNode;
  this->root->id = 0;
  this->root->expanded = 0;
  this->root->labelButton = nullptr;
  std::strcpy(this->root->name, "Root");
  this->UpdateScrollBar();
}

// OFFSET: 0x0087C6E0
// FUNCTION: fn_45605
void UITreeView::InitializeTree(int width, int height) {
  this->UIWindow::Resize(width, height);
  this->Move(width, height);
  this->OnCreate(width, height);
  this->OnSize(width, height);
  this->root = new UITreeNode;
  this->root->id = 0;
  this->root->expanded = 0;
  this->root->labelButton = nullptr;
  std::strcpy(this->root->name, "Root");
  this->UpdateScrollBar();
}
