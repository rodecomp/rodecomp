#ifndef RAGNAROK_SOURCE_RENDER_DX9_D3DX9RUNTIME_H
#define RAGNAROK_SOURCE_RENDER_DX9_D3DX9RUNTIME_H

#include <d3d9.h>

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

#endif
