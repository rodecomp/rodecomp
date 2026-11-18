#include "Resource/Bitmap/PaletteResource.h"
#include "Client/Application/ErrorDialog.h"
#include "File/File.h"
#include "Render/DeviceContext/DCSurface.h"
#include "Resource/Sprites/SpriteResource.h"
#include "Resource/ResourceMgr.h"

#include <cstring>
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
  if (!file.Open(name, 0, true, false)) {
    ShowErrorDialog((std::string("CPaletteRes :: Cannot find File : ") + std::string(name)).c_str(), 0);
    return false;
  }

  file.Read(this->sourcePalette, sizeof(this->sourcePalette));
  ConvertSpritePalette(this->palette, this->sourcePalette, 256);
  file.Close();
  return true;
}

// OFFSET: 0x0075DAF0
// FUNCTION: fn_08041
void CPaletteRes::Unload() {
}

// OFFSET: 0x0075DBD0
// FUNCTION: fn_45834
void CPaletteRes::Save(std::uint32_t* output) {
  const auto* source = this->sourcePalette;
  volatile auto* header = output;
  header[0] = 0x204C4150;
  header[1] = 0x61746164;
  header[0] = 0x01000300;
  std::memcpy(output, source, sizeof(this->sourcePalette));
}

// INLINED
CPaletteRes::CPaletteRes(const char* extension, const char* prefix) {
  ResourceMgr::Instance().Register(extension, prefix, this);
}

// GLOBAL: gl_25562
CPaletteRes g_palPrototype("pal", "palette\\");
