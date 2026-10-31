#include "Resource/PaletteResource.h"
#include "Client/Application/ErrorDialog.h"
#include "File/File.h"
#include "Render/DeviceContext/DCSurface.h"
#include "Resource/Sprites/SpriteResource.h"

#include <string>

// OFFSET: 0x0075DB60
// FUNCTION: fn_08043
CPaletteRes::CPaletteRes() : sourcePalette{}, palette{} {
}

// INLINED
CPaletteRes::~CPaletteRes() = default;

// OFFSET: 0x0075D730
// FUNCTION: fn_08039
CPaletteRes* CPaletteRes::Create() {
  return new CPaletteRes;
}

// OFFSET: 0x0075D7D0
// FUNCTION: fn_08040
bool CPaletteRes::Load(const char* name) {
  CFile file;
  if (file.Open(name, 0, true, false)) {
    file.Read(this->sourcePalette, sizeof(this->sourcePalette));
    ConvertSpritePalette(this->palette, this->sourcePalette, 256);
    file.Close();
    return true;
  }

  const std::string message = std::string("CPaletteRes :: Cannot find File : ") + name;
  ShowErrorDialog(GetRenderSystemHolder()->system, message.c_str(), 0);
  return false;
}

// OFFSET: 0x0075DAF0
// FUNCTION: fn_08041
void CPaletteRes::Unload() {
}
