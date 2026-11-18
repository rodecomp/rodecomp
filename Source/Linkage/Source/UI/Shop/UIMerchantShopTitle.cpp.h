#pragma once

#include <string>
#include "Client/Mode/CMode.h"
#include "UI/GameSettings/TalkType.h"
#include "Client/Mode/ModeMessage.h"
#include "UI/Window/UIEnums.h"
#include "Client/Mode/CGameMode.h"
#include "Client/Mode/ModeMgr.h"
#include "Client/Session/Session.h"
#include "Contents/Configuration/ServiceOptions.h"
#include "Contents/Tables/ForbiddenWords.h"
#include "Resource/Bitmap/BitmapResource.h"
#include "Resource/ResourceMgr.h"
#include "Resource/Bitmap/TextureNames.h"
#include "UI/Shop/UIMerchantShopTitle.h"
#include "UI/Login/UINewSelectCharWnd.h"
#include "UI/Window/UIWindowMgr.h"
#include "UI/Window/WindowBorder.h"
#include "UI/Text/UIStaticText.h"

extern template std::string& std::string::operator=(std::string&&) noexcept;
template __declspec(noinline) std::string& std::string::operator=(std::string&&) noexcept;
