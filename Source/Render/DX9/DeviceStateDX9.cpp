#include "Render/DX9/DeviceOwnerDX9.h"

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

  for (int state = 0; state < 210; ++state) {
    DWORD result;
    if (SUCCEEDED(this->device->GetRenderState(static_cast<D3DRENDERSTATETYPE>(state), &result))) {
      this->render[state] = result;
    }
  }

  for (int stage = 0; stage < 2; ++stage) {
    for (int state = 0; state < 33; ++state) {
      DWORD result;
      if (SUCCEEDED(this->device->GetTextureStageState(stage, static_cast<D3DTEXTURESTAGESTATETYPE>(state), &result))) {
        this->textureStage[stage][state] = result;
      }
    }

    for (int state = 0; state < 14; ++state) {
      DWORD result;
      if (SUCCEEDED(this->device->GetSamplerState(stage, static_cast<D3DSAMPLERSTATETYPE>(state), &result))) {
        this->sampler[stage][state] = result;
      }
    }
  }
}

// OFFSET: 0x00550C00
// FUNCTION: fn_01993
void DeviceStateDX9::Begin() {
  for (int stage = 0; stage < 2; ++stage) {
    this->textures[stage] = nullptr;
    this->device->SetTexture(stage, nullptr);
  }
}

// OFFSET: 0x00550F50
// FUNCTION: fn_01994
void DeviceStateDX9::End() {
  this->textures[0] = nullptr;
  this->textures[1] = nullptr;
}

// OFFSET: 0x005517E0
// FUNCTION: fn_01999
HRESULT DeviceStateDX9::SetFVF(DWORD value) {
  if (!this->device) {
    return noDevice;
  }

  if (value == this->fvf) {
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
void ScopedDeviceStateDX9::Save() {
  memcpy_s(this->m_render, sizeof(this->m_render), this->m_state->render, sizeof(this->m_render));
  memcpy_s(this->m_textureStage, sizeof(this->m_textureStage), this->m_state->textureStage, sizeof(this->m_textureStage));
  memcpy_s(this->m_sampler, sizeof(this->m_sampler), this->m_state->sampler, sizeof(this->m_sampler));
}

// OFFSET: 0x005516A0
// FUNCTION: fn_01998
ScopedDeviceStateDX9::~ScopedDeviceStateDX9() {
  for (int state = 0; state < 210; ++state) {
    if (this->m_render[state] != this->m_state->render[state]) {
      this->m_state->SetRenderState(static_cast<D3DRENDERSTATETYPE>(state), this->m_render[state]);
    }
  }

  for (int stage = 0; stage < 2; ++stage) {
    for (int state = 0; state < 33; ++state) {
      if (this->m_textureStage[stage][state] != this->m_state->textureStage[stage][state]) {
        this->m_state->SetTextureStageState(stage, static_cast<D3DTEXTURESTAGESTATETYPE>(state), this->m_textureStage[stage][state]);
      }
    }

    for (int state = 0; state < 14; ++state) {
      if (this->m_sampler[stage][state] != this->m_state->sampler[stage][state]) {
        this->m_state->SetSamplerState(stage, static_cast<D3DSAMPLERSTATETYPE>(state), this->m_sampler[stage][state]);
      }
    }
  }
}
