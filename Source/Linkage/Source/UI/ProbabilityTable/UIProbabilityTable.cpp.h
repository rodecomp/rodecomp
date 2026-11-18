#pragma once

#include "UI/Window/WindowId.h"
#include "UI/ProbabilityTable/UIProbabilityTable.h"
#include "Utility/Cipher/LocalEncryptedName.h"
#include "Utility/Cipher/Obfuscate.h"
#include "Network/TextEncoding.h"
#include "UI/Edit/UIEditBoxEx.h"
#include "UI/Lists/UIVScrollBar.h"
#include "UI/Window/UIControl.h"
#include "UI/Window/WindowBorder.h"
#include "UI/Window/UIWindowMgr.h"
#include "UI/Text/UIStaticTextControl.h"
#include "Contents/Tables/MessageTable.h"
#include "Render/Renderer.h"
#include "Utility/Cipher/XorBytes.h"
#include "Utility/Text/FormatString.h"
#include "Utility/Text/StringUtil.h"
#include "UI/Window/UIRegisterMgr.h"
#include "Resource/MSGSTRINGID.h"
#include <cstddef>
#include <cstdint>
#include <algorithm>
#include <cstring>
#include <iterator>
#include <string>
#include <string_view>

template __declspec(noinline) std::function<void(CUIListItem*)>& std::function<void(CUIListItem*)>::operator=(const std::function<void(CUIListItem*)>&);
