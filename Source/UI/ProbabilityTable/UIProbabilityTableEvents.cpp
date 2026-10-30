#include "UI/ProbabilityTable/UIProbabilityTable.h"

#include "UI/Edit/UIEditBoxEx.h"

#include <iterator>

// OFFSET: 0x00A4E3B0
// FUNCTION: fn_16206
template <>
bool CUIListBoxEx<ProbabilityTable::CUIItem, ProbabilityTable::CItemData>::RebindIndex(int index) {
  if (index != -1) {
    for (ProbabilityTable::CUIItem* item : this->items) {
      if (index == item->index) {
        if (!this->binder) {
          return false;
        }
        this->binder(item, *std::next(this->data.begin(), index));
        return true;
      }
    }
  }
  return false;
}

// OFFSET: 0x00A4E490
// FUNCTION: fn_16207
int CUIProbabilityTable::SendMsg(UIWindow* sender, int message, std::intptr_t argument1, std::intptr_t argument2, std::intptr_t argument3, std::intptr_t argument4) {
  if (message == 177) {
    this->m_searchEdit->SetFocus();
  }
  return UIFrameWnd::SendMsg(sender, message, argument1, argument2, argument3, argument4);
}
