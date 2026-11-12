#include "Client/Application/ErrorDialog.h"
#include "File/File.h"
#include "Render/DeviceContext/DCSurface.h"
#include "Resource/Bitmap/BitmapResource.h"
#include "Resource/ResourceMgr.h"

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
    return this->DecodeBitmap(data, bytes, iconOnly);
  }

  if (lstrcmpiA(extension, ".tga") == 0) {
    return this->DecodeTga(data, bytes);
  }

  if (lstrcmpiA(extension, ".jpg") == 0) {
    return this->DecodeJpeg(data, bytes);
  }

  if (lstrcmpiA(extension, ".png") == 0) {
    return this->DecodePng(data, bytes, name);
  }

  ShowErrorDialog("Unsupport fileformat!", 0);
  return false;
}

// INLINED
CBitmapRes::CBitmapRes(const char* extension, const char* prefix) {
  ResourceMgr::Instance().Register(extension, prefix, this);
}

// GLOBAL: gl_25549
CBitmapRes g_tgaPrototype("tga", "texture\\");

// GLOBAL: gl_25550
CBitmapRes g_jpgPrototype("jpg", "texture\\");

// GLOBAL: gl_25551
CBitmapRes g_pngPrototype("png", "texture\\");

// GLOBAL: gl_25552
CBitmapRes g_bmpPrototype("bmp", "texture\\");
