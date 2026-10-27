#ifndef RAGNAROK_SOURCE_RENDER_DX9_D3DX9RUNTIME_H
#define RAGNAROK_SOURCE_RENDER_DX9_D3DX9RUNTIME_H

#include <d3d9.h>
#include <system_error>

class D3DX9Runtime {
public:
  D3DX9Runtime();
  ~D3DX9Runtime();
  D3DX9Runtime(const D3DX9Runtime&) = delete;
  D3DX9Runtime& operator=(const D3DX9Runtime&) = delete;

  HRESULT(WINAPI* filterTexture)(IDirect3DBaseTexture9*, const PALETTEENTRY*, UINT, DWORD);
  HRESULT(WINAPI* loadSurfaceFromMemory)(
    IDirect3DSurface9*, const PALETTEENTRY*, const RECT*, const void*, D3DFORMAT, UINT, const PALETTEENTRY*, const RECT*, DWORD, D3DCOLOR
  );
  HRESULT(WINAPI* loadSurfaceFromSurface)(
    IDirect3DSurface9*, const PALETTEENTRY*, const RECT*, IDirect3DSurface9*, const PALETTEENTRY*, const RECT*, DWORD, D3DCOLOR
  );
  D3DMATRIX*(WINAPI* perspectiveFovLH)(D3DMATRIX*, float, float, float, float);
  HRESULT(WINAPI* saveSurfaceToFileA)(const char*, int, IDirect3DSurface9*, const PALETTEENTRY*, const RECT*);

private:
  HMODULE m_module;
};

// INLINED
inline D3DX9Runtime::D3DX9Runtime() : m_module(LoadLibraryA("D3DX9_43.dll")) {

  if (!this->m_module) {
    throw std::system_error(GetLastError(), std::system_category(), "D3DX9_43.dll");
  }

  this->filterTexture = reinterpret_cast<decltype(this->filterTexture)>(GetProcAddress(this->m_module, "D3DXFilterTexture"));
  this->loadSurfaceFromMemory = reinterpret_cast<decltype(this->loadSurfaceFromMemory)>(GetProcAddress(this->m_module, "D3DXLoadSurfaceFromMemory"));
  this->loadSurfaceFromSurface = reinterpret_cast<decltype(this->loadSurfaceFromSurface)>(GetProcAddress(this->m_module, "D3DXLoadSurfaceFromSurface"));
  this->perspectiveFovLH = reinterpret_cast<decltype(this->perspectiveFovLH)>(GetProcAddress(this->m_module, "D3DXMatrixPerspectiveFovLH"));
  this->saveSurfaceToFileA = reinterpret_cast<decltype(this->saveSurfaceToFileA)>(GetProcAddress(this->m_module, "D3DXSaveSurfaceToFileA"));

  if (!this->filterTexture || !this->loadSurfaceFromMemory || !this->loadSurfaceFromSurface || !this->perspectiveFovLH || !this->saveSurfaceToFileA) {
    FreeLibrary(this->m_module);
    throw std::system_error(ERROR_PROC_NOT_FOUND, std::system_category(), "D3DX9_43 exports");
  }
}

// INLINED
inline D3DX9Runtime::~D3DX9Runtime() {
  FreeLibrary(this->m_module);
}

#endif
