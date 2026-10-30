#include "Client/Mode/CGameMode.h"
#include "Contents/Actors/Pc.h"
#include "Contents/Ground/TerrainHeight.h"
#include "Contents/World/World.h"
#include "Render/Primitives/RenderPrimitivePool.h"
#include "Render/Primitives/RenderQueues.h"
#include "Render/Renderer.h"
#include "Render/TextureQuad.h"
#include "Render/Textures/TextureCache.h"
#include "Resource/World/TerrainResource.h"
#include "UI/Window/UIWindow.h"
#include "UI/Window/UIWindowMgr.h"
#include "Utility/Math/AffineMatrix.h"
#include "Utility/Text/FormatString.h"

#include <windows.h>

// GLOBAL: gl_24934
int g_searchStoreX;
// GLOBAL: gl_24935
int g_searchStoreY;

// OFFSET: 0x00CCFCE0
// FUNCTION: fn_21246
void MiniMapNoOp() {
}

// OFFSET: 0x00CCFFA0

// OFFSET: 0x00CD0150

// OFFSET: 0x00CD0300

// OFFSET: 0x00CD0380

// OFFSET: 0x00CD1450

// OFFSET: 0x00CD2BE0
