#include "Client/Input/InputCharset.h"
#include "Client/Mode/ModeMgr.h"
#include "Contents/Configuration/ServiceConfiguration.h"
#include "Contents/Skin/SkinMgr.h"
#include "Resource/ResourceMgr.h"
#include "File/ResourcePath.h"
#include "Resource/TextureNames.h"

#include <cstring>
#include <string>

// OFFSET: 0x00ACE510
// FUNCTION: fn_17788
const char* CLanguageBase::ServiceName(const char* name) {
  if (g_serviceType != 0 && g_serviceType != 2 && g_serviceType != 20) {
    // STATIC: 0x016D3860
    // GLOBAL: gl_24372
    static char path[260];
    std::strcpy(path, this->servicePrefix.c_str());
    std::strcat(path, name);
    const bool exists = std::strstr(name, ".txt") || std::strstr(name, ".xml") ? ResourceExists(path, 0) : ResourceMgr::Instance().Exists(path);

    if (exists) {
      return path;
    }
  }

  return name;
}

// OFFSET: 0x00B01200
// FUNCTION: fn_18378
const char* Resolve(const char* name) {
  if (std::strlen(name) == 0) {
    return name;
  }

  const std::string original(name);

  if (original.find("유저인터페이스", 0, 14) != std::string::npos) {
    if (g_modeMgr.gameMode && g_skinInfo.Selected() == true) {
      const std::string prefix(g_skinInfo.Prefix());
      // STATIC: 0x016D58CC
      // GLOBAL: gl_24385
      static std::string selectedPath;
      std::string path(name);
      selectedPath.clear();
      const int length = static_cast<int>(path.size()) - 15;
      for (int index = 0; index < length; ++index) {
        selectedPath.push_back(path[index + 15]);
      }

      g_skinInfo.Prepare();
      selectedPath = prefix + selectedPath;
      path = selectedPath;
      if (ResourceMgr::Instance().Exists(path.c_str())) {
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
      std::string path(name);
      uiPath.clear();
      const int length = static_cast<int>(path.size()) - 15;
      for (int index = 0; index < length; ++index) {
        uiPath.push_back(path[index + 15]);
      }

      uiPath = "UI\\" + uiPath;
      path = uiPath;
      if (ResourceMgr::Instance().Exists(path.c_str())) {
        return uiPath.c_str();
      }
    }
  }

  return name;
}
