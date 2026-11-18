#pragma once

#include <cstddef>
#include <string>
#include "UI/Gauges/UIBarGraph.h"
#include "Contents/Skin/SkinMgr.h"
#include "Resource/Bitmap/BitmapResource.h"
#include "Resource/ResourceMgr.h"
#include "Resource/Bitmap/SkinPalette.h"
#include "Resource/Bitmap/TextureNames.h"
#include "Utility/Text/FormatString.h"
#include <cstdio>
#include <windows.h>

extern template std::string::basic_string(std::_String_constructor_concat_tag, const std::string&, const char*, std::size_t, const char*, std::size_t);
template __declspec(noinline) std::string::basic_string(std::_String_constructor_concat_tag, const std::string&, const char*, std::size_t, const char*, std::size_t);
