#pragma once

#include "UI/Chat/UINewChatWnd.h"
#include "UI/Window/UIEnums.h"
#include "Client/Input/InputCharset.h"
#include "Contents/Items/ItemNaming.h"
#include "Render/Text/TextContext.h"
#include "UI/Navigation/NavigationLink.h"
#include "UI/Text/PixelTextWrapping.h"
#include "UI/Text/TextMarkup.h"
#include "UI/Text/TextWrapping.h"
#include "UI/Lists/UIChatHisBox.h"
#include "UI/Navigation/UIListBoxNavi.h"
#include "UI/Lists/UIScrollBar.h"
#include "UI/Buttons/UITextButton.h"
#include "UI/Text/UITextViewer.h"
#include <cstdint>
#include <cstring>
#include <string>
#include <vector>

template __declspec(noinline) std::string std::string::substr(std::string::size_type, std::string::size_type) const;
