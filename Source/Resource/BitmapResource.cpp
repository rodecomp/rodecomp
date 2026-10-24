#include "Client/ErrorDialog.h"
#include "Resource/BitmapResource.h"
#include "Resource/IJL15Runtime.h"

#include <cstring>
#include <windows.h>

// OFFSET: 0x00749580
// FUNCTION: fn_07847
CBitmapRes::CBitmapRes(BitmapResourceContext& context) : m_context(&context) {
}

// OFFSET: 0x007498B0
// FUNCTION: fn_07851
CBitmapRes::~CBitmapRes() = default;

// OFFSET: 0x00749580
// FUNCTION: fn_07847
CBitmapRes* CBitmapRes::Create() {
  return new CBitmapRes(*this->m_context);
}

// OFFSET: 0x00749630
// FUNCTION: fn_07848
bool CBitmapRes::Load(const char* name) {
  bool result = false;
  if (name && this->m_context->file.Open(name)) {
    if (this->Decode(name, this->m_context->file.Data(), this->m_context->file.Size(), false)) {
      this->m_context->file.Close();
      result = true;
    } else {
      this->m_context->file.Close();
    }
  }
  // Original CFile destructor invokes Close again after the explicit close.
  this->m_context->file.Close();
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
    return this->image.DecodeJpeg(data, bytes, this->m_context->jpeg);
  }
  if (lstrcmpiA(extension, ".png") == 0) {
    return this->image.DecodePng(data, bytes, name);
  }
  ShowErrorDialog(this->m_context->renderSystem, "Unsupport fileformat!", 0);
  return false;
}
