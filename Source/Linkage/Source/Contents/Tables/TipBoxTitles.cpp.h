#pragma once

#include "Contents/Tables/TipBoxTitles.h"
#include "Client/Input/InputCharset.h"
#include "Resource/Bitmap/BitmapResource.h"
#include "Resource/ResourceMgr.h"
#include "Resource/Sprites/SpriteResource.h"
#include "Resource/Bitmap/TextureNames.h"
#include "Script/NLuaState.h"
#include "Utility/Cipher/XorBytes.h"
#include "Utility/Cipher/LocalEncryptedName.h"
#include "Utility/Cipher/CryptName.h"
#include "Utility/Cipher/Obfuscate.h"
extern "C" {
#include <lauxlib.h>
#include <lua.h>
#include <lualib.h>
}
#include <algorithm>
#include <cctype>
#include <cstddef>
#include <cstdint>
#include <cstdlib>
#include <mbstring.h>
#include <new>
#include <windows.h>

template __declspec(noinline) std::string std::operator+(std::string&&, const char* const);
