#pragma once

#include "UI/Window/UIEnums.h"
#include "Client/Input/InputCharset.h"
#include "UI/Window/UIEnums.h"
#include "Client/Input/Language.h"
#include "Contents/Skin/SkinMgr.h"
#include "Render/Text/FontCache.h"
#include "Render/Text/TextContext.h"
#include "Resource/Bitmap/BitmapResource.h"
#include "Resource/ResourceMgr.h"
#include "Resource/Bitmap/SkinPalette.h"
#include "Resource/Bitmap/TextureNames.h"
#include "UI/Buttons/UIButton.h"
#include "UI/Buttons/UIItemTagButton.h"
#include "UI/Window/InputFocus.h"
#include "UI/Window/UIWindowMgr.h"
#include "UI/Chat/UINewChatWnd.h"
#include "UI/Edit/UIEditCtrl.h"
#include "UI/Lists/UIScrollBar.h"
#include "UI/Lists/UIScrollBarVariants.h"
#include "Network/TextEncoding.h"
#include "Utility/Text/FormatString.h"
#include "Utility/Text/StringUtil.h"
#include <cstring>
#include <windows.h>
#include <timeapi.h>

extern template void std::vector<unsigned int>::_Tidy() noexcept;
extern template std::size_t std::_Traits_find<std::char_traits<char>>(const char* const, const std::size_t, const std::size_t, const char* const, const std::size_t) noexcept;
