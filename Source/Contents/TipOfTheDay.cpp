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

// OFFSET: 0x00909CF0
// FUNCTION: fn_12400
int CTipOfTheDay::Size() {
  if (this->m_bTryLoading == false) {
    this->Load();
  }

  return static_cast<int>(this->m_tipList.size());
}

// OFFSET: 0x00AE7710
