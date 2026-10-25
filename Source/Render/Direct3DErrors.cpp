#include "Render/Direct3DErrors.h"

#include <d3d9.h>

namespace {
  struct Error {
    HRESULT value;
    const char* name;
    const char* description;
  };

  constexpr Error errors[] = {
    {D3D_OK, "D3D_OK", "OK"},
    {D3DOK_NOAUTOGEN, "D3DOK_NOAUTOGEN", "This is a success code. However, the autogeneration of mipmaps is not supported for this format."},
    {S_NOT_RESIDENT, "S_NOT_RESIDENT", "Resource not resident in memory."},
    {S_RESIDENT_IN_SHARED_MEMORY, "S_RESIDENT_IN_SHARED_MEMORY", "Resource resident in shared memory."},
    {S_PRESENT_MODE_CHANGED, "S_PRESENT_MODE_CHANGED", "The desktop display mode has been changed."},
    {S_PRESENT_OCCLUDED, "S_PRESENT_OCCLUDED", "Client window is occluded. (minimized or other fullscreen)"},
    {D3DERR_OUTOFVIDEOMEMORY, "D3DERR_OUTOFVIDEOMEMORY", "Direct3D does not have enough display memory to perform the operation."},
    {D3DERR_WASSTILLDRAWING, "D3DERR_WASSTILLDRAWING", "The previous blit operation that is transferring information to or from this surface is incomplete."},
    {D3DERR_WRONGTEXTUREFORMAT, "D3DERR_WRONGTEXTUREFORMAT", "The pixel format of the texture surface is not valid."},
    {D3DERR_UNSUPPORTEDCOLOROPERATION, "D3DERR_UNSUPPORTEDCOLOROPERATION",
     "The device does not support a specified texture-blending operation for color values."},
    {D3DERR_UNSUPPORTEDCOLORARG, "D3DERR_UNSUPPORTEDCOLORARG", "The device does not support a specified texture-blending argument for color values."},
    {D3DERR_UNSUPPORTEDALPHAOPERATION, "D3DERR_UNSUPPORTEDALPHAOPERATION",
     "The device does not support a specified texture-blending operation for the alpha channel."},
    {D3DERR_UNSUPPORTEDALPHAARG, "D3DERR_UNSUPPORTEDALPHAARG", "The device does not support a specified texture-blending argument for the alpha channel."},
    {D3DERR_TOOMANYOPERATIONS, "D3DERR_TOOMANYOPERATIONS", "The application is requesting more texture-filtering operations than the device supports."},
    {D3DERR_CONFLICTINGTEXTUREFILTER, "D3DERR_CONFLICTINGTEXTUREFILTER", "The current texture filters cannot be used together."},
    {D3DERR_UNSUPPORTEDFACTORVALUE, "D3DERR_UNSUPPORTEDFACTORVALUE", "The device does not support the specified texture factor value."},
    {D3DERR_CONFLICTINGRENDERSTATE, "D3DERR_CONFLICTINGRENDERSTATE", "The currently set render states cannot be used together."},
    {D3DERR_UNSUPPORTEDTEXTUREFILTER, "D3DERR_UNSUPPORTEDTEXTUREFILTER", "The device does not support the specified texture filter."},
    {D3DERR_CONFLICTINGTEXTUREPALETTE, "D3DERR_CONFLICTINGTEXTUREPALETTE", "The current textures cannot be used simultaneously."},
    {D3DERR_DRIVERINTERNALERROR, "D3DERR_DRIVERINTERNALERROR", "Internal driver error."},
    {D3DERR_NOTFOUND, "D3DERR_NOTFOUND", "The requested item was not found."},
    {D3DERR_MOREDATA, "D3DERR_MOREDATA", "There is more data available than the specified buffer size can hold."},
    {D3DERR_DEVICELOST, "D3DERR_DEVICELOST", "The device has been lost but cannot be reset at this time. "},
    {D3DERR_DEVICENOTRESET, "D3DERR_DEVICENOTRESET", "The device has been lost but can be reset at this time."},
    {D3DERR_NOTAVAILABLE, "D3DERR_NOTAVAILABLE", "Not available"},
    {D3DERR_INVALIDDEVICE, "D3DERR_INVALIDDEVICE", "The requested device type is not valid."},
    {D3DERR_INVALIDCALL, "D3DERR_INVALIDCALL", "The method call is invalid."},
    {D3DERR_DRIVERINVALIDCALL, "D3DERR_DRIVERINVALIDCALL", "Driver invalid call"},
    {D3DERR_DEVICEREMOVED, "D3DERR_DEVICEREMOVED", "The hardware adapter has been removed."},
    {D3DERR_DEVICEHUNG, "D3DERR_DEVICEHUNG", "Hardware adapter reset by the OS."},
    {D3DERR_UNSUPPORTEDOVERLAY, "D3DERR_UNSUPPORTEDOVERLAY", "The device does not support overlay for the specified size or display mode."},
    {D3DERR_UNSUPPORTEDOVERLAYFORMAT, "D3DERR_UNSUPPORTEDOVERLAYFORMAT", "The device does not support overlay for the specified surface format."},
    {D3DERR_CANNOTPROTECTCONTENT, "D3DERR_CANNOTPROTECTCONTENT", "Contect protection not available."},
    {D3DERR_UNSUPPORTEDCRYPTO, "D3DERR_UNSUPPORTEDCRYPTO", "The specified cryptographic algorithm is not supported."},
    {D3DERR_PRESENT_STATISTICS_DISJOINT, "D3DERR_PRESENT_STATISTICS_DISJOINT", "Presentation statistics are disjoint."},
  };
}

// OFFSET: 0x005511D0
// FUNCTION: fn_01996
const char* Direct3DErrorName(HRESULT value) {
  for (const Error& error : errors) {
    if (error.value == value) {
      return error.name;
    }
  }

  return "Unknown error.";
}

// OFFSET: 0x00550F70
// FUNCTION: fn_01995
const char* Direct3DErrorDescription(HRESULT value) {
  for (const Error& error : errors) {
    if (error.value == value) {
      return error.description;
    }
  }

  return "Unknown error.";
}
