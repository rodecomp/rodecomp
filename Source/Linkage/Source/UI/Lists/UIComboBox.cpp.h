#pragma once

#include "UI/Lists/UIListBoxID.h"
#include "UI/Window/UIEnums.h"
#include "UI/Window/WindowId.h"
#include "UI/Lists/UIComboBox.h"
#include "UI/Lists/UIScrollBar.h"
#include "UI/Window/UIWindowMgr.h"
#include <algorithm>

extern template IdListLine& std::map<int, IdListLine>::operator[](const int&);
template __declspec(noinline) std::map<int, int>::iterator std::map<int, int>::_Mybase::find(const int&);
