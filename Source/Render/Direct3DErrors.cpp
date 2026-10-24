#include "Render/Direct3DErrors.h"

namespace {
  struct Error {
    HRESULT value;
    const char* name;
    const char* description;
  };

  // Result-name and description tables.
  constexpr Error errors[] = {
    {static_cast<HRESULT>(0), "D3D_OK", "OK"},
    {static_cast<HRESULT>(141953135), "D3DOK_NOAUTOGEN",
     "This is a success code. However, the autogeneration of mipmaps is not supported for this format."},
    {static_cast<HRESULT>(141953141), "S_NOT_RESIDENT", "Resource not resident in memory."},
    {static_cast<HRESULT>(141953142), "S_RESIDENT_IN_SHARED_MEMORY", "Resource resident in shared memory."},
    {static_cast<HRESULT>(141953143), "S_PRESENT_MODE_CHANGED", "The desktop display mode has been changed."},
    {static_cast<HRESULT>(141953144), "S_PRESENT_OCCLUDED",
     "Client window is occluded. (minimized or other fullscreen)"},
    {static_cast<HRESULT>(-2005532292), "D3DERR_OUTOFVIDEOMEMORY",
     "Direct3D does not have enough display memory to perform the operation."},
    {static_cast<HRESULT>(-2005532132), "D3DERR_WASSTILLDRAWING",
     "The previous blit operation that is transferring information to or from this surface is incomplete."},
    {static_cast<HRESULT>(-2005530600), "D3DERR_WRONGTEXTUREFORMAT",
     "The pixel format of the texture surface is not valid."},
    {static_cast<HRESULT>(-2005530599), "D3DERR_UNSUPPORTEDCOLOROPERATION",
     "The device does not support a specified texture-blending operation for color values."},
    {static_cast<HRESULT>(-2005530598), "D3DERR_UNSUPPORTEDCOLORARG",
     "The device does not support a specified texture-blending argument for color values."},
    {static_cast<HRESULT>(-2005530597), "D3DERR_UNSUPPORTEDALPHAOPERATION",
     "The device does not support a specified texture-blending operation for the alpha channel."},
    {static_cast<HRESULT>(-2005530596), "D3DERR_UNSUPPORTEDALPHAARG",
     "The device does not support a specified texture-blending argument for the alpha channel."},
    {static_cast<HRESULT>(-2005530595), "D3DERR_TOOMANYOPERATIONS",
     "The application is requesting more texture-filtering operations than the device supports."},
    {static_cast<HRESULT>(-2005530594), "D3DERR_CONFLICTINGTEXTUREFILTER",
     "The current texture filters cannot be used together."},
    {static_cast<HRESULT>(-2005530593), "D3DERR_UNSUPPORTEDFACTORVALUE",
     "The device does not support the specified texture factor value."},
    {static_cast<HRESULT>(-2005530591), "D3DERR_CONFLICTINGRENDERSTATE",
     "The currently set render states cannot be used together."},
    {static_cast<HRESULT>(-2005530590), "D3DERR_UNSUPPORTEDTEXTUREFILTER",
     "The device does not support the specified texture filter."},
    {static_cast<HRESULT>(-2005530586), "D3DERR_CONFLICTINGTEXTUREPALETTE",
     "The current textures cannot be used simultaneously."},
    {static_cast<HRESULT>(-2005530585), "D3DERR_DRIVERINTERNALERROR", "Internal driver error."},
    {static_cast<HRESULT>(-2005530522), "D3DERR_NOTFOUND", "The requested item was not found."},
    {static_cast<HRESULT>(-2005530521), "D3DERR_MOREDATA",
     "There is more data available than the specified buffer size can hold."},
    {static_cast<HRESULT>(-2005530520), "D3DERR_DEVICELOST",
     "The device has been lost but cannot be reset at this time. "},
    {static_cast<HRESULT>(-2005530519), "D3DERR_DEVICENOTRESET",
     "The device has been lost but can be reset at this time."},
    {static_cast<HRESULT>(-2005530518), "D3DERR_NOTAVAILABLE", "Not available"},
    {static_cast<HRESULT>(-2005530517), "D3DERR_INVALIDDEVICE", "The requested device type is not valid."},
    {static_cast<HRESULT>(-2005530516), "D3DERR_INVALIDCALL", "The method call is invalid."},
    {static_cast<HRESULT>(-2005530515), "D3DERR_DRIVERINVALIDCALL", "Driver invalid call"},
    {static_cast<HRESULT>(-2005530512), "D3DERR_DEVICEREMOVED", "The hardware adapter has been removed."},
    {static_cast<HRESULT>(-2005530508), "D3DERR_DEVICEHUNG", "Hardware adapter reset by the OS."},
    {static_cast<HRESULT>(-2005530501), "D3DERR_UNSUPPORTEDOVERLAY",
     "The device does not support overlay for the specified size or display mode."},
    {static_cast<HRESULT>(-2005530500), "D3DERR_UNSUPPORTEDOVERLAYFORMAT",
     "The device does not support overlay for the specified surface format."},
    {static_cast<HRESULT>(-2005530499), "D3DERR_CANNOTPROTECTCONTENT", "Contect protection not available."},
    {static_cast<HRESULT>(-2005530498), "D3DERR_UNSUPPORTEDCRYPTO",
     "The specified cryptographic algorithm is not supported."},
    {static_cast<HRESULT>(-2005530492), "D3DERR_PRESENT_STATISTICS_DISJOINT", "Presentation statistics are disjoint."},
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
