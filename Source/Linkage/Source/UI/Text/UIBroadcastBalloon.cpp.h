#pragma once

#include "UI/Text/UIBroadcastBalloon.h"
#include "Render/Primitives/AlphaRectangle.h"
#include "Render/Renderer.h"
#include "Render/Primitives/TextureQuad.h"
#include "UI/Buttons/UIButton.h"
#include "UI/Window/WindowBorder.h"
#include <windows.h>

template decltype(auto) std::list<BroadcastBalloonLine>::emplace_back<std::string&, std::uint32_t&>(std::string&, std::uint32_t&);
