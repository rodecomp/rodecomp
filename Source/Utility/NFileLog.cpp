#include "Utility/NFileLog.h"
#include "Utility/Text/FormatString.h"

#include <cstdio>
#include <cstring>
#include <ctime>
#include <io.h>

// FUNCTION: fn_07589
bool NFileLog::Initialize(const char* directoryName, const char* filePrefix) {
  char currentDirectory[1024]{};
  GetCurrentDirectoryA(sizeof(currentDirectory), currentDirectory);
  StringFormat(this->directory, sizeof(this->directory), static_cast<std::size_t>(-1), "%s\\%s", currentDirectory, directoryName);
  CreateDirectoryA(this->directory, nullptr);
  SYSTEMTIME currentTime;
  GetLocalTime(&currentTime);
  StringFormat(this->prefix, sizeof(this->prefix), static_cast<std::size_t>(-1), "%s", filePrefix);
  this->year = currentTime.wYear;
  this->month = currentTime.wMonth;
  this->day = currentTime.wDay;

  __time32_t cutoffTime = _time32(nullptr) - 15552000;
  const std::tm cutoff = *_localtime32(&cutoffTime);
  char cutoffPrefix[260]{};
  StringFormat(cutoffPrefix, sizeof(cutoffPrefix), static_cast<std::size_t>(-1), "%s(%04d%02d%2d)", this->prefix, cutoff.tm_year + 1900, cutoff.tm_mon, cutoff.tm_mday);
  char filename[1024]{};
  StringFormat(filename, sizeof(filename), static_cast<std::size_t>(-1), "%s\\%s*.*", this->directory, this->prefix);
  WIN32_FIND_DATAA found{};
  HANDLE search = FindFirstFileA(filename, &found);
  if (search != INVALID_HANDLE_VALUE) {
    do {
      if (std::strncmp(found.cFileName, cutoffPrefix, std::strlen(cutoffPrefix)) < 0) {
        char expired[260]{};
        StringFormat(expired, sizeof(expired), static_cast<std::size_t>(-1), "%s\\%s", this->directory, found.cFileName);
        _unlink(expired);
      }
    } while (FindNextFileA(search, &found));
    FindClose(search);
  }

  int lastExisting = -1;
  int nextIndex = 0;
  for (;; ++nextIndex) {
    char candidate[1024]{};
    char base[1024]{};
    StringFormat(base, sizeof(base), static_cast<std::size_t>(-1), "%s\\%s(%04d%02d%02d)", this->directory, this->prefix, this->year, this->month, this->day);
    if (nextIndex != 0) {
      StringFormat(candidate, sizeof(candidate), static_cast<std::size_t>(-1), "%s-%04d.txt", base, nextIndex);
    } else {
      StringFormat(candidate, sizeof(candidate), static_cast<std::size_t>(-1), "%s.txt", base);
    }
    if (_access(candidate, 0) != 0) {
      break;
    }
    lastExisting = nextIndex;
  }

  bool reuse = false;
  if (lastExisting >= 0) {
    std::memset(filename, 0, sizeof(filename));
    char base[1024]{};
    StringFormat(base, sizeof(base), static_cast<std::size_t>(-1), "%s\\%s(%04d%02d%02d)", this->directory, this->prefix, this->year, this->month, this->day);
    if (lastExisting != 0) {
      StringFormat(filename, sizeof(filename), static_cast<std::size_t>(-1), "%s-%04d.txt", base, lastExisting);
    } else {
      StringFormat(filename, sizeof(filename), static_cast<std::size_t>(-1), "%s.txt", base);
    }
    FILE* file = nullptr;
    if (fopen_s(&file, filename, "r") == 0 && file != nullptr) {
      std::fseek(file, 0, SEEK_END);
      this->fileSize = std::ftell(file);
      std::fclose(file);
      reuse = this->fileSize < 3145728;
    }
  }
  if (!reuse) {
    this->fileSize = 0;
    lastExisting = nextIndex;
  }
  this->fileIndex = lastExisting;
  return static_cast<unsigned>(lastExisting) <= 9999;
}
