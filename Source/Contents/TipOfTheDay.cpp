#include "Contents/TipOfTheDay.h"
#include "Client/Input/InputCharset.h"
#include "Contents/Configuration/ServiceConfiguration.h"
#include "File/File.h"
#include "File/ResourcePath.h"

#include <cstdio>

// GLOBAL: gl_24741
CTipOfTheDay g_tipOfTheDay;

// GLOBAL: gl_24740
bool g_tipOfTheDayEnglish;

// OFFSET: 0x00AE7760
// FUNCTION: fn_18079
int CTipOfTheDay::Load() {
  this->m_bTryLoading = true;
  char* fileBuf = nullptr;
  int fileSize = 0;

  const char* fName;
  bool bDataFolder = false;

  if (g_serviceType == 0) {
    fName = "tipOfTheday.txt";
  } else if (g_serviceType == 19) {
    fName = g_tipOfTheDayEnglish ? "tipoftheday.txt" : "tipOfTheday_Eng.txt";
  } else {
    fName = g_serverLanguage->ServiceName("tipofday.txt");

    if (!ResourceExists(fName, 0)) {
      return 0;
    }

    bDataFolder = true;
  }

  if (bDataFolder) {
    CFile fp;

    if (!fp.Open(fName, 0, true, false)) {
      return 0;
    }

    fileSize = static_cast<int>(fp.GetSize());
    fileBuf = new char[fileSize + 1];
    fp.Read(fileBuf, fileSize);
    fp.Close();
  } else {
    FILE* file = std::fopen(fName, "r");

    if (file == nullptr) {
      return 0;
    }

    std::fseek(file, 0, SEEK_END);
    fileSize = std::ftell(file);
    fileBuf = new char[fileSize + 1];
    std::fseek(file, 0, SEEK_SET);
    std::fread(fileBuf, fileSize, 1, file);
    std::fclose(file);
  }

  fileBuf[fileSize] = 0;

  char* baseBuf = fileBuf;
  StringList tmpList;

  for (int i = 0; i < fileSize; ++i) {
    switch (fileBuf[i]) {
    case '#':
      fileBuf[i] = 0;
      tmpList.push_back(baseBuf);
      this->m_tipList.push_back(tmpList);
      tmpList.clear();
      baseBuf = fileBuf + i + 1;
      break;
    case 10:
      fileBuf[i] = 0;
      tmpList.push_back(baseBuf);
      baseBuf = fileBuf + i + 1;
      break;
    }
  }

  if (fileBuf) {
    delete[] fileBuf;
  }

  return 1;
}

// OFFSET: 0x00909CF0
// FUNCTION: fn_12400
int CTipOfTheDay::Size() {
  if (this->m_bTryLoading == false) {
    this->Load();
  }

  return static_cast<int>(this->m_tipList.size());
}

// OFFSET: 0x00AE7710
// FUNCTION: fn_18078
StringList& CTipOfTheDay::GetTip(int index) {
  if (this->m_bTryLoading == false) {
    this->Load();
  }

  if (static_cast<unsigned>(index) >= this->m_tipList.size() || index < 0) {
    return this->m_tipList[0];
  }

  return this->m_tipList[index];
}

struct SubAE7C10 {
  int a;
  int b;
  char c;
  SubAE7C10();
};

// OFFSET: 0x00AE7C10
// FUNCTION: fn_18080
SubAE7C10::SubAE7C10() : a(0), b(0), c(0) {
}

// OFFSET: 0x00AE7D20
// FUNCTION: fn_18081
int __stdcall sub_AE7D20(void*, void*) {
  return 0;
}

