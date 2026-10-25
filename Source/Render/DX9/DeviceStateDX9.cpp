#include "Render/DX9/DeviceStateDX9.h"

#include <cstring>

namespace {
  constexpr HRESULT noDevice = static_cast<HRESULT>(0x8200000Eu);
}

// OFFSET: 0x00550AF0
// FUNCTION: fn_01991
DeviceStateDX9::DeviceStateDX9() : device(nullptr), fvf(0) {
  std::memset(&this->render, 0, 1224);
}

// OFFSET: 0x00551500
// FUNCTION: fn_01997
void DeviceStateDX9::Initialize(IDirect3DDevice9* value) {
  this->device = value;
  value->GetFVF(&this->fvf);

  for (unsigned state = 0; state < 210; ++state) {
    DWORD result;
    if (SUCCEEDED(value->GetRenderState(static_cast<D3DRENDERSTATETYPE>(state), &result))) {
      this->render[state] = result;
    }
  }

  for (unsigned stage = 0; stage < 2; ++stage) {
    for (unsigned state = 0; state < 33; ++state) {
      DWORD result;
      if (SUCCEEDED(value->GetTextureStageState(stage, static_cast<D3DTEXTURESTAGESTATETYPE>(state), &result))) {
        this->textureStage[stage][state] = result;
      }
    }

    for (unsigned state = 0; state < 14; ++state) {
      DWORD result;
      if (SUCCEEDED(value->GetSamplerState(stage, static_cast<D3DSAMPLERSTATETYPE>(state), &result))) {
        this->sampler[stage][state] = result;
      }
    }
  }
}

// OFFSET: 0x0054C2A0
// FUNCTION: fn_01919
void DeviceStateDX9::SetInitialStates() {
  this->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
  this->SetRenderState(D3DRS_ZFUNC, D3DCMP_LESSEQUAL);
  this->SetRenderState(D3DRS_ALPHATESTENABLE, 1);
  this->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATEREQUAL);
  this->SetRenderState(D3DRS_ALPHAREF, 240);
  this->SetRenderState(D3DRS_ZENABLE, D3DZB_TRUE);
  this->SetRenderState(D3DRS_ZWRITEENABLE, 1);
  this->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
  this->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);

  this->SetTextureStageState(0, D3DTSS_TEXCOORDINDEX, 0);
  this->SetTextureStageState(0, D3DTSS_COLOROP, D3DTOP_MODULATE);
  this->SetTextureStageState(0, D3DTSS_COLORARG1, D3DTA_TEXTURE);
  this->SetTextureStageState(0, D3DTSS_COLORARG2, D3DTA_DIFFUSE);
  this->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_MODULATE);
  this->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);
  this->SetTextureStageState(0, D3DTSS_ALPHAARG2, D3DTA_DIFFUSE);
  this->SetTextureStageState(1, D3DTSS_TEXCOORDINDEX, 1);
  this->SetTextureStageState(1, D3DTSS_COLORARG1, D3DTA_TEXTURE);
  this->SetTextureStageState(1, D3DTSS_COLOROP, D3DTOP_DISABLE);
  this->SetTextureStageState(1, D3DTSS_COLORARG2, D3DTA_CURRENT);
  this->SetTextureStageState(1, D3DTSS_ALPHAOP, D3DTOP_DISABLE);

  this->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);
  this->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);
  this->SetSamplerState(0, D3DSAMP_MIPFILTER, D3DTEXF_LINEAR);
  this->SetSamplerState(1, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);
  this->SetSamplerState(1, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);
  this->SetSamplerState(1, D3DSAMP_MIPFILTER, D3DTEXF_LINEAR);
  this->SetSamplerState(0, D3DSAMP_ADDRESSU, D3DTADDRESS_CLAMP);
  this->SetSamplerState(0, D3DSAMP_ADDRESSV, D3DTADDRESS_CLAMP);
  this->SetSamplerState(1, D3DSAMP_ADDRESSU, D3DTADDRESS_BORDER);
  this->SetSamplerState(1, D3DSAMP_ADDRESSV, D3DTADDRESS_BORDER);
  this->SetSamplerState(1, D3DSAMP_BORDERCOLOR, 0xFF000000u);
}

// OFFSET: 0x00550C00
// FUNCTION: fn_01993
void DeviceStateDX9::Begin() {
  for (unsigned stage = 0; stage < 2; ++stage) {
    this->textures[stage] = nullptr;
    this->device->SetTexture(stage, nullptr);
  }
}

// OFFSET: 0x00550F50
// FUNCTION: fn_01994
void DeviceStateDX9::End() {
  this->textures[0] = this->textures[1] = nullptr;
}

// OFFSET: 0x005517E0
// FUNCTION: fn_01999
HRESULT DeviceStateDX9::SetFVF(DWORD value) {
  if (!this->device) {
    return noDevice;
  }

  if (this->fvf == value) {
    return D3D_OK;
  }

  HRESULT result = this->device->SetFVF(value);
  if (SUCCEEDED(result)) {
    this->fvf = value;
  }

  return result;
}

// OFFSET: 0x00551820
// FUNCTION: fn_02000
HRESULT DeviceStateDX9::SetRenderState(D3DRENDERSTATETYPE state, DWORD value) {
  if (!this->device) {
    return noDevice;
  }

  if (this->render[state] == value) {
    return D3D_OK;
  }

  HRESULT result = this->device->SetRenderState(state, value);
  if (SUCCEEDED(result)) {
    this->render[state] = value;
  }

  return result;
}

// OFFSET: 0x00551870
// FUNCTION: fn_02001
HRESULT DeviceStateDX9::SetSamplerState(UINT stage, D3DSAMPLERSTATETYPE state, DWORD value) {
  if (!this->device) {
    return noDevice;
  }

  if (stage >= 2) {
    return D3DERR_INVALIDCALL;
  }

  if (this->sampler[stage][state] == value) {
    return D3D_OK;
  }

  HRESULT result = this->device->SetSamplerState(stage, state, value);
  if (SUCCEEDED(result)) {
    this->sampler[stage][state] = value;
  }

  return result;
}

// OFFSET: 0x005518E0
// FUNCTION: fn_02002
HRESULT DeviceStateDX9::SetTexture(UINT stage, IDirect3DBaseTexture9* texture) {
  if (!this->device) {
    return noDevice;
  }

  if (stage >= 2) {
    return D3DERR_INVALIDCALL;
  }

  if (this->textures[stage] == texture) {
    return D3D_OK;
  }

  HRESULT result = this->device->SetTexture(stage, texture);
  if (SUCCEEDED(result)) {
    this->textures[stage] = texture;
  }

  return result;
}

// OFFSET: 0x00551940
// FUNCTION: fn_02003
HRESULT DeviceStateDX9::SetTextureStageState(UINT stage, D3DTEXTURESTAGESTATETYPE state, DWORD value) {
  if (!this->device) {
    return noDevice;
  }

  if (stage >= 2) {
    return D3DERR_INVALIDCALL;
  }

  if (this->textureStage[stage][state] == value) {
    return D3D_OK;
  }

  HRESULT result = this->device->SetTextureStageState(stage, state, value);
  if (SUCCEEDED(result)) {
    this->textureStage[stage][state] = value;
  }

  return result;
}

// OFFSET: 0x00550B20
// FUNCTION: fn_01992
ScopedDeviceStateDX9::ScopedDeviceStateDX9(DeviceStateDX9& state) : m_state(&state) {
  std::memcpy(this->m_render, state.render, sizeof(this->m_render));
  std::memcpy(this->m_textureStage, state.textureStage, sizeof(this->m_textureStage));
  std::memcpy(this->m_sampler, state.sampler, sizeof(this->m_sampler));
}

// OFFSET: 0x005516A0
// FUNCTION: fn_01998
ScopedDeviceStateDX9::~ScopedDeviceStateDX9() {
  for (unsigned state = 0; state < 210; ++state) {
    if (this->m_render[state] != this->m_state->render[state] && this->m_state->device) {
      this->m_state->SetRenderState(static_cast<D3DRENDERSTATETYPE>(state), this->m_render[state]);
    }
  }

  for (unsigned stage = 0; stage < 2; ++stage) {
    for (unsigned state = 0; state < 33; ++state) {
      if (this->m_textureStage[stage][state] != this->m_state->textureStage[stage][state] && this->m_state->device) {
        this->m_state->SetTextureStageState(stage, static_cast<D3DTEXTURESTAGESTATETYPE>(state), this->m_textureStage[stage][state]);
      }
    }

    for (unsigned state = 0; state < 14; ++state) {
      if (this->m_sampler[stage][state] != this->m_state->sampler[stage][state] && this->m_state->device) {
        this->m_state->SetSamplerState(stage, static_cast<D3DSAMPLERSTATETYPE>(state), this->m_sampler[stage][state]);
      }
    }
  }
}
