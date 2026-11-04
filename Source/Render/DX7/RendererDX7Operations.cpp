#include "Render/DX7/RenderSystemDX7.h"
#include "Client/Mode/CGameMode.h"
#include "Client/Mode/ModeMgr.h"
#include "Contents/Configuration/ServiceOptions.h"
#include "Contents/World/World.h"
#include "Render/BlendColor.h"
#include "Render/DX7/DeviceOwnerDX7.h"
#include "Render/DX7/QueuedDrawingDX7.h"
#include "Render/Primitives/LightmappedPrimitive.h"
#include "Render/Primitives/RenderPrimitive.h"
#include "Render/RendererDX7.h"
#include "Render/TextureQuad.h"

#include <algorithm>
#include <bit>
#include <cstdint>

// OFFSET: 0x0055EC10
// FUNCTION: fn_02114
void CRendererDX7::SetLighting(const float* direction, const float* diffuse, const float* ambient) {
  D3DLIGHT7 light{};
  light.dltType = D3DLIGHT_DIRECTIONAL;
  light.dvDirection.x = -direction[0];
  light.dvDirection.y = -direction[1];
  light.dvDirection.z = -direction[2];
  light.dcvDiffuse.r = diffuse[0];
  light.dcvDiffuse.g = diffuse[1];
  light.dcvDiffuse.b = diffuse[2];
  light.dcvDiffuse.a = 255.0f;
  light.dcvAmbient.r = ambient[0];
  light.dcvAmbient.g = ambient[1];
  light.dcvAmbient.b = ambient[2];
  light.dcvAmbient.a = 255.0f;
  this->device->SetLight(0, &light);
  this->device->LightEnable(0, TRUE);
}

// OFFSET: 0x0055A210
// FUNCTION: fn_02077
void CRendererDX7::ClearDepth(int clearColor) {
  if (clearColor) {
    if (g_deviceOwnerDX7) {
      g_deviceOwnerDX7->Clear(0xFF000000);
    }
  } else if (g_deviceOwnerDX7) {
    g_deviceOwnerDX7->ClearDepth();
  }
}

// OFFSET: 0x0055A240
// FUNCTION: fn_02078
void CRendererDX7::ClearWorld() {
  unsigned color = this->clearColor;
  if (this->fog.enabled != 0 && static_cast<CGameMode*>(g_modeMgr.GetGameMode())->scene->unknown_00FC == 1) {
    color = BlendColor(this->clearColor, g_worldOverlayColor);
  }

  if (g_deviceOwnerDX7) {
    g_deviceOwnerDX7->Clear(color);
  }
}

// OFFSET: 0x0055ACB0
// FUNCTION: fn_02082
void CRendererDX7::DrawPrimitive(RenderCommand* primitive) {
  if (primitive->vertexCount <= 0) {
    return;
  }

  const auto type = static_cast<D3DPRIMITIVETYPE>(primitive->primitiveType);

  if (primitive->indexCount) {
    this->device->DrawIndexedPrimitive(
      type, D3DFVF_XYZRHW | D3DFVF_DIFFUSE | D3DFVF_SPECULAR | D3DFVF_TEX1, primitive->vertices, primitive->vertexCount, primitive->indices,
      primitive->indexCount, 0
    );
  } else {
    this->device->DrawPrimitive(type, D3DFVF_XYZRHW | D3DFVF_DIFFUSE | D3DFVF_SPECULAR | D3DFVF_TEX1, primitive->vertices, primitive->vertexCount, 0);
  }
}

// OFFSET: 0x0055AD00
// FUNCTION: fn_02083
// OFFSET: 0x0055AD50
// FUNCTION: fn_02084
void CRendererDX7::DrawLightmappedPrimitive(LightmappedPrimitive* primitive) {
  if (primitive->vertexCount <= 0) {
    return;
  }

  const auto type = static_cast<D3DPRIMITIVETYPE>(primitive->primitiveType);

  if (primitive->indexCount) {
    this->device->DrawIndexedPrimitive(
      type, D3DFVF_XYZRHW | D3DFVF_DIFFUSE | D3DFVF_SPECULAR | D3DFVF_TEX2, primitive->vertices, primitive->vertexCount, primitive->indices,
      primitive->indexCount, 0
    );
  } else {
    this->device->DrawPrimitive(type, D3DFVF_XYZRHW | D3DFVF_DIFFUSE | D3DFVF_SPECULAR | D3DFVF_TEX2, primitive->vertices, primitive->vertexCount, 0);
  }
}

// OFFSET: 0x0055AFA0
// FUNCTION: fn_02086
std::uint16_t DeviceOwnerDX7Handle::PackColor(int red, int green, int blue) const {
  const DeviceOwnerDX7* owner = this->owner;
  if (!owner) {
    return 0;
  }

  return static_cast<std::uint16_t>(
    (static_cast<std::uint16_t>(blue >> owner->blue.expansion) << owner->blue.shift) +
    (static_cast<std::uint16_t>(green >> owner->green.expansion) << owner->green.shift) +
    (static_cast<std::uint16_t>(red >> owner->red.expansion) << owner->red.shift)
  );
}

// OFFSET: 0x0055C110
// FUNCTION: fn_45496
void CRendererDX7::DrawTopmostQueue() {
  std::sort(this->queues.queue_021C.begin(), this->queues.queue_021C.end(), [](const DepthPrimitive& left, const DepthPrimitive& right) {
    if (left.depth < right.depth) {
      return true;
    }

    if (right.depth < left.depth) {
      return false;
    }

    return left.depth == right.depth && reinterpret_cast<std::uintptr_t>(left.primitive) < reinterpret_cast<std::uintptr_t>(right.primitive);
  });

  for (auto it = this->queues.queue_021C.begin(); it != this->queues.queue_021C.end(); ++it) {
    RenderCommand* command = it->primitive;
    this->BindTexture(static_cast<CTextureDX7*>(command->texture));
    command->Draw(reinterpret_cast<int>(this->device));
  }
}

// OFFSET: 0x0055C1B0
// FUNCTION: fn_45497
void CRendererDX7::DrawBlendedQueue() {
  for (auto it = this->queues.queue_0210.begin(); it != this->queues.queue_0210.end(); ++it) {
    if (RenderCommand* command = *it) {
      this->BindTexture(static_cast<CTextureDX7*>(command->texture));
      this->SetTextureMode(command->unknown_0020);
      this->device->SetRenderState(D3DRENDERSTATE_SRCBLEND, command->unknown_0018);
      this->device->SetRenderState(D3DRENDERSTATE_DESTBLEND, command->unknown_001C);
      command->Draw(reinterpret_cast<int>(this->device));
    }
  }

  this->SetTextureMode(0);
}

// OFFSET: 0x0055C270
// FUNCTION: fn_45495
void CRendererDX7::FlushRenderList() {
  const auto byDepth = [](const DepthPrimitive& left, const DepthPrimitive& right) {
    if (left.depth < right.depth) {
      return true;
    }

    if (right.depth < left.depth) {
      return false;
    }

    return left.depth == right.depth && reinterpret_cast<std::uintptr_t>(left.primitive) < reinterpret_cast<std::uintptr_t>(right.primitive);
  };

  this->device->SetTextureStageState(0, D3DTSS_MIPFILTER, D3DTFP_LINEAR);
  this->ApplyFog();
  this->device->SetTextureStageState(0, D3DTSS_COLOROP, D3DTOP_MODULATE);
  this->device->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_MODULATE);
  this->device->SetTextureStageState(1, D3DTSS_COLOROP, D3DTOP_DISABLE);
  this->device->SetTextureStageState(1, D3DTSS_ALPHAOP, D3DTOP_DISABLE);
  this->device->SetRenderState(D3DRENDERSTATE_FOGENABLE, this->fog.enabled);
  this->device->SetRenderState(D3DRENDERSTATE_ALPHAREF, 207);
  this->device->SetRenderState(D3DRENDERSTATE_ALPHATESTENABLE, TRUE);
  this->device->SetRenderState(D3DRENDERSTATE_ALPHABLENDENABLE, FALSE);
  this->device->SetRenderState(D3DRENDERSTATE_ZENABLE, D3DZB_TRUE);
  this->device->SetRenderState(D3DRENDERSTATE_ZWRITEENABLE, TRUE);
  this->DrawOpaqueQueues();
  this->device->SetRenderState(D3DRENDERSTATE_AMBIENT, 0xFFFFFFFF);
  this->device->SetRenderState(D3DRENDERSTATE_CULLMODE, D3DCULL_CW);

  if (g_serviceOptions.Get(119)) {
    if (g_multitexture) {
      this->device->SetRenderState(D3DRENDERSTATE_COLORKEYENABLE, FALSE);
      this->device->SetTextureStageState(1, D3DTSS_COLOROP, D3DTOP_MODULATEALPHA_ADDCOLOR);
      std::sort(
        this->queues.lightmapped.first.begin(), this->queues.lightmapped.first.end(),
        [](const LightmappedPrimitive* left, const LightmappedPrimitive* right) {
          const auto first = reinterpret_cast<std::uintptr_t>(left->texture);
          const auto second = reinterpret_cast<std::uintptr_t>(right->texture);

          if (first != second) {
            return first < second;
          }

          return reinterpret_cast<std::uintptr_t>(left->lightmap) < reinterpret_cast<std::uintptr_t>(right->lightmap);
        }
      );

      for (auto it = this->queues.lightmapped.first.begin(); it != this->queues.lightmapped.first.end(); ++it) {
        LightmappedPrimitive* command = *it;

        if (command->vertexCount > 0) {
          this->BindTexture(static_cast<CTextureDX7*>(command->texture));
          this->BindLightmap(static_cast<CTextureDX7*>(command->lightmap));
          command->DrawLightmapped(reinterpret_cast<int>(this->device));
        }
      }

      this->device->SetRenderState(D3DRENDERSTATE_COLORKEYENABLE, TRUE);
      this->device->SetTextureStageState(1, D3DTSS_COLOROP, D3DTOP_DISABLE);
    } else {
      this->DrawGroundBaseQueue();
      this->device->SetRenderState(D3DRENDERSTATE_COLORKEYENABLE, FALSE);
      this->device->SetTextureStageState(0, D3DTSS_COLOROP, D3DTOP_SELECTARG1);
      this->device->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_SELECTARG1);
      this->device->SetRenderState(D3DRENDERSTATE_ALPHABLENDENABLE, TRUE);
      this->device->SetRenderState(D3DRENDERSTATE_ALPHAREF, 1);
      this->device->SetRenderState(D3DRENDERSTATE_SRCBLEND, D3DBLEND_ONE);
      this->device->SetRenderState(D3DRENDERSTATE_DESTBLEND, D3DBLEND_SRCALPHA);
      std::sort(this->queues.queue_0174.begin(), this->queues.queue_0174.end(), [](const RenderCommand* left, const RenderCommand* right) {
        return reinterpret_cast<std::uintptr_t>(left->texture) < reinterpret_cast<std::uintptr_t>(right->texture);
      });

      for (auto it = this->queues.queue_0174.begin(); it != this->queues.queue_0174.end(); ++it) {
        RenderCommand* command = *it;
        this->BindTexture(static_cast<CTextureDX7*>(command->texture));
        command->Draw(reinterpret_cast<int>(this->device));
      }

      this->device->SetRenderState(D3DRENDERSTATE_COLORKEYENABLE, TRUE);
      this->device->SetTextureStageState(0, D3DTSS_COLOROP, D3DTOP_MODULATE);
      this->device->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_MODULATE);
      this->device->SetRenderState(D3DRENDERSTATE_ALPHABLENDENABLE, FALSE);
      this->device->SetRenderState(D3DRENDERSTATE_ALPHAREF, 207);
    }
  } else {
    this->DrawGroundBaseQueue();
  }

  this->device->SetRenderState(D3DRENDERSTATE_COLORKEYENABLE, FALSE);
  this->device->SetRenderState(D3DRENDERSTATE_AMBIENT, 0);
  this->device->SetRenderState(D3DRENDERSTATE_CULLMODE, D3DCULL_NONE);
  this->device->SetRenderState(D3DRENDERSTATE_ALPHAREF, 1);
  this->device->SetRenderState(D3DRENDERSTATE_ALPHABLENDENABLE, TRUE);
  this->DrawAuxiliaryQueue();

  DWORD fogEnable = 0;
  DWORD zWrite = 0;
  this->device->GetRenderState(D3DRENDERSTATE_FOGENABLE, &fogEnable);
  this->device->GetRenderState(D3DRENDERSTATE_ZWRITEENABLE, &zWrite);
  this->device->SetRenderState(D3DRENDERSTATE_FOGENABLE, FALSE);
  this->device->SetRenderState(D3DRENDERSTATE_ZWRITEENABLE, FALSE);
  this->device->SetRenderState(D3DRENDERSTATE_SRCBLEND, D3DBLEND_ONE);
  this->device->SetRenderState(D3DRENDERSTATE_DESTBLEND, D3DBLEND_ONE);
  this->DrawBlendedQueue();
  this->device->SetRenderState(D3DRENDERSTATE_FOGENABLE, fogEnable);
  this->device->SetRenderState(D3DRENDERSTATE_ZWRITEENABLE, zWrite);
  this->device->SetRenderState(D3DRENDERSTATE_SRCBLEND, D3DBLEND_SRCALPHA);
  this->device->SetRenderState(D3DRENDERSTATE_DESTBLEND, D3DBLEND_INVSRCALPHA);
  this->device->SetRenderState(D3DRENDERSTATE_ZENABLE, D3DZB_FALSE);
  this->DrawTopmostQueue();
  this->device->SetRenderState(D3DRENDERSTATE_ZENABLE, D3DZB_TRUE);

  DWORD depthWrite = 0;
  DWORD alphaTest = 0;
  DWORD alphaReference = 0;
  DWORD alphaFunction = 0;
  this->device->GetRenderState(D3DRENDERSTATE_ZWRITEENABLE, &depthWrite);
  this->device->GetRenderState(D3DRENDERSTATE_ALPHATESTENABLE, &alphaTest);
  this->device->GetRenderState(D3DRENDERSTATE_ALPHAREF, &alphaReference);
  this->device->GetRenderState(D3DRENDERSTATE_ALPHAFUNC, &alphaFunction);
  this->device->SetRenderState(D3DRENDERSTATE_ALPHATESTENABLE, TRUE);
  this->device->SetRenderState(D3DRENDERSTATE_ZWRITEENABLE, TRUE);
  this->device->SetRenderState(D3DRENDERSTATE_ALPHAREF, 159);
  this->device->SetRenderState(D3DRENDERSTATE_ALPHAFUNC, D3DCMP_GREATEREQUAL);
  this->DrawDepthSortedQueue();
  this->device->SetRenderState(D3DRENDERSTATE_ZWRITEENABLE, FALSE);
  this->device->SetRenderState(D3DRENDERSTATE_ALPHAREF, 159);
  this->device->SetRenderState(D3DRENDERSTATE_ALPHAFUNC, D3DCMP_LESS);
  this->DrawDepthSortedQueue();
  this->device->SetRenderState(D3DRENDERSTATE_ZWRITEENABLE, depthWrite);
  this->device->SetRenderState(D3DRENDERSTATE_ALPHATESTENABLE, alphaTest);
  this->device->SetRenderState(D3DRENDERSTATE_ALPHAREF, alphaReference);
  this->device->SetRenderState(D3DRENDERSTATE_ALPHAFUNC, alphaFunction);

  this->device->SetRenderState(D3DRENDERSTATE_SRCBLEND, D3DBLEND_ONE);
  this->device->SetRenderState(D3DRENDERSTATE_DESTBLEND, D3DBLEND_ONE);
  std::sort(this->queues.queue_01C8.begin(), this->queues.queue_01C8.end(), byDepth);

  for (auto it = this->queues.queue_01C8.begin(); it != this->queues.queue_01C8.end(); ++it) {
    RenderCommand* command = it->primitive;
    this->BindTexture(static_cast<CTextureDX7*>(command->texture));
    command->Draw(reinterpret_cast<int>(this->device));
  }

  this->device->SetRenderState(D3DRENDERSTATE_ZWRITEENABLE, FALSE);
  this->device->SetRenderState(D3DRENDERSTATE_SRCBLEND, D3DBLEND_SRCALPHA);
  this->device->SetRenderState(D3DRENDERSTATE_DESTBLEND, D3DBLEND_INVSRCALPHA);
  this->DrawAlphaQueues();

  DWORD reference = 0;
  this->device->GetRenderState(D3DRENDERSTATE_ALPHAREF, &reference);
  this->device->SetRenderState(D3DRENDERSTATE_ALPHAREF, 207);
  std::sort(this->queues.queue_0228.begin(), this->queues.queue_0228.end(), byDepth);

  for (auto it = this->queues.queue_0228.begin(); it != this->queues.queue_0228.end(); ++it) {
    RenderCommand* command = it->primitive;
    this->BindTexture(static_cast<CTextureDX7*>(command->texture));
    command->Draw(reinterpret_cast<int>(this->device));
  }

  this->device->SetRenderState(D3DRENDERSTATE_ALPHAREF, reference);
  this->device->SetRenderState(D3DRENDERSTATE_FOGENABLE, FALSE);
  this->device->SetRenderState(D3DRENDERSTATE_SRCBLEND, D3DBLEND_ONE);
  this->device->SetRenderState(D3DRENDERSTATE_DESTBLEND, D3DBLEND_ONE);
  std::sort(this->queues.queue_01D4.begin(), this->queues.queue_01D4.end(), byDepth);

  for (auto it = this->queues.queue_01D4.begin(); it != this->queues.queue_01D4.end(); ++it) {
    RenderCommand* command = it->primitive;
    this->BindTexture(static_cast<CTextureDX7*>(command->texture));
    command->Draw(reinterpret_cast<int>(this->device));
  }

  this->device->SetTextureStageState(0, D3DTSS_ADDRESS, D3DTADDRESS_WRAP);

  for (auto it = this->queues.queue_01EC.begin(); it != this->queues.queue_01EC.end(); ++it) {
    if (RenderCommand* command = *it) {
      this->BindTexture(static_cast<CTextureDX7*>(command->texture));
      this->SetTextureMode(command->unknown_0020);
      this->device->SetRenderState(D3DRENDERSTATE_SRCBLEND, command->unknown_0018);
      this->device->SetRenderState(D3DRENDERSTATE_DESTBLEND, command->unknown_001C);
      command->Draw(reinterpret_cast<int>(this->device));
    }
  }

  this->SetTextureMode(0);
  this->device->SetTextureStageState(0, D3DTSS_ADDRESS, D3DTADDRESS_CLAMP);
  this->device->SetRenderState(D3DRENDERSTATE_ZENABLE, D3DZB_FALSE);
  this->device->SetRenderState(D3DRENDERSTATE_SRCBLEND, D3DBLEND_ONE);
  this->device->SetRenderState(D3DRENDERSTATE_DESTBLEND, D3DBLEND_ONE);
  std::sort(this->queues.queue_01E0.begin(), this->queues.queue_01E0.end(), byDepth);

  for (auto it = this->queues.queue_01E0.begin(); it != this->queues.queue_01E0.end(); ++it) {
    RenderCommand* command = it->primitive;
    this->BindTexture(static_cast<CTextureDX7*>(command->texture));
    command->Draw(reinterpret_cast<int>(this->device));
  }

  this->device->SetRenderState(D3DRENDERSTATE_SRCBLEND, D3DBLEND_SRCALPHA);
  this->device->SetRenderState(D3DRENDERSTATE_DESTBLEND, D3DBLEND_INVSRCALPHA);

  for (auto it = this->queues.queue_0198.begin(); it != this->queues.queue_0198.end(); ++it) {
    RenderCommand* command = it->primitive;
    this->BindTexture(static_cast<CTextureDX7*>(command->texture));
    command->Draw(reinterpret_cast<int>(this->device));
  }

  for (auto it = this->queues.queue_0234.begin(); it != this->queues.queue_0234.end(); ++it) {
    if (RenderCommand* command = it->primitive) {
      this->BindTexture(static_cast<CTextureDX7*>(command->texture));
      this->SetTextureMode(command->unknown_0020);

      if (it->depth >= 0.99f) {
        this->device->SetRenderState(D3DRENDERSTATE_SRCBLEND, D3DBLEND_SRCALPHA);
        this->device->SetRenderState(D3DRENDERSTATE_DESTBLEND, D3DBLEND_INVSRCALPHA);
      } else {
        this->device->SetRenderState(D3DRENDERSTATE_SRCBLEND, command->unknown_0018);
        this->device->SetRenderState(D3DRENDERSTATE_DESTBLEND, command->unknown_001C);
      }

      command->Draw(reinterpret_cast<int>(this->device));
    }
  }

  this->SetTextureMode(0);
  this->device->SetRenderState(D3DRENDERSTATE_SRCBLEND, D3DBLEND_SRCALPHA);
  this->device->SetRenderState(D3DRENDERSTATE_DESTBLEND, D3DBLEND_INVSRCALPHA);
  this->device->SetTextureStageState(0, D3DTSS_MIPFILTER, D3DTFP_NONE);
}

// OFFSET: 0x0055ADA0
// FUNCTION: fn_02085
bool CRendererDX7::Draw() {
  this->screenWidth = g_deviceOwnerDX7 ? g_deviceOwnerDX7->width : 0;
  this->screenHeight = g_deviceOwnerDX7 ? g_deviceOwnerDX7->height : 0;

  if (SUCCEEDED(this->device->BeginScene())) {
    this->FlushRenderList();
    this->ClearFrame();
    this->device->EndScene();
    return true;
  } else {
    this->ClearFrame();
    return false;
  }
}
