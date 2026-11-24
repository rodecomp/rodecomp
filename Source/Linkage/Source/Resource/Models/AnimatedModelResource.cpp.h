#pragma once

#include "Client/Application/ErrorDialog.h"
#include "File/File.h"
#include "Render/DeviceContext/DCSurface.h"
#include "Render/Textures/Texture.h"
#include "Render/Textures/TextureCache.h"
#include "Resource/Models/AnimatedModelResource.h"
#include "Resource/Models/GrannyModelResource.h"
#include "Resource/Bitmap/BitmapResource.h"
#include "Resource/ResourceByteReader.h"
#include "Resource/ResourceMgr.h"
#include <cstdlib>
#include <cstring>
#include <string>

template __declspec(noinline) bool std::_Traits_equal<std::char_traits<char>>(const char*, std::size_t, const char*, std::size_t) noexcept;
template __declspec(noinline) void std::list<std::string>::push_back(std::string&&);
template __declspec(noinline) std::string std::operator+(std::string&&, std::string&&);
