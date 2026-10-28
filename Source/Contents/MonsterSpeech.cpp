#include "Client/Mode/ActiveGameMode.h"
#include "Client/Mode/CGameMode.h"
#include "Client/Session.h"

#include <cstdint>
#include <cstdio>
#include <cstring>
#include <string>

// STRING: st_16737
static const char monsterSpeechFather[] = "아빠";
// STRING: st_16738
static const char monsterSpeechOlderBrotherHonorific[] = "형님";
// STRING: st_16739
static const char monsterSpeechOlderBrother[] = "오빠";
// STRING: st_16740
static const char monsterSpeechYoungMaster[] = "도련님";
// STRING: st_16741
static const char monsterSpeechBrother[] = "형씨";
// STRING: st_16742
static const char monsterSpeechYoungMasterTopic[] = "도련님은";
// STRING: st_16743
static const char monsterSpeechYoungMasterObject[] = "도련님을";
// STRING: st_16744
static const char monsterSpeechYoungMasterWith[] = "도련님과";
// STRING: st_16745
static const char monsterSpeechMother[] = "엄마";
// STRING: st_16746
static const char monsterSpeechOlderSisterHonorific[] = "누님";
// STRING: st_16747
static const char monsterSpeechOlderSister[] = "언니";
// STRING: st_16748
static const char monsterSpeechYoungLady[] = "아가씨";
// STRING: st_16749
static const char monsterSpeechYoungLadyTopic[] = "아가씨는";
// STRING: st_16750
static const char monsterSpeechYoungLadyObject[] = "아가씨를";
// STRING: st_16751
static const char monsterSpeechYoungLadyWith[] = "아가씨와";

// GLOBAL: gl_24523
unsigned char monsterSpeechReplacementCount = 13;

// GLOBAL: gl_24524
const char* monsterSpeechReplacements[26] = {
  monsterSpeechFather,
  monsterSpeechOlderBrotherHonorific,
  monsterSpeechOlderBrother,
  monsterSpeechYoungMaster,
  monsterSpeechBrother,
  monsterSpeechYoungMasterTopic,
  monsterSpeechYoungMasterObject,
  monsterSpeechYoungMasterWith,
  monsterSpeechFather,
  monsterSpeechFather,
  monsterSpeechFather,
  monsterSpeechFather,
  monsterSpeechFather,
  monsterSpeechMother,
  monsterSpeechOlderSisterHonorific,
  monsterSpeechOlderSister,
  monsterSpeechYoungLady,
  monsterSpeechYoungLady,
  monsterSpeechYoungLadyTopic,
  monsterSpeechYoungLadyObject,
  monsterSpeechYoungLadyWith,
  monsterSpeechMother,
  monsterSpeechMother,
  monsterSpeechMother,
  monsterSpeechMother,
  monsterSpeechMother,
};

// OFFSET: 0x00E12870
// FUNCTION: fn_23818
void FormatMonsterSpeech(char* text, char* output, std::uint32_t aid, int feminine) {
  char* source = *text == '\t' ? text + 1 : text;
  std::sprintf(output, source);

  int offset = 0;
  int length = 0;
  char* cursor = output;
  while (feminine && monsterSpeechReplacementCount) {
    char* found = nullptr;
    for (int index = 0; index < monsterSpeechReplacementCount; ++index) {
      found = std::strstr(cursor, monsterSpeechReplacements[index]);
      if (found) {
        const char* replacement = monsterSpeechReplacements[monsterSpeechReplacementCount + index];
        length = static_cast<int>(std::strlen(replacement));
        std::strcpy(found, replacement);
        offset = static_cast<int>(found - output);
        std::strcat(found, source + offset + length);
        cursor = found + length;
        break;
      }
      cursor = output + offset + length;
    }
    if (!found) {
      break;
    }
  }

  const std::string name = static_cast<CGameMode*>(g_world.ActiveGameMode())->nameCard.Name(aid);
  const char* speaker;
  if (!name.empty()) {
    speaker = name.c_str();
  } else if (aid == g_session.unknown_1010) {
    speaker = g_session.unknown_1018;
  } else {
    *output = '\0';
    return;
  }

  std::sprintf(source, "%s : ", speaker);
  std::strcat(source, output);
  std::strcpy(output, source);
  const std::size_t end = std::strlen(output) - 1;
  if (output[end] == '\t') {
    output[end] = '\0';
  }
}
