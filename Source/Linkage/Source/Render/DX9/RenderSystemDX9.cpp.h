#pragma once

#include "Render/DX9/RenderSystemDX9.h"
#include "UI/GameSettings/GameSettingsUI.h"
#include "Render/Primitives/TextureQuad.h"
#include <new>

template __declspec(noinline) std::shared_ptr<CSurface>& std::shared_ptr<CSurface>::operator=(std::shared_ptr<CSurface>&&) noexcept;
template __declspec(noinline) std::shared_ptr<CSurface>& std::shared_ptr<CSurface>::operator=<CSurfaceDX9>(std::shared_ptr<CSurfaceDX9>&&) noexcept;
