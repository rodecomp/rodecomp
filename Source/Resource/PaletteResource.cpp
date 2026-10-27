#include "Resource/PaletteResource.h"
#include "Client/Application/ErrorDialog.h"
#include "File/File.h"
#include "Render/DeviceContext/DCSurface.h"
#include "Resource/Sprites/SpriteResource.h"

#include <string>

// INLINED
CPaletteRes::CPaletteRes() : sourcePalette{}, palette{} {
}

// INLINED
CPaletteRes::~CPaletteRes() = default;

// OFFSET: 0x0075D730

// OFFSET: 0x0075D7D0

// OFFSET: 0x0075DAF0
// FUNCTION: fn_08041
void CPaletteRes::Unload() {
}
