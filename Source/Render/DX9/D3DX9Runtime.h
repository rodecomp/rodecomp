#ifndef RAGNAROK_SOURCE_RENDER_DX9_D3DX9RUNTIME_H
#define RAGNAROK_SOURCE_RENDER_DX9_D3DX9RUNTIME_H

#include <d3d9.h>

extern "C" {
HRESULT WINAPI D3DXFilterTexture(IDirect3DBaseTexture9* texture, const PALETTEENTRY* palette, UINT level, DWORD filter);
HRESULT WINAPI D3DXLoadSurfaceFromMemory(
  IDirect3DSurface9* destination, const PALETTEENTRY* destinationPalette, const RECT* destinationRect, const void* source, D3DFORMAT format,
  UINT pitch, const PALETTEENTRY* sourcePalette, const RECT* sourceRect, DWORD filter, D3DCOLOR colorKey
);
HRESULT WINAPI D3DXLoadSurfaceFromSurface(
  IDirect3DSurface9* destination, const PALETTEENTRY* destinationPalette, const RECT* destinationRect, IDirect3DSurface9* source,
  const PALETTEENTRY* sourcePalette, const RECT* sourceRect, DWORD filter, D3DCOLOR colorKey
);
D3DMATRIX* WINAPI D3DXMatrixPerspectiveFovLH(D3DMATRIX* output, float fovy, float aspect, float nearPlane, float farPlane);
HRESULT WINAPI D3DXSaveSurfaceToFileA(const char* filename, int format, IDirect3DSurface9* surface, const PALETTEENTRY* palette, const RECT* rectangle);
}

#endif
