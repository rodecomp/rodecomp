#include "Contents/NavigationManager.h"

#include "Client/Registry.h"
#include "Utility/RegistryKey.h"

#include <cstring>

// OFFSET: 0x00B88800
// FUNCTION: fn_19376
NavigationObject* NavigationManager::FindObject(const char* name) const {
  for (NavigationObject* object : this->objects) {
    if (object->HasName(name)) {
      return object;
    }
  }
  return nullptr;
}

// OFFSET: 0x00B8D270
// FUNCTION: fn_19434
bool NavigationObject::HasName(const char* name) const {
  return std::strcmp(this->name.c_str(), name) == 0;
}

// OFFSET: 0x00B996D0
// FUNCTION: fn_19613
void NavigationManager::LoadSettings() {
  RegistryKey key;
  std::string subKey(g_subKey);
  subKey += "\Navigation";
  if (key.Open(HKEY_LOCAL_MACHINE, subKey.c_str())) {
    DWORD viewIcon = 0;
    if (key.Query("ViewIcon", REG_DWORD, reinterpret_cast<BYTE*>(&viewIcon), sizeof(viewIcon))) {
      this->viewIcon = viewIcon;
    }
  }
}

// OFFSET: 0x00B9C020
// FUNCTION: fn_19618
float NavigationManager::FitToSquare(int* offsetX, int* offsetY, int width, int height, float size) const {
  float scale;
  if (width > height) {
    scale = size / width;
    *offsetY = static_cast<int>((width - height) * 0.5f * scale);
    *offsetX = 0;
  } else if (width < height) {
    scale = size / height;
    *offsetX = static_cast<int>((height - width) * 0.5f * scale);
    *offsetY = 0;
  } else {
    *offsetY = 0;
    scale = size / width;
    *offsetX = 0;
  }
  return scale;
}
