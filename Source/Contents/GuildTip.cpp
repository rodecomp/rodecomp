#include "Contents/GuildTip.h"
#include "Client/Input/InputCharset.h"
#include "Contents/Configuration/ServiceConfiguration.h"
#include "File/File.h"
#include "File/ResourcePath.h"

#include <cstdio>

// OFFSET: 0x00AE54B0
// FUNCTION: fn_18052
std::vector<StringList> CGuildTip::GetTipList() {
  if (this->m_bTryLoading == false) {
    this->Load();
  }

  return this->m_tipList;
}

// OFFSET: 0x00AE55B0
// FUNCTION: fn_18053
int CGuildTip::Load() {
  this->m_bTryLoading = true;
  char* fileBuf = nullptr;
  int fileSize = 0;

  if (g_serviceType == 0) {
    FILE* file = std::fopen("GuildTip.txt", "r");

    if (file == nullptr) {
      return 0;
    }

    std::fseek(file, 0, SEEK_END);
    fileSize = std::ftell(file);
    fileBuf = new char[fileSize + 1];
    std::fseek(file, 0, SEEK_SET);
    std::fread(fileBuf, fileSize, 1, file);
    std::fclose(file);
  } else {
    const char* fName = g_serverLanguage->ServiceName("GuildTip.txt");

    if (!ResourceExists(fName, 0)) {
      return 0;
    }

    CFile fp;

    if (!fp.Open(fName, 0, true, false)) {
      return 0;
    }

    fileSize = static_cast<int>(fp.GetSize());
    fileBuf = new char[fileSize + 1];
    fp.Read(fileBuf, fileSize);
    fp.Close();
  }

  fileBuf[fileSize] = 0;

  char* baseBuf = fileBuf;
  StringList tmpList;

  for (int i = 0; i < fileSize; ++i) {
    switch (fileBuf[i]) {
    case 10: {
      fileBuf[i] = 0;
      std::string line(baseBuf);
      tmpList.push_back(line);
      baseBuf = fileBuf + i + 1;
      break;
    }
    case '#':
      fileBuf[i] = 0;
      tmpList.push_back(baseBuf);
      this->m_tipList.push_back(tmpList);
      tmpList.clear();
      baseBuf = fileBuf + i + 1;
      break;
    }
  }

  if (fileBuf) {
    delete[] fileBuf;
  }

  return 1;
}
