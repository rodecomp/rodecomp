#pragma once

#include "Client/Mode/CGameMode.h"
#include "Client/Mode/ModeMgr.h"
#include "Contents/Movies/BinkMovies.h"
#include "Contents/World/World.h"
#include "Render/DeviceContext/DCSurface.h"
#include "Render/RenderSystemBase.h"
#include "Render/Textures/Texture.h"
#include "Render/Textures/TextureCache.h"
#include "Resource/Bitmap/BitmapResource.h"
#include "Resource/ResourceMgr.h"
#include "Resource/Bitmap/TextureNames.h"
#include <cstring>
#include <string>
#include <windows.h>
#include <timeapi.h>

template __declspec(noinline) std::string::basic_string(const char* const);
using TextureMap = std::map<const char*, CTexture*, TextureNameLess>;
template __declspec(noinline) TextureMap::iterator TextureMap::_Mybase::find(const char* const&);
