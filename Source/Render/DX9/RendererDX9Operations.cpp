#include "Render/DX9/DeviceOwnerDX9.h"
#include "Render/Drawing/BlendColor.h"
#include "Client/Mode/CGameMode.h"
#include "Client/Mode/ModeMgr.h"
#include "Contents/World/World.h"
#include "Render/Primitives/TextureQuad.h"
#include "Render/DX9/DeviceStateDX9.h"
#include "Render/Primitives/LightmappedPrimitive.h"
#include "Render/Primitives/RenderPrimitive.h"
#include "Render/Primitives/RenderQueues.h"
#include "Render/DX9/RendererDX9.h"
#include "Render/DX9/TextureDX9.h"

#include <algorithm>
#include <atlbase.h>
#include <bit>
#include <functional>
#include <new>

// OFFSET: 0x00568460
// FUNCTION: fn_02240
void CRendererDX9::SetCamera(const float*, const float*, const float*) {
}

// OFFSET: 0x00568370
// FUNCTION: fn_02239
void CRendererDX9::SetLighting(const float* direction, const float* diffuse, const float* ambient) {
  D3DLIGHT9 light{};
  light.Type = D3DLIGHT_DIRECTIONAL;
  light.Direction.x = -direction[0];
  light.Direction.y = -direction[1];
  light.Direction.z = -direction[2];
  light.Diffuse.r = diffuse[0];
  light.Diffuse.g = diffuse[1];
  light.Diffuse.b = diffuse[2];
  light.Diffuse.a = 255.0f;
  light.Ambient.r = ambient[0];
  light.Ambient.g = ambient[1];
  light.Ambient.a = 255.0f;
  light.Ambient.b = ambient[2];

  this->nativeDevice->SetLight(0, &light);
  this->nativeDevice->LightEnable(0, TRUE);
}

// OFFSET: 0x00565150
// FUNCTION: fn_02214
void CRendererDX9::ClearDepth(int clearColor) {
  if (clearColor) {
    this->nativeDevice->Clear(0, nullptr, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, 0xFF000000u, 1.0f, 0);
  } else {
    this->nativeDevice->Clear(0, nullptr, D3DCLEAR_ZBUFFER, 0xFF000000u, 1.0f, 0);
  }
}

// OFFSET: 0x005651A0
// FUNCTION: fn_02215
void CRendererDX9::ClearWorld() {
  unsigned color = this->clearColor;
  if (this->fog.enabled == 1 && static_cast<CGameMode*>(g_modeMgr.GetGameMode())->scene->backgroundFog == 1) {
    color = this->BlendColor(this->clearColor, g_worldOverlayColor);
  }

  this->nativeDevice->Clear(0, nullptr, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, color, 1.0f, 0);
}

// OFFSET: 0x00565740
// FUNCTION: fn_02218
void CRendererDX9::DrawPrimitive(RenderCommand* primitive) {
  if (primitive->vertexCount <= 0) {
    return;
  }

  const auto type = static_cast<D3DPRIMITIVETYPE>(primitive->primitiveType);
  this->device->state.SetFVF(D3DFVF_XYZRHW | D3DFVF_DIFFUSE | D3DFVF_SPECULAR | D3DFVF_TEX1);

  if (primitive->indexCount) {
    this->nativeDevice->DrawIndexedPrimitiveUP(
      type, 0, primitive->vertexCount, primitive->indexCount / (type == D3DPT_TRIANGLELIST ? 3 : 2), primitive->indices, D3DFMT_INDEX16,
      primitive->vertices, sizeof(RenderVertex)
    );
  } else {
    this->nativeDevice->DrawPrimitiveUP(type, primitive->vertexCount / (type == D3DPT_TRIANGLELIST ? 3 : 2), primitive->vertices, sizeof(RenderVertex));
  }
}

// OFFSET: 0x005657D0
// FUNCTION: fn_02219
void CRendererDX9::DrawLightmappedPrimitive(LightmappedPrimitive* primitive) {
  if (primitive->vertexCount <= 0) {
    return;
  }

  const auto type = static_cast<D3DPRIMITIVETYPE>(primitive->primitiveType);
  this->device->state.SetFVF(D3DFVF_XYZRHW | D3DFVF_DIFFUSE | D3DFVF_SPECULAR | D3DFVF_TEX2);

  if (primitive->indexCount) {
    this->nativeDevice->DrawIndexedPrimitiveUP(
      type, 0, primitive->vertexCount, primitive->indexCount / (type == D3DPT_TRIANGLELIST ? 3 : 2), primitive->indices, D3DFMT_INDEX16,
      primitive->vertices, sizeof(LightmappedVertex)
    );
  } else {
    this->nativeDevice->DrawPrimitiveUP(type, primitive->vertexCount / (type == D3DPT_TRIANGLELIST ? 3 : 2), primitive->vertices, sizeof(LightmappedVertex));
  }
}

// INLINED
static bool operator<(const DepthPrimitive& left, const DepthPrimitive& right) {
  if (left.depth < right.depth) {
    return true;
  }

  if (right.depth < left.depth) {
    return false;
  }

  return left.depth == right.depth && std::less<RenderCommand*>{}(left.primitive, right.primitive);
}

// OFFSET: 0x005658F0
// FUNCTION: fn_02221
void CRendererDX9::DrawDepthQueue(bool sort, std::vector<DepthPrimitive>& queue) {
  if (sort) {
    std::sort(queue.begin(), queue.end());
  }

  for (const DepthPrimitive& entry : queue) {
    RenderCommand* command = entry.primitive;
    IDirect3DTexture9* nativeTexture = command->texture ? static_cast<CTextureDX9*>(command->texture)->NativeTexture() : nullptr;
    this->device->state.SetTexture(0, nativeTexture);
    this->DrawPrimitive(command);
  }
}

// OFFSET: 0x00565210
// FUNCTION: fn_02216
bool CRendererDX9::RenderToTexture(int width, int height, const std::list<RenderPrimitive*>& primitives, CTexture*& texture) {
  IDirect3DTexture9* nativeTexture = nullptr;
  if (texture) {
    nativeTexture = static_cast<CTextureDX9*>(texture)->NativeTexture();
  } else {
    if (FAILED(this->nativeDevice->CreateTexture(width, height, 1, D3DUSAGE_RENDERTARGET, D3DFMT_A8R8G8B8, D3DPOOL_DEFAULT, &nativeTexture, nullptr))) {
      return false;
    }

    CTextureDX9* created = new (std::nothrow) CTextureDX9(*this->device, nativeTexture);

    if (!created) {
      return false;
    }

    texture = created;
  }

  if (!nativeTexture) {
    return false;
  }

  CComPtr<IDirect3DSurface9> previous;
  CComPtr<IDirect3DSurface9> target;

  if (FAILED(nativeTexture->GetSurfaceLevel(0, &target)) || FAILED(this->nativeDevice->GetRenderTarget(0, &previous)) ||
      FAILED(this->nativeDevice->SetRenderTarget(0, target))) {
    return false;
  }

  {
    ScopedDeviceStateDX9 restore = this->device->SaveState();
    this->device->state.SetRenderState(D3DRS_ZENABLE, D3DZB_FALSE);
    this->device->state.SetRenderState(D3DRS_FOGENABLE, 0);
    this->device->state.SetRenderState(D3DRS_ALPHAREF, 1);
    this->device->state.SetRenderState(D3DRS_ALPHATESTENABLE, 1);
    this->device->state.SetRenderState(D3DRS_ALPHABLENDENABLE, 1);
    this->nativeDevice->Clear(0, nullptr, D3DCLEAR_TARGET, 0, 1.0f, 0);

    if (SUCCEEDED(this->nativeDevice->BeginScene())) {
      for (const auto& primitive : primitives) {
        IDirect3DTexture9* selected = primitive->texture ? static_cast<CTextureDX9*>(primitive->texture)->NativeTexture() : nullptr;
        this->device->state.SetTexture(0, selected);
        this->DrawPrimitive(primitive);
      }

      this->nativeDevice->EndScene();
    }
  }
  this->nativeDevice->SetRenderTarget(0, previous);

  return true;
}
