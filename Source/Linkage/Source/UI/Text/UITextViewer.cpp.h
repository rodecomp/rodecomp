#pragma once

#include <cstddef>
#include <string>
#include "Render/DeviceContext/DeviceContext.h"
#include "Contents/Skin/SkinMgr.h"
#include "UI/Window/UIEnums.h"
#include "Render/Text/TextContext.h"
#include "Resource/Bitmap/BitmapResource.h"
#include "Resource/ResourceMgr.h"
#include "Resource/Bitmap/SkinPalette.h"
#include "UI/Text/ColoredText.h"
#include "UI/Lists/UIChatHisBox.h"
#include "UI/Text/TextMarkup.h"
#include "Contents/Tables/TipBoxTitles.h"
#include "Network/TextEncoding.h"
#include "UI/Buttons/UIItemTagButton.h"
#include "UI/Edit/UIItemTagCtrl.h"
#include "Utility/Text/FormatString.h"
#include "UI/Lists/UIScrollBar.h"
#include "UI/Buttons/UITextButton.h"
#include "UI/Text/UITextViewer.h"
#include "UI/Text/UIString.h"
#include "UI/Achievement/UIAchWnd.h"
#include "UI/Window/UIWindowMgr.h"
#include "UI/Dialogs/UINpcEditDialogWnd.h"
#include "UI/Rodex/UIRodexComposeWnd.h"
#include <algorithm>
#include <new>

extern template std::vector<UIString*>::~vector();
template __declspec(noinline) std::string::basic_string(std::_String_constructor_concat_tag, const std::string&, const char*, std::size_t, const char*, std::size_t);
template __declspec(noinline) std::string::basic_string(const std::string&);
template __declspec(noinline) void std::list<ItemTagRecord>::push_back(const ItemTagRecord&);
