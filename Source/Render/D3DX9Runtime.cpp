#include "Render/D3DX9Runtime.h"

#include <system_error>

D3DX9Runtime::D3DX9Runtime() : m_module(LoadLibraryA("D3DX9_43.dll")) {
  if (!this->m_module) {
    throw std::system_error(GetLastError(), std::system_category(), "D3DX9_43.dll");
  }
  this->filterTexture =
    reinterpret_cast<decltype(this->filterTexture)>(GetProcAddress(this->m_module, "D3DXFilterTexture"));
  this->loadSurfaceFromMemory = reinterpret_cast<decltype(this->loadSurfaceFromMemory)>(
    GetProcAddress(this->m_module, "D3DXLoadSurfaceFromMemory")
  );
  this->loadSurfaceFromSurface = reinterpret_cast<decltype(this->loadSurfaceFromSurface)>(
    GetProcAddress(this->m_module, "D3DXLoadSurfaceFromSurface")
  );
  this->perspectiveFovLH =
    reinterpret_cast<decltype(this->perspectiveFovLH)>(GetProcAddress(this->m_module, "D3DXMatrixPerspectiveFovLH"));
  this->saveSurfaceToFileA =
    reinterpret_cast<decltype(this->saveSurfaceToFileA)>(GetProcAddress(this->m_module, "D3DXSaveSurfaceToFileA"));
  if (
    !this->filterTexture || !this->loadSurfaceFromMemory || !this->loadSurfaceFromSurface || !this->perspectiveFovLH ||
    !this->saveSurfaceToFileA
  ) {
    FreeLibrary(this->m_module);
    throw std::system_error(ERROR_PROC_NOT_FOUND, std::system_category(), "D3DX9_43 exports");
  }
}

D3DX9Runtime::~D3DX9Runtime() {
  FreeLibrary(this->m_module);
}
