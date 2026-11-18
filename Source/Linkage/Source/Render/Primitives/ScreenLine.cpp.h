#pragma once

#include "Script/LuaScript.h"

#include <cstddef>
#include <map>
#include <string>
#include <vector>
#include "Render/DeviceContext/DCSurface.h"
#include "UI/Navigation/UIRoMapWnd.h"
#include "Client/Session/Session.h"
#include "Render/Textures/TextureCache.h"
#include "Script/MiniMapPosition.h"
#include "UI/Text/ColoredText.h"
#include "UI/Window/WindowBorder.h"
#include "Contents/Map/MapSkyMgr.h"
#include "Render/Primitives/AlphaRectangle.h"
#include "Render/Renderer.h"
#include "Render/Projection/ScreenGeometry.h"
#include "Render/Primitives/TextureQuad.h"
#include "Contents/Configuration/ServiceConfiguration.h"
#include "Render/Primitives/RenderPrimitive.h"
#include "Render/Primitives/RenderPrimitivePool.h"
#include "Render/Primitives/RenderQueues.h"
#include "Render/Textures/SpriteAtlas.h"
#include "Render/Textures/SpritePixels.h"
#include "Resource/ResourceMgr.h"
#include "Resource/Sprites/ActionResource.h"
#include "Resource/Sprites/SpriteResource.h"
#include "UI/Chat/UINewChatWnd.h"
#include "UI/Chat/UISubChatHisWnd.h"
#include "UI/Lists/UITabControl.h"
#include "Utility/Math/AffineMatrix.h"
#include <cmath>
#include <windows.h>
#include <timeapi.h>

extern template int std::_Traits_compare<std::char_traits<char>>(const char*, std::size_t, const char*, std::size_t) noexcept;
using KafraServiceMapTree = std::_Tree<std::_Tmap_traits<
  std::string, std::vector<std::string>, std::less<std::string>, std::allocator<std::pair<const std::string, std::vector<std::string>>>, false>>;
extern template std::_Tree_find_result<std::_Tree_node<std::pair<const std::string, std::vector<std::string>>, void*>*>
KafraServiceMapTree::_Find_lower_bound<std::string>(const std::string&) const;
using MiniMapPositionTree = std::_Tree<std::_Tmap_traits<
  std::string, MiniMapPosition, std::less<std::string>, std::allocator<std::pair<const std::string, MiniMapPosition>>, false>>;
extern template std::_Tree_find_result<std::_Tree_node<std::pair<const std::string, MiniMapPosition>, void*>*>
MiniMapPositionTree::_Find_lower_bound<std::string>(const std::string&) const;
extern template MiniMapPosition& std::map<std::string, MiniMapPosition>::operator[](const std::string&);
template __declspec(noinline) int std::_Traits_compare<std::char_traits<char>>(const char*, std::size_t, const char*, std::size_t) noexcept;
