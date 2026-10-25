#include "Contents/SkinInfo.h"
#include "File/ResourcePath.h"
#include "Resource/ResourceMgr.h"
#include "Resource/TextureNames.h"

#include <cstring>

// OFFSET: 0x00ACE510
// FUNCTION: fn_17788
const char* TextureNames::ServiceName(const char* name) {
  if (this->service == 0 || this->service == 2 || this->service == 20) {
    return name;
  }

  // STATIC: 0x016D3860
  // GLOBAL: gl_24372
  static char path[260];
  std::strcpy(path, this->servicePrefix.c_str());
  std::strcat(path, name);
  const bool exists = std::strstr(name, ".txt") || std::strstr(name, ".xml") ? ResourceExists(path, 0) : this->resources.Exists(path);

  return exists ? path : name;
}

// OFFSET: 0x00B01200
// FUNCTION: fn_18378
const char* TextureNames::Resolve(const char* name) {
  if (*name == '\0') {
    return name;
  }

  const std::string original(name);
  constexpr char marker[] = "\xC0\xAF\xC0\xFA\xC0\xCE\xC5\xCD\xC6\xE4\xC0\xCC\xBD\xBA";

  if (original.find(marker, 0, 14) == std::string::npos) {
    return name;
  }

  if (this->activeMode && this->skin.Selected()) {
    const std::string prefix(this->skin.Prefix());
    // STATIC: 0x016D58CC
    // GLOBAL: gl_24385
    static std::string selectedPath;
    selectedPath = prefix;

    if (original.size() > 15) {
      selectedPath.append(original.data() + 15, original.size() - 15);
    }

    this->skin.Prepare();

    if (this->resources.Exists(selectedPath.c_str())) {
      return selectedPath.c_str();
    }
  }

  if (this->service != 0) {
    const char* serviceName = this->ServiceName(name);

    if (this->resources.Exists(serviceName)) {
      return this->ServiceName(name);
    }

    // STATIC: 0x016D58E8
    // GLOBAL: gl_24386
    static std::string uiPath;
    uiPath = "UI\\";
    if (original.size() > 15) {
      uiPath.append(original.data() + 15, original.size() - 15);
    }

    if (this->resources.Exists(uiPath.c_str())) {
      return uiPath.c_str();
    }
  }

  return name;
}
