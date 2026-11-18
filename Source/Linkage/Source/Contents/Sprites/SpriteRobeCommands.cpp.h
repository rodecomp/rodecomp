#pragma once

#include "Client/Mode/CGameMode.h"
#include "Client/Mode/ModeMgr.h"
#include "Client/Session/Session.h"
#include "Contents/Actors/CharacterRender.h"
#include "Contents/Camera/Camera.h"
#include "Render/DeviceContext/DCSurface.h"
#include "Render/Primitives/RenderPrimitive.h"
#include "Render/Drawing/SpriteGeometry.h"
#include "Render/Primitives/SpriteQuad.h"
#include "Render/Textures/SpritePixels.h"
#include "Resource/ResourceMgr.h"
#include "Resource/Sprites/ActionResource.h"
#include "Resource/Sprites/SpriteResource.h"
#include <list>
#include <string>
#include <windows.h>

template __declspec(noinline) void std::list<CharacterRenderDrawItem>::push_back(const CharacterRenderDrawItem&);
