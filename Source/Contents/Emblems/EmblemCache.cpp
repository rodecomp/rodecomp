#include "Client/Application/Launcher.h"
#include "Client/Mode/CGameMode.h"
#include "Client/Mode/ModeMgr.h"
#include "Client/Session.h"
#include "Contents/Emblems/GuildEmblemReqBundle.h"
#include "Contents/GuildMgr.h"
#include "File/FileEnumeration.h"
#include "UI/Window/UIWindow.h"
#include "UI/Window/UIWindowMgr.h"
#include "Utility/Text/FormatString.h"

#include <cstdio>
#include <cstring>
#include <direct.h>
#include <io.h>
#include <list>
#include <memory>
#include <sstream>
#include <sys/stat.h>
#include <windows.h>
#include <zlib.h>

namespace {
  // INLINED
  __forceinline std::string Basename(const std::string& path) {
    std::string name = path;
    const auto separator = path.rfind('\\');
    if (separator != std::string::npos) {
      name = path.substr(separator + 1);
    }

    return name;
  }
}

// OFFSET: 0x0065ED00
// FUNCTION: fn_05178
std::string CGuildMgr::EmblemFilename(std::uint32_t guild, std::uint32_t revision) const {
  std::stringstream name;
  name << g_session.m_accountName << "_" << guild << "_" << (revision + 0x7FFFFFFFu) << ".ebm";
  return name.str();
}

// OFFSET: 0x0065E6F0
// FUNCTION: fn_05176
bool CGuildMgr::IsInvalidEmblem(std::string path) const {
  return this->m_invalidEmblems.contains(Basename(path));
}

// OFFSET: 0x0065DFB0
// FUNCTION: fn_05171
void CGuildMgr::MarkInvalidEmblem(std::string path) {
  this->m_invalidEmblems.insert(Basename(path));
}

// OFFSET: 0x0065E890
// FUNCTION: fn_05177
void CGuildMgr::StoreEmblem(const void* bytes, std::uint32_t size, std::uint32_t guild, std::uint32_t revision) {
  const std::string directory = FormatOwnedString("%s\\_tmpEmblem\\", g_applicationDirectory);
  const std::string filename = g_guildMgr->EmblemFilename(guild, revision);
  uLongf compressedSize = compressBound(size);
  Bytef* compressed = new Bytef[compressedSize];

  if (compress(compressed, &compressedSize, static_cast<const Bytef*>(bytes), size) == Z_OK) {
    if (_access(directory.c_str(), 0) != 0 && _mkdir(directory.c_str()) == -1) {
      return;
    }

    {
      std::stringstream stream;
      stream << g_session.m_accountName << "_" << guild << "_*.ebm";
      std::string path = stream.str();
      std::list<std::string> names;
      EnumerateFileNames(names, directory.c_str(), path.c_str());

      for (const std::string& name : names) {
        path = directory + name;
        _chmod(path.c_str(), _S_IWRITE);
        _unlink(path.c_str());
      }
    }

    const std::string path = directory + filename;
    FILE* file = std::fopen(path.c_str(), "wb");

    if (file) {
      std::fwrite(compressed, compressedSize, 1, file);
      std::fclose(file);
    }
  } else {
    g_guildMgr->MarkInvalidEmblem(filename);
  }

  delete compressed;
}

// OFFSET: 0x00D588E0
// FUNCTION: fn_21954
void __stdcall CGuildMgr::ReceiveEmblem(std::uint16_t state, std::uint32_t guild, std::uint32_t revision, const unsigned char* bytes, int size) {
  CGuildMgr* manager = g_guildMgr;
  switch (state) {
  case 0:
    manager->m_receiveGuildId = guild;
    manager->m_receiveVersion = revision;
    manager->m_receiveData.clear();
    break;
  case 1:
    if (guild == manager->m_receiveGuildId && revision == manager->m_receiveVersion) {
      manager->m_receiveData.insert(manager->m_receiveData.end(), bytes, bytes + size);
    }
    break;
  case 2:
    if (guild == manager->m_receiveGuildId && revision == manager->m_receiveVersion) {
      StoreEmblem(manager->m_receiveData.data(), static_cast<std::uint32_t>(manager->m_receiveData.size()), guild, revision);
      RefreshEmblem(guild);
    }
    break;
  }
}

// OFFSET: 0x0065EE40
// FUNCTION: fn_05179
void CGuildMgr::UpdateEmblemRequests(
  CEmblemDataMgr& emblems, std::uint32_t accountId, const std::string& accountName, const char* token16, CConnection& connection, CAsyncWorkMgr& manager
) {
  this->m_reqBundle->Update(emblems, accountId, accountName, token16, connection, manager);
}

// OFFSET: 0x0065EE50
// FUNCTION: fn_05180
void CGuildMgr::RefreshEmblem(std::uint32_t guild) {
  if (g_modeMgr.GetGameMode()) {
    static_cast<CGameMode*>(g_modeMgr.GetGameMode())->NotifyUnknown0604(guild);
  }

  UIWindow* window = g_windowMgr.LookupWindow(60);

  if (window) {
    window->Invalidate();
  }
}
