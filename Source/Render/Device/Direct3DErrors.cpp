#include "Render/Device/Direct3DErrors.h"

#include <d3d9.h>

// OFFSET: 0x005511D0
// FUNCTION: fn_01996
const char* Direct3DErrorName(HRESULT value) {
  switch (value) {
  case D3D_OK:
    return "D3D_OK";
  case D3DOK_NOAUTOGEN:
    return "D3DOK_NOAUTOGEN";
  case S_NOT_RESIDENT:
    return "S_NOT_RESIDENT";
  case S_RESIDENT_IN_SHARED_MEMORY:
    return "S_RESIDENT_IN_SHARED_MEMORY";
  case S_PRESENT_MODE_CHANGED:
    return "S_PRESENT_MODE_CHANGED";
  case S_PRESENT_OCCLUDED:
    return "S_PRESENT_OCCLUDED";
  case D3DERR_OUTOFVIDEOMEMORY:
    return "D3DERR_OUTOFVIDEOMEMORY";
  case D3DERR_WASSTILLDRAWING:
    return "D3DERR_WASSTILLDRAWING";
  case D3DERR_WRONGTEXTUREFORMAT:
    return "D3DERR_WRONGTEXTUREFORMAT";
  case D3DERR_UNSUPPORTEDCOLOROPERATION:
    return "D3DERR_UNSUPPORTEDCOLOROPERATION";
  case D3DERR_UNSUPPORTEDCOLORARG:
    return "D3DERR_UNSUPPORTEDCOLORARG";
  case D3DERR_UNSUPPORTEDALPHAOPERATION:
    return "D3DERR_UNSUPPORTEDALPHAOPERATION";
  case D3DERR_UNSUPPORTEDALPHAARG:
    return "D3DERR_UNSUPPORTEDALPHAARG";
  case D3DERR_TOOMANYOPERATIONS:
    return "D3DERR_TOOMANYOPERATIONS";
  case D3DERR_CONFLICTINGTEXTUREFILTER:
    return "D3DERR_CONFLICTINGTEXTUREFILTER";
  case D3DERR_UNSUPPORTEDFACTORVALUE:
    return "D3DERR_UNSUPPORTEDFACTORVALUE";
  case D3DERR_CONFLICTINGRENDERSTATE:
    return "D3DERR_CONFLICTINGRENDERSTATE";
  case D3DERR_UNSUPPORTEDTEXTUREFILTER:
    return "D3DERR_UNSUPPORTEDTEXTUREFILTER";
  case D3DERR_CONFLICTINGTEXTUREPALETTE:
    return "D3DERR_CONFLICTINGTEXTUREPALETTE";
  case D3DERR_DRIVERINTERNALERROR:
    return "D3DERR_DRIVERINTERNALERROR";
  case D3DERR_NOTFOUND:
    return "D3DERR_NOTFOUND";
  case D3DERR_MOREDATA:
    return "D3DERR_MOREDATA";
  case D3DERR_DEVICELOST:
    return "D3DERR_DEVICELOST";
  case D3DERR_DEVICENOTRESET:
    return "D3DERR_DEVICENOTRESET";
  case D3DERR_NOTAVAILABLE:
    return "D3DERR_NOTAVAILABLE";
  case D3DERR_INVALIDDEVICE:
    return "D3DERR_INVALIDDEVICE";
  case D3DERR_INVALIDCALL:
    return "D3DERR_INVALIDCALL";
  case D3DERR_DRIVERINVALIDCALL:
    return "D3DERR_DRIVERINVALIDCALL";
  case D3DERR_DEVICEREMOVED:
    return "D3DERR_DEVICEREMOVED";
  case D3DERR_DEVICEHUNG:
    return "D3DERR_DEVICEHUNG";
  case D3DERR_UNSUPPORTEDOVERLAY:
    return "D3DERR_UNSUPPORTEDOVERLAY";
  case D3DERR_UNSUPPORTEDOVERLAYFORMAT:
    return "D3DERR_UNSUPPORTEDOVERLAYFORMAT";
  case D3DERR_CANNOTPROTECTCONTENT:
    return "D3DERR_CANNOTPROTECTCONTENT";
  case D3DERR_UNSUPPORTEDCRYPTO:
    return "D3DERR_UNSUPPORTEDCRYPTO";
  case D3DERR_PRESENT_STATISTICS_DISJOINT:
    return "D3DERR_PRESENT_STATISTICS_DISJOINT";
  default:
    return "Unknown error.";
  }
}

// OFFSET: 0x00550F70
// FUNCTION: fn_01995
const char* Direct3DErrorDescription(HRESULT value) {
  switch (value) {
  case D3D_OK:
    return "OK";
  case D3DOK_NOAUTOGEN:
    return "This is a success code. However, the autogeneration of mipmaps is not supported for this format.";
  case S_NOT_RESIDENT:
    return "Resource not resident in memory.";
  case S_RESIDENT_IN_SHARED_MEMORY:
    return "Resource resident in shared memory.";
  case S_PRESENT_MODE_CHANGED:
    return "The desktop display mode has been changed.";
  case S_PRESENT_OCCLUDED:
    return "Client window is occluded. (minimized or other fullscreen)";
  case D3DERR_OUTOFVIDEOMEMORY:
    return "Direct3D does not have enough display memory to perform the operation.";
  case D3DERR_WASSTILLDRAWING:
    return "The previous blit operation that is transferring information to or from this surface is incomplete.";
  case D3DERR_WRONGTEXTUREFORMAT:
    return "The pixel format of the texture surface is not valid.";
  case D3DERR_UNSUPPORTEDCOLOROPERATION:
    return "The device does not support a specified texture-blending operation for color values.";
  case D3DERR_UNSUPPORTEDCOLORARG:
    return "The device does not support a specified texture-blending argument for color values.";
  case D3DERR_UNSUPPORTEDALPHAOPERATION:
    return "The device does not support a specified texture-blending operation for the alpha channel.";
  case D3DERR_UNSUPPORTEDALPHAARG:
    return "The device does not support a specified texture-blending argument for the alpha channel.";
  case D3DERR_TOOMANYOPERATIONS:
    return "The application is requesting more texture-filtering operations than the device supports.";
  case D3DERR_CONFLICTINGTEXTUREFILTER:
    return "The current texture filters cannot be used together.";
  case D3DERR_UNSUPPORTEDFACTORVALUE:
    return "The device does not support the specified texture factor value.";
  case D3DERR_CONFLICTINGRENDERSTATE:
    return "The currently set render states cannot be used together.";
  case D3DERR_UNSUPPORTEDTEXTUREFILTER:
    return "The device does not support the specified texture filter.";
  case D3DERR_CONFLICTINGTEXTUREPALETTE:
    return "The current textures cannot be used simultaneously.";
  case D3DERR_DRIVERINTERNALERROR:
    return "Internal driver error.";
  case D3DERR_NOTFOUND:
    return "The requested item was not found.";
  case D3DERR_MOREDATA:
    return "There is more data available than the specified buffer size can hold.";
  case D3DERR_DEVICELOST:
    return "The device has been lost but cannot be reset at this time. ";
  case D3DERR_DEVICENOTRESET:
    return "The device has been lost but can be reset at this time.";
  case D3DERR_NOTAVAILABLE:
    return "Not available";
  case D3DERR_INVALIDDEVICE:
    return "The requested device type is not valid.";
  case D3DERR_INVALIDCALL:
    return "The method call is invalid.";
  case D3DERR_DRIVERINVALIDCALL:
    return "Driver invalid call";
  case D3DERR_DEVICEREMOVED:
    return "The hardware adapter has been removed.";
  case D3DERR_DEVICEHUNG:
    return "Hardware adapter reset by the OS.";
  case D3DERR_UNSUPPORTEDOVERLAY:
    return "The device does not support overlay for the specified size or display mode.";
  case D3DERR_UNSUPPORTEDOVERLAYFORMAT:
    return "The device does not support overlay for the specified surface format.";
  case D3DERR_CANNOTPROTECTCONTENT:
    return "Contect protection not available.";
  case D3DERR_UNSUPPORTEDCRYPTO:
    return "The specified cryptographic algorithm is not supported.";
  case D3DERR_PRESENT_STATISTICS_DISJOINT:
    return "Presentation statistics are disjoint.";
  default:
    return "Unknown error.";
  }
}
