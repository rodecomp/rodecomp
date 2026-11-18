#pragma once

#include "Contents/Actors/CharacterRender.h"
#include "Client/Mode/CGameMode.h"
#include "Client/Mode/ModeMgr.h"
#include "Client/Session/Session.h"
#include "Contents/Actors/Pc.h"
#include "Contents/Actors/RenderObject.h"
#include "Contents/Camera/Camera.h"
#include "Contents/Skills/ClientSkills.h"
#include "Contents/Sprites/SpriteLayerDrawing.h"
#include "Contents/Sprites/SpriteLayerOrder.h"
#include "Contents/Sprites/SpriteRidingOffset.h"
#include "Contents/Sprites/SpriteRidingPolicy.h"
#include "Render/DeviceContext/DCSurface.h"
#include "Render/Primitives/RenderPrimitive.h"
#include "Render/Renderer.h"
#include "Render/Drawing/SpriteGeometry.h"
#include "Render/Primitives/SpriteQuad.h"
#include "Render/Textures/SpritePixels.h"
#include "Resource/Sprites/ImfResource.h"
#include "Resource/Bitmap/PaletteResource.h"
#include "Resource/ResourceMgr.h"
#include "Resource/Sprites/ActionResource.h"
#include "Resource/Sprites/SpriteResource.h"
#include "Script/LuaScript.h"
#include <algorithm>
#include <cstring>
#include <list>
#include <map>
#include <string>
#include <vector>
#include <windows.h>

template __declspec(noinline) std::vector<CActRes*>::vector(const size_type, CActRes* const&, const std::allocator<CActRes*>&);
template __declspec(noinline) std::vector<CSprRes*>::vector(const size_type, CSprRes* const&, const std::allocator<CSprRes*>&);
template __declspec(noinline) std::vector<CActRes*>::~vector() noexcept;
template __declspec(noinline) std::vector<CSprRes*>::~vector() noexcept;
template __declspec(noinline) void std::list<CharacterRenderDrawItem>::push_back(const CharacterRenderDrawItem&);
template __declspec(noinline) std::pair<std::map<int, SpriteLayerQuad>::iterator, bool> std::map<int, SpriteLayerQuad>::_Mybase::emplace<std::pair<int, SpriteLayerQuad>>(std::pair<int, SpriteLayerQuad>&&);
template __declspec(noinline) std::map<int, SpriteLayerQuad>::_Mybase::~_Tree() noexcept;
