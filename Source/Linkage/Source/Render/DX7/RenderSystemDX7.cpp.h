#pragma once

#include "Render/Device/RenderAdapter.h"

#include <vector>
#include "Render/DX7/RenderSystemDX7.h"
#include "Contents/Configuration/ServiceOptions.h"
#include "Render/DX7/DeviceOwnerDX7.h"
#include "Render/DX7/DirectDrawEnumeration.h"
#include "Render/DX7/SurfaceDX7.h"
#include "Render/DX7/TextureAllocationDX7.h"
#include "Render/DX7/TextureDX7.h"
#include "Render/Device/DeviceSelection.h"
#include <new>

extern template std::vector<RenderAdapter>::~vector();
template __declspec(noinline) std::shared_ptr<CSurface>& std::shared_ptr<CSurface>::operator=(std::shared_ptr<CSurface>&&) noexcept;
template __declspec(noinline) std::shared_ptr<CSurface>& std::shared_ptr<CSurface>::operator=<CSurfaceDX7>(std::shared_ptr<CSurfaceDX7>&&) noexcept;
