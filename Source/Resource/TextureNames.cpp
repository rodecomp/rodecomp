#include "Client/Input/InputCharset.h"
#include "Client/Mode/ModeMgr.h"
#include "Contents/Configuration/ServiceConfiguration.h"
#include "Contents/SkinMgr.h"
#include "Resource/ResourceMgr.h"
#include "File/ResourcePath.h"
#include "Resource/TextureNames.h"

#include <cstring>
#include <string>

// OFFSET: 0x00ACE510
// FUNCTION: fn_17788
const char* InputCharset::ServiceName(const char* name) {
  if (g_serviceType == 0 || g_serviceType == 2 || g_serviceType == 20) {
    return name;
  }

  // STATIC: 0x016D3860
  // GLOBAL: gl_24372
  static char path[260];
  std::strcpy(path, this->servicePrefix.c_str());
  std::strcat(path, name);
  const bool exists = std::strstr(name, ".txt") || std::strstr(name, ".xml") ? ResourceExists(path, 0) : ResourceMgr::Instance().Exists(path);

  return exists ? path : name;
}

// OFFSET: 0x00B01200
// FUNCTION: fn_18378
const char* Resolve(const char* name) {
  if (*name == '\0') {
    return name;
  }

  const std::string original(name);
  constexpr char marker[] = "유저인터페이스";

  if (original.find(marker, 0, 14) == std::string::npos) {
    return name;
  }

  if (g_modeMgr.gameMode && g_skinInfo.Selected()) {
    const std::string prefix(g_skinInfo.Prefix());
    // STATIC: 0x016D58CC
    // GLOBAL: gl_24385
    static std::string selectedPath;
    selectedPath = prefix;

    if (original.size() > 15) {
      selectedPath.append(original.data() + 15, original.size() - 15);
    }

    g_skinInfo.Prepare();

    if (ResourceMgr::Instance().Exists(selectedPath.c_str())) {
      return selectedPath.c_str();
    }
  }

  if (g_serviceType != 0) {
    if (ResourceMgr::Instance().Exists(g_serverLanguage->ServiceName(name))) {
      return g_serverLanguage->ServiceName(name);
    }

    // STATIC: 0x016D58E8
    // GLOBAL: gl_24386
    static std::string uiPath;
    uiPath = "UI\\";
    if (original.size() > 15) {
      uiPath.append(original.data() + 15, original.size() - 15);
    }

    if (ResourceMgr::Instance().Exists(uiPath.c_str())) {
      return uiPath.c_str();
    }
  }

  return name;
}
