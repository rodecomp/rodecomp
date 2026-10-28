#include "Client/Input/InputCharset.h"
#include "Client/Session.h"
#include "File/File.h"
#include "Resource/XmlNode.h"

// OFFSET: 0x00E1A2B0
// FUNCTION: fn_23848
void CSession::LoadXmlTables() {
  CFile petTalkFile;
  petTalkFile.Open(g_serverLanguage->ServiceName("PetTalkTable.xml"), 0, true, false);
  const char* petTalk = static_cast<const char*>(petTalkFile.GetData());
  this->m_petTalkTable.Parse(petTalk, petTalk + petTalkFile.GetSize());
  petTalkFile.Close();

  CFile skillFile;
  if (skillFile.Open("MonsterSkillInfo.xml", 0, true, false)) {
    const char* skillInfo = static_cast<const char*>(skillFile.GetData());
    this->m_monsterSkillInfo.Parse(skillInfo, skillInfo + skillFile.GetSize());
    skillFile.Close();
  }

  CFile talkFile;
  if (talkFile.Open("MonsterTalkTable.xml", 0, true, false)) {
    const char* monsterTalk = static_cast<const char*>(talkFile.GetData());
    this->m_monsterTalkTable.Parse(monsterTalk, monsterTalk + talkFile.GetSize());
    talkFile.Close();
  }
}
