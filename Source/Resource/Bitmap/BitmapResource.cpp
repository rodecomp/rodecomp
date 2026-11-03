#include "Client/Application/ErrorDialog.h"
#include "File/File.h"
#include "Render/DeviceContext/DCSurface.h"
#include "Resource/Bitmap/BitmapResource.h"

#include <cstring>
#include <windows.h>

// INLINED
CBitmapRes::~CBitmapRes() = default;

// OFFSET: 0x00749580
// FUNCTION: fn_07847
CBitmapRes* CBitmapRes::Create() {
  return new CBitmapRes;
}

// OFFSET: 0x00749630
// FUNCTION: fn_07848
bool CBitmapRes::Load(const char* name) {
  CFile file;
  bool result = false;
  if (name && file.Open(name, 0, true, false)) {
    if (this->Decode(name, file.GetData(), file.GetSize(), false)) {
      file.Close();
      result = true;
    } else {
      file.Close();
    }
  }

  file.Close();
  return result;
}

// OFFSET: 0x00749850
// FUNCTION: fn_07850
void CBitmapRes::Unload() {
  this->image.Clear();
}

// OFFSET: 0x00749780
// FUNCTION: fn_07849
bool CBitmapRes::Decode(const char* name, const void* data, int bytes, bool iconOnly) {
  const char* extension = std::strrchr(name, '.');
  if (!extension) {
    return false;
  }

  if (lstrcmpiA(extension, ".bmp") == 0) {
    return this->image.DecodeBitmap(data, bytes, iconOnly);
  }

  if (lstrcmpiA(extension, ".tga") == 0) {
    return this->image.DecodeTga(data, bytes);
  }

  if (lstrcmpiA(extension, ".jpg") == 0) {
    return this->image.DecodeJpeg(data, bytes);
  }

  if (lstrcmpiA(extension, ".png") == 0) {
    return this->image.DecodePng(data, bytes, name);
  }

  ShowErrorDialog(GetRenderSystemHolder()->system, "Unsupport fileformat!", 0);
  return false;
}
