#pragma once

#include "Client/Input/InputCharset.h"
#include "Contents/Configuration/ServiceEnums.h"
#include "Client/Input/LanguageTC.h"
#include "Contents/Configuration/ServiceConfiguration.h"
#include "Archive/PakRegistry.h"
#include "Client/Application/ErrorDialog.h"
#include "Client/Application/Registry.h"
#include "Client/Session/Session.h"
#include "Contents/Map/MapSkyMgr.h"
#include "File/CommonObject.h"
#include "File/File.h"
#include "File/ResourcePath.h"
#include "Network/TextEncoding.h"
#include "Render/DeviceContext/DCSurface.h"
#include "Resource/Bitmap/BitmapResource.h"
#include "Resource/ResourceMgr.h"
#include "Resource/Xml/XmlNode.h"
#include "UI/Text/UIBalloonText.h"
#include "Utility/Text/TextTokens.h"
#include "Utility/Cipher/XorBytes.h"
#include "Utility/Text/FormatString.h"
#include "UI/Login/UILoginWnd.h"
#include "Utility/Cipher/LocalEncryptedName.h"
#include "Utility/Cipher/CryptName.h"
#include "Utility/Cipher/Obfuscate.h"
#include <cstddef>
#include <cstdint>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <iterator>
#include <string>
#include <vector>
#include <windows.h>

template __declspec(noinline) bool std::_Traits_equal<std::char_traits<char>>(const char*, std::size_t, const char*, std::size_t) noexcept;
