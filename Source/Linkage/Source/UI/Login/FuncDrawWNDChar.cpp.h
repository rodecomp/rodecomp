#pragma once

#include "UI/Login/FuncDrawWNDChar.h"
#include "Client/Session/Session.h"
#include "Script/LuaScript.h"
#include "Contents/Sprites/SpriteRidingOffset.h"
#include "Contents/Sprites/SpriteRidingPolicy.h"
#include "Resource/Sprites/ActionResource.h"
#include "Contents/Configuration/ServiceConfiguration.h"
#include "Contents/Skills/ClientSkills.h"
#include "Render/DeviceContext/DCSurface.h"
#include "Render/DeviceContext/DeviceContext.h"
#include "Render/Renderer.h"
#include "Render/Textures/SpriteAtlas.h"
#include "Render/Textures/SpritePixels.h"
#include "Render/Textures/Texture.h"
#include "Resource/Bitmap/PaletteResource.h"
#include "Resource/ResourceMgr.h"
#include "Resource/Sprites/SpriteResource.h"
#include "Script/LuaScript.h"
#include "UI/Window/UIWindow.h"

template __declspec(noinline) std::string::~basic_string() noexcept;
template __declspec(noinline) std::pair<CSprRes*, CActRes*>& std::map<int, std::pair<CSprRes*, CActRes*>>::operator[](const int&);
