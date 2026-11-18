#pragma once

#include "Client/Mode/ModeMessage.h"
#include "Contents/Skin/SkinMgr.h"
#include "UI/Window/WindowId.h"
#include "UI/Exchange/UIExchangeUI.h"
#include "UI/Window/UIEnums.h"
#include "Utility/Cipher/LocalEncryptedName.h"
#include "Utility/Cipher/CryptName.h"
#include "Utility/Cipher/Obfuscate.h"
#include "Client/Session/Session.h"
#include "Client/Mode/CGameMode.h"
#include "Client/Mode/CMode.h"
#include "Client/Mode/ModeDragState.h"
#include "Client/Mode/ModeMgr.h"
#include "Contents/Configuration/ServiceConfiguration.h"
#include "Contents/Configuration/ServiceEnums.h"
#include "Contents/Tables/MessageTable.h"
#include "File/ShortcutFilename.h"
#include "Render/Renderer.h"
#include "Resource/ResourceMgr.h"
#include "Resource/Bitmap/SkinPalette.h"
#include "UI/Buttons/UIRadioButton.h"
#include "UI/Lists/UIVScrollBar.h"
#include "UI/Shop/PriceColor.h"
#include "UI/Text/UIStaticTextControl.h"
#include "UI/Window/UIWindowMgr.h"
#include "Utility/Cipher/XorBytes.h"
#include "Utility/Text/FormatString.h"
#include "Utility/Text/GroupedInteger.h"
#include "UI/Window/UIRegisterMgr.h"
#include "Resource/MSGSTRINGID.h"
#include "UI/Login/UIPasswordWnd.h"
#include <cstddef>
#include <cstdint>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <format>
#include <functional>
#include <string>
#include <windows.h>

template __declspec(noinline) std::list<ExchangeUI::CItemData>::~list() noexcept;
template __declspec(noinline) std::function<void(CUIListItem*)>& std::function<void(CUIListItem*)>::operator=(const std::function<void(CUIListItem*)>&);
template void CUIListBoxEx<ExchangeUI::CUIItem, ExchangeUI::CItemData>::SetOnSelect(std::function<void(ExchangeUI::CUIItem*, const ExchangeUI::CItemData&)>);
template void CUIListBoxEx<ExchangeUI::CUIItem, ExchangeUI::CItemData>::SetBinder(std::function<void(ExchangeUI::CUIItem*, const ExchangeUI::CItemData&)>);
template void CUIListBoxEx<ExchangeUI::CUIItem, ExchangeUI::CItemData>::SetImages(const std::string&, const std::string&, const std::string&,
                                                                                    const std::string&);
