#include "Render/DX9/DeviceOwnerDX9.h"

// OFFSET: 0x0054BB10
// FUNCTION: fn_01912
bool DeviceOwnerDX9::CanAutoGenerateMipmaps(D3DFORMAT format) const {
  if (!this->direct3D || !(this->caps.Caps2 & D3DCAPS2_CANAUTOGENMIPMAP)) {
    return false;
  }

  return this->direct3D->CheckDeviceFormat(this->adapter, D3DDEVTYPE_HAL, this->presentation.BackBufferFormat, D3DUSAGE_AUTOGENMIPMAP, D3DRTYPE_TEXTURE, format) ==
         D3D_OK;
}
