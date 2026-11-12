#include "UI/Window/UIRegisterMgr.h"

template __declspec(noinline) std::_Tree_find_result<std::map<int, std::function<CUIFrameWnd*()>>::_Mybase::_Nodeptr>
std::map<int, std::function<CUIFrameWnd*()>>::_Mybase::_Find_lower_bound<int>(const int&) const;

// OFFSET: 0x00A970A0
// FUNCTION: fn_17199
bool CUIRegisterMgr::IsRegistered(int key) {
  return this->entries.end() != this->entries.find(key);
}
