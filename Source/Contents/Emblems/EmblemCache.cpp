#include "Contents/Emblems/EmblemCache.h"
#include "UI/Window/UIWindow.h"
#include "UI/Window/WindowManagerWindows.h"

#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <direct.h>
#include <io.h>
#include <memory>
#include <sstream>
#include <sys/stat.h>
#include <windows.h>
#include <zlib.h>

namespace {
  // INLINED
  std::string Basename(std::string path) {
    const auto separator = path.rfind('\\');

    if (separator != std::string::npos) {
      return path.substr(separator + 1);
    }

    return path;
  }
}

// OFFSET: 0x0065ED00
// FUNCTION: fn_05178
std::string EmblemCache::Filename(std::uint32_t guild, std::uint32_t revision) const {
  std::ostringstream name;
  name << this->accountName << '_' << guild << '_' << (revision + 0x7FFFFFFFu) << ".ebm";
  return name.str();
}

// OFFSET: 0x0065E6F0
// FUNCTION: fn_05176
bool EmblemCache::IsInvalid(std::string path) const {
  return this->invalid.contains(Basename(path));
}

// OFFSET: 0x0065DFB0
// FUNCTION: fn_05171
void EmblemCache::MarkInvalid(std::string path) {
  this->invalid.insert(Basename(path));
}

// OFFSET: 0x0065E890
// FUNCTION: fn_05177
void EmblemCache::Store(const void* bytes, std::uint32_t size, std::uint32_t guild, std::uint32_t revision) {
  const std::string directory = std::string(this->rootDirectory) + "\\_tmpEmblem\\";
  const std::string filename = this->Filename(guild, revision);
  uLongf compressedSize = compressBound(size);
  std::unique_ptr<unsigned char[]> compressed(new unsigned char[compressedSize]);

  if (compress(compressed.get(), &compressedSize, static_cast<const Bytef*>(bytes), size) != Z_OK) {
    this->MarkInvalid(filename);
    return;
  }

  if (_access(directory.c_str(), 0) != 0 && _mkdir(directory.c_str()) == -1) {
    return;
  }

  std::ostringstream pattern;
  pattern << this->accountName << '_' << guild << "_*.ebm";
  WIN32_FIND_DATAA found{};
  const HANDLE search = FindFirstFileA((directory + pattern.str()).c_str(), &found);
  std::list<std::string> names;

  if (search != INVALID_HANDLE_VALUE) {
    do {
      if (std::strcmp(found.cFileName, ".") != 0 && std::strcmp(found.cFileName, "..") != 0) {
        names.emplace_back(found.cFileName);
      }
    } while (FindNextFileA(search, &found));
  }

  FindClose(search);

  for (const std::string& name : names) {
    const std::string path = directory + name;
    _chmod(path.c_str(), _S_IWRITE);
    _unlink(path.c_str());
  }

  const std::string path = directory + filename;
  FILE* file = std::fopen(path.c_str(), "wb");

  if (file) {
    std::fwrite(compressed.get(), compressedSize, 1, file);
    std::fclose(file);
  }
}

// OFFSET: 0x00D588E0
// FUNCTION: fn_21954
void EmblemCache::Receive(std::uint16_t state, std::uint32_t guild, std::uint32_t revision, const unsigned char* bytes, int size) {
  if (state == 0) {
    this->guildId = guild;
    this->version = revision;
    this->data.clear();
  } else if (state == 1) {
    if (guild == this->guildId && revision == this->version) {
      this->data.insert(this->data.end(), bytes, bytes + size);
    }
  } else if (state == 2 && guild == this->guildId && revision == this->version) {
    this->Store(this->data.data(), static_cast<std::uint32_t>(this->data.size()), guild, revision);
    this->Refresh(guild);
  }
}

// OFFSET: 0x0065EE50
// FUNCTION: fn_05180
void EmblemCache::Refresh(std::uint32_t guild) {
  if (this->modeWindows) {
    for (UIWindow* window : *this->modeWindows) {
      window->SendMsg(nullptr, 60, guild, 0, 0, 0);
    }
  }

  UIWindow* window = this->windows.LookupWindow(60);

  if (window) {
    window->Invalidate();
  }
}
