#pragma once

#include "UI/Window/UIEnums.h"
#include "Client/Session/Session.h"
#include "Contents/Tables/MessageTable.h"
#include "File/ShortcutFilename.h"
#include "Render/DeviceContext/DeviceContext.h"
#include "Resource/Bitmap/BitmapResource.h"
#include "Resource/ResourceMgr.h"
#include "Resource/Bitmap/TextureNames.h"
#include "UI/Party/UIPartyInfoWnd.h"
#include "UI/Window/UIWindowMgr.h"
#include "Utility/Text/FormatString.h"
#include "Resource/MSGSTRINGID.h"

template __declspec(noinline) std::string::basic_string(const std::string&);
template __declspec(noinline) std::string& std::string::operator=(std::string&&) noexcept;
