#ifndef RAGNAROK_SOURCE_UI_LISTS_UITAGLISTBOX_H
#define RAGNAROK_SOURCE_UI_LISTS_UITAGLISTBOX_H

#include "UI/Lists/UIListBox.h"

#include <vector>

class UITagListBox : public UIListBox {
public:
  void AddLine(const char* text) override;
  void Clear() override;

protected:
  /* 00D4 */ std::vector<UIWindow*> m_tagWindows;
};

#endif
