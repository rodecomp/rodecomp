#include "Render/BlendColor.h"
#include "Render/DX9/DeviceStateDX9.h"
#include "Render/Primitives/LightmappedPrimitive.h"
#include "Render/Primitives/RenderPrimitive.h"
#include "Render/Primitives/RenderQueues.h"
#include "Render/DX9/RendererDX9Operations.h"
#include "Render/DX9/TextureDX9.h"

#include <algorithm>
#include <bit>
#include <functional>
#include <new>

// OFFSET: 0x00568460
// FUNCTION: fn_02240
void RendererDX9Operations::SetCamera(const float*, const float*, const float*) {
}

// OFFSET: 0x00568370
// FUNCTION: fn_02239
void RendererDX9Operations::SetLighting(const float* direction, const float* diffuse, const float* ambient) {
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
  light.Ambient.b = ambient[2];
  light.Ambient.a = 255.0f;

  this->context.device->SetLight(0, &light);
  this->context.device->LightEnable(0, TRUE);
}

// OFFSET: 0x00565150
// FUNCTION: fn_02214
void RendererDX9Operations::ClearDepth(int clearColor) {
  this->context.device->Clear(0, nullptr, clearColor ? D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER : D3DCLEAR_ZBUFFER, 0xFF000000u, 1.0f, 0);
}

// OFFSET: 0x005651A0
// FUNCTION: fn_02215
void RendererDX9Operations::ClearWorld(unsigned color, int mode, unsigned char worldFlag, unsigned overlay) {
  if (mode == 1 && worldFlag == 1) {
    color = BlendColor(color, overlay);
  }

  this->context.device->Clear(0, nullptr, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, color, 1.0f, 0);
}

// OFFSET: 0x00565740
// FUNCTION: fn_02218
void RendererDX9Operations::DrawPrimitive(RenderCommand* primitive) {
  if (primitive->vertexCount <= 0) {
    return;
  }

  this->state.SetFVF(D3DFVF_XYZRHW | D3DFVF_DIFFUSE | D3DFVF_SPECULAR | D3DFVF_TEX1);
  const auto type = static_cast<D3DPRIMITIVETYPE>(primitive->primitiveType);
  const unsigned divisor = type == D3DPT_TRIANGLELIST ? 3 : 2;

  if (primitive->indexCount) {
    this->context.device->DrawIndexedPrimitiveUP(
      type, 0, primitive->vertexCount, primitive->indexCount / divisor, primitive->indices, D3DFMT_INDEX16, primitive->vertices, sizeof(RenderVertex)
    );
  } else {
    this->context.device->DrawPrimitiveUP(type, primitive->vertexCount / divisor, primitive->vertices, sizeof(RenderVertex));
  }
}

// OFFSET: 0x005657D0
// FUNCTION: fn_02219
void RendererDX9Operations::DrawLightmappedPrimitive(LightmappedPrimitive* primitive) {
  if (primitive->vertexCount <= 0) {
    return;
  }

  this->state.SetFVF(D3DFVF_XYZRHW | D3DFVF_DIFFUSE | D3DFVF_SPECULAR | D3DFVF_TEX2);
  const auto type = static_cast<D3DPRIMITIVETYPE>(primitive->primitiveType);
  const unsigned divisor = type == D3DPT_TRIANGLELIST ? 3 : 2;

  if (primitive->indexCount) {
    this->context.device->DrawIndexedPrimitiveUP(
      type, 0, primitive->vertexCount, primitive->indexCount / divisor, primitive->indices, D3DFMT_INDEX16, primitive->vertices, sizeof(LightmappedVertex)
    );
  } else {
    this->context.device->DrawPrimitiveUP(type, primitive->vertexCount / divisor, primitive->vertices, sizeof(LightmappedVertex));
  }
}

// OFFSET: 0x005658F0
// FUNCTION: fn_02221
void RendererDX9Operations::DrawDepthQueue(std::vector<DepthPrimitive>& queue, bool sort) {
  if (sort) {
    std::sort(queue.begin(), queue.end(), [](const DepthPrimitive& left, const DepthPrimitive& right) {
      if (left.depth < right.depth) {
        return true;
      }

      if (right.depth < left.depth) {
        return false;
      }

      return left.depth == right.depth && std::less<RenderCommand*>{}(left.primitive, right.primitive);
    });
  }

  for (const DepthPrimitive& entry : queue) {
    RenderCommand* command = entry.primitive;
    this->state.SetTexture(0, command->texture ? static_cast<CTextureDX9*>(command->texture)->NativeTexture() : nullptr);
    this->DrawPrimitive(command);
  }
}

// OFFSET: 0x00565210
// FUNCTION: fn_02216
bool RendererDX9Operations::RenderToTexture(int width, int height, const std::list<RenderPrimitive*>& primitives, CTexture*& texture) {
  IDirect3DTexture9* nativeTexture = nullptr;
  if (texture) {
    nativeTexture = static_cast<CTextureDX9*>(texture)->NativeTexture();
  } else {
    if (FAILED(this->context.device->CreateTexture(width, height, 1, D3DUSAGE_RENDERTARGET, D3DFMT_A8R8G8B8, D3DPOOL_DEFAULT, &nativeTexture, nullptr))) {
      return false;
    }

    CTextureDX9* created = new (std::nothrow) CTextureDX9(this->context, nativeTexture);

    if (!created) {
      return false;
    }

    texture = created;
  }

  if (!nativeTexture) {
    return false;
  }

  IDirect3DSurface9* previous = nullptr;
  IDirect3DSurface9* target = nullptr;
  bool result = false;

  if (SUCCEEDED(nativeTexture->GetSurfaceLevel(0, &target)) && SUCCEEDED(this->context.device->GetRenderTarget(0, &previous)) &&
      SUCCEEDED(this->context.device->SetRenderTarget(0, target))) {
    {
      ScopedDeviceStateDX9 restore(this->state);
      this->state.SetRenderState(D3DRS_ZENABLE, D3DZB_FALSE);
      this->state.SetRenderState(D3DRS_FOGENABLE, 0);
      this->state.SetRenderState(D3DRS_ALPHAREF, 1);
      this->state.SetRenderState(D3DRS_ALPHATESTENABLE, 1);
      this->state.SetRenderState(D3DRS_ALPHABLENDENABLE, 1);
      this->context.device->Clear(0, nullptr, D3DCLEAR_TARGET, 0, 1.0f, 0);

      if (SUCCEEDED(this->context.device->BeginScene())) {
        for (RenderPrimitive* primitive : primitives) {
          this->state.SetTexture(0, primitive->texture ? static_cast<CTextureDX9*>(primitive->texture)->NativeTexture() : nullptr);
          this->DrawPrimitive(primitive);
        }

        this->context.device->EndScene();
      }
    }
    this->context.device->SetRenderTarget(0, previous);

    result = true;
  }

  if (target) {
    target->Release();
  }

  if (previous) {
    previous->Release();
  }

  return result;
}
