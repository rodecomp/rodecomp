#include "Client/Session.h"
#include "Contents/MessageTable.h"
#include "UI/Window/WindowDialogs.h"

#include <algorithm>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <vector>

// GLOBAL: gl_24750
std::vector<int> g_adminAccountIds;

// OFFSET: 0x00AD1BE0
// FUNCTION: fn_17812
__declspec(noinline) bool IsAdminAccount(int aid) {
  return std::find(g_adminAccountIds.begin(), g_adminAccountIds.end(), aid) != g_adminAccountIds.end();
}

// OFFSET: 0x00DFA390
// FUNCTION: fn_23572
int CSession::IsAdminSubMode(int subMode) {
  if (IsAdminAccount(this->m_aid) && subMode == 7) {
    return 1;
  }

  return 0;
}

// OFFSET: 0x00E09B50
// FUNCTION: fn_23789
void CSession::GetMonsterSkillInfo(const char* monster, int skill, int* action, int* loopType, WindowDialogs& dialogs) {
  *action = -1;

  char tableName[21];
  std::memcpy(tableName, "Monster_Action_Table", sizeof(tableName));
  XmlNode* table = this->m_monsterSkillInfo.FindChild(DecodeMessageRot(tableName));
  if (!table) {
    return;
  }

  char number[8];
  char skillName[8];
  _itoa(skill, number, 10);
  std::sprintf(skillName, "_%s", number);

  char monsterName[256];
  std::strcpy(monsterName, monster);
  _strlwr(monsterName);
  XmlNode* entry = table->FindChild(DecodeMessageRot(monsterName));
  if (!entry) {
    return;
  }

  XmlNode* skillNode = entry->FindChild(DecodeMessageRot(skillName));
  if (!skillNode) {
    return;
  }

  char fieldName[9];
  std::memcpy(fieldName, "action", 7);
  XmlNode* actionNode = skillNode->FindChild(DecodeMessageRot(fieldName));
  std::memcpy(fieldName, "loopType", 9);
  XmlNode* loopNode = skillNode->FindChild(DecodeMessageRot(fieldName));

  if (this->IsAdminSubMode(1) && (!actionNode || !loopNode)) {
    *action = 0;
    *loopType = 0;
    dialogs.Confirm("에러발생. 클라이언트 담당자 에게 에러 사실을 통보 하세요. 위치 - CPlayer :: GetMonsterSkillInfo", 0, true, false, 0, "Message", 280, 120, 0);
    dialogs.Confirm("action 또는 loopType 이 존재하지 않음.", 0, true, false, 0, "Message", 280, 120, 0);
    return;
  }

  char actionText[8];
  char loopText[8];
  std::sprintf(actionText, DecodeMessageRot(const_cast<char*>(actionNode->Text().c_str())));
  std::sprintf(loopText, DecodeMessageRot(const_cast<char*>(loopNode->Text().c_str())));
  *action = std::atoi(actionText);
  *loopType = std::atoi(loopText);
}
