#include "Client/Application/Launcher.h"
#include "Client/Mode/CGameMode.h"
#include "Client/Session.h"
#include "Contents/Actors/GameActor.h"
#include "Contents/GuildMgr.h"
#include "File/File.h"

#include <cstdio>
#include <algorithm>
#include <cstring>

// GLOBAL: gl_24759
std::vector<int> g_monsterJobs;

// OFFSET: 0x00AD1C10
// FUNCTION: fn_17813
__declspec(noinline) int MonsterJobColor(int) {
  return 0;
}

// OFFSET: 0x00AD1C20
// FUNCTION: fn_17814
__declspec(noinline) bool IsMonsterJob(int job) {
  return std::find(g_monsterJobs.begin(), g_monsterJobs.end(), job) != g_monsterJobs.end();
}

// OFFSET: 0x00DDC6F0
// FUNCTION: fn_23368
ITEM_INFO CSession::GetItem17F0(int index) const {
  if (index == 15) {
    return LookupClientItemByIdentity(this->m_inventoryItems, this->unknown_16EC);
  }

  if (static_cast<unsigned int>(index) <= 9) {
    return this->unknown_17F0[index];
  }

  return ITEM_INFO(0);
}

// OFFSET: 0x00E3D290
// FUNCTION: fn_24184
unsigned char CSession::IsAllyActor(std::uint32_t aid, int job) {
  CGameActor* actor = static_cast<CGameActor*>(this->FindGameActor(aid));
  if (actor) {
    switch (actor->unknown_0314) {
    case 1:
    case 6:
    case 12:
      return 1;
    }
  }

  return this->IsNpcJob(job);
}

// OFFSET: 0x00CD3400
// FUNCTION: fn_21263
std::uint32_t CGameMode::FindUnknown01AC(std::uint32_t key) {
  const auto found = this->unknown_01AC.find(key);
  if (found != this->unknown_01AC.end()) {
    return found->second;
  }

  return 0;
}

// OFFSET: 0x00CD3470
// FUNCTION: fn_21264
std::string CGameMode::ActorName(std::uint32_t id) {
  return this->nameCard.Name(id);
}

// OFFSET: 0x00CD34D0
// FUNCTION: fn_21265
std::uint32_t CGameMode::ActorNameColor(int job, std::uint32_t aid) {
  if (IsMonsterJob(aid)) {
    return 0xFFFF;
  }

  if (MonsterJobColor(aid)) {
    return MonsterJobColor(aid);
  }

  if (g_session.IsAllyActor(aid, job) == 1) {
    return 0xF7B895;
  }

  if ((job > 1000 && job < 4000) || static_cast<unsigned int>(job - 20021) <= 11978) {
    return 0xC3C3FF;
  }

  return 0xFFFFFF;
}

// OFFSET: 0x00CD3570
// FUNCTION: fn_21266
const CActorNameCard& CGameMode::LookupNameCard(std::uint32_t id) {
  return this->nameCard.Lookup(id);
}

// OFFSET: 0x00CD3580
// FUNCTION: fn_21267
const char* CSession::Text8104() const {
  return this->unknown_8104.c_str();
}

// OFFSET: 0x00CD35A0
// FUNCTION: fn_21268
const char* CSession::Text80EC() const {
  return this->unknown_80EC.c_str();
}

// OFFSET: 0x00CD3A30
// FUNCTION: fn_21272
const char* CGameMode::DragItemName() {
  if (this->drag.source == 2 || this->drag.source == 24) {
    const ITEM_INFO item = g_session.GetItem17F0(this->drag.slot);
    return item.unknown_0000[1] != 0 ? item.itemId.c_str() : nullptr;
  }

  if (this->drag.name.size() == 0) {
    return nullptr;
  }

  return this->drag.name.c_str();
}

// OFFSET: 0x00CD3B10
// FUNCTION: fn_21273
bool CGameMode::AppendGuildEmblem(std::vector<char>& output, std::vector<std::string>& names, int guildId, int) {
  if (guildId <= 0) {
    return false;
  }

  char path[260];
  std::memset(path, 0, sizeof(path));
  std::string name = g_guildMgr->GetEmblemFilename(guildId);

  for (const std::string& existing : names) {
    if (std::strcmp(name.c_str(), existing.c_str()) == 0) {
      return false;
    }
  }

  std::sprintf(path, "%s\\_tmpEmblem\\%s", g_applicationDirectory, name.c_str());
  CFile file;
  if (!file.Open(path, 0, true, false)) {
    file.Close();
    return false;
  }

  output.resize(file.GetSize() + 4 + name.size());
  *reinterpret_cast<std::uint32_t*>(output.data()) = static_cast<std::uint32_t>(name.size());
  std::memcpy(output.data() + 4, name.c_str(), name.size());
  file.Read(output.data() + name.size() + 4, file.GetSize());
  file.Close();
  names.push_back(name);
  return true;
}
