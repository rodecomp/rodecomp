#pragma once

#include "UI/Window/UIWindowMgr.h"
#include "UI/Window/WindowId.h"
#include "UI/Window/UIEnums.h"
#include "Render/DeviceContext/DeviceContext.h"
#include "Render/Text/TextContext.h"
#include "Resource/Bitmap/BitmapResource.h"
#include "Resource/ResourceMgr.h"
#include "Resource/Bitmap/SkinPalette.h"
#include "Contents/Skin/SkinMgr.h"
#include "Contents/Items/ItemLink.h"
#include "UI/Text/ColoredText.h"
#include "UI/Lists/UIChatHisBox.h"
#include "UI/Lists/UIListBox.h"
#include "UI/Lists/UIListBoxID.h"
#include "UI/Lists/UIMemberListBox.h"
#include "Render/Text/FontCache.h"
#include "UI/Lists/UIScrollBar.h"
#include "UI/Navigation/NavigationLink.h"
#include "UI/Text/PixelTextWrapping.h"
#include "UI/Text/TextTags.h"
#include "UI/Window/WindowManagerWindows.h"
#include <algorithm>
#include <cstdlib>
#include <cstring>
#include <mbstring.h>

template __declspec(noinline) std::pair<std::map<int, IdListLine>::iterator, bool> std::_Tree<std::_Tmap_traits<int, IdListLine, std::less<int>, std::allocator<std::pair<const int, IdListLine>>, false>>::insert(std::pair<const int, IdListLine>&&);
template __declspec(noinline) std::_Tree_find_result<std::map<int, int>::_Mybase::_Nodeptr> std::map<int, int>::_Mybase::_Find_lower_bound<int>(const int&) const;
