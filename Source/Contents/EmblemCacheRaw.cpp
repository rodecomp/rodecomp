#include "Client/Application/Launcher.h"
#include "Client/Session.h"
#include "Contents/GuildMgr.h"
#include "File/FileEnumeration.h"

#include <cstdio>
#include <direct.h>
#include <io.h>
#include <list>
#include <string>
#include <sys/stat.h>

// GLOBAL: gl_24748
std::uint32_t g_guildId;

// OFFSET: 0x00DFF600
// FUNCTION: fn_23620
void CSession::StoreEmblem(const void* data, std::size_t size, std::uint32_t guildId, int version) {
  char directory[256];
  char path[256];
  char filename[256];

  if (guildId == g_guildId) {
    g_guildMgr->SetEmblemVersion(guildId, version);
  }

  std::sprintf(directory, "%s\\_tmpEmblem\\", g_applicationDirectory);
  if (_access(directory, 0) != 0 && _mkdir(directory) == -1) {
    return;
  }

  std::sprintf(path, "%s_%d_*.ebm", this->m_accountName.c_str(), guildId);
  std::list<std::string> names;
  EnumerateFileNames(names, directory, path);
  for (const std::string& name : names) {
    std::sprintf(path, "%s%s", directory, name.c_str());
    _chmod(path, _S_IWRITE);
    _unlink(path);
  }

  std::sprintf(filename, "%s%s_%d_%d.ebm", directory, this->m_accountName.c_str(), guildId, version);
  FILE* file = std::fopen(filename, "wb");
  if (file) {
    std::fwrite(data, size, 1, file);
    std::fclose(file);
  }
}
