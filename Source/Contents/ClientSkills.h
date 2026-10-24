#ifndef RAGNAROK_SOURCE_CONTENTS_CLIENTSKILLS_H
#define RAGNAROK_SOURCE_CONTENTS_CLIENTSKILLS_H

#include "Contents/PlayerSkillBundle.h"

class UIWindow;

// Live original skill-window globals plus the authoritative window lookup.
struct SkillWindows {
  UIWindow*& first;
  UIWindow*& second;
  WindowManagerWindows& windows;

  void Notify();
};

// Borrowed client use-level vectors (+0x70, +0x710, +0x71C). The additional
// range deliberately has no upper bound in the original lookup.
struct SkillUseLevels {
  const std::vector<int>& use;
  const std::vector<int>& additional;
  const std::vector<int>& homunculus;

  int Lookup(int id) const;
};

// Same recovered algorithm for client+0x84 and client+0x8C. Each view borrows
// that list's specific window global and the shared second skill window.
struct CompanionSkills {
  std::list<CSkillInfo>& skills;
  UIWindow*& window;
  UIWindow*& sharedWindow;

  // Updates only the first ID match; returns true only when appending. Existing
  // names, availability, prerequisites and unknown_0030 remain untouched.
  bool Update(const CSkillInfo& value, unsigned char notify);
  void Notify();
};

// Active-effect vector, 28-byte records.
struct ActiveSkillEffect {
  std::uint32_t id;
  std::uint32_t unknown_0004;
  std::uint32_t unknown_0008;
  std::uint32_t unknown_000C;
  std::uint32_t unknown_0010;
  std::uint32_t unknown_0014;
  std::uint32_t unknown_0018;
};

// Non-owning view of original client fields; not a second client singleton.
struct ClientSkills {
  CPlayerSkillBundle& bundle;
  const std::list<CSkillInfo>& firstRange;  // client+0x84
  const std::list<CSkillInfo>& otherRanges; // client+0x8C
  std::list<CSkillInfo>& additional;        // client+0x04
  const int& classMode;                     // client+0x8134
  const std::vector<ActiveSkillEffect>& effects;

  bool HasEffect(std::uint32_t id) const;
  CSkillInfo LookupFirst(std::uint32_t id) const;
  CSkillInfo LookupOther(std::uint32_t id) const;
  CSkillInfo LookupAdditional(std::uint32_t id) const;
  CSkillInfo Lookup(std::uint32_t id) const;
  // First-match partial update; retains availability/name/unknown_000C and
  // prerequisites. Append copies the full value. The original ignores unused.
  bool UpdateAdditional(const CSkillInfo& value, int unused);
  // Grouped skills restore the recorded previous level and always notify.
  // Nongrouped skills remove the first base/extra matches; notify only when
  // an extra record was removed. Pending skills and change records survive.
  void Remove(std::uint32_t id, SkillWindows& windows);

  // Returns true only for a newly appended base skill. Updates grouped/extra
  // state from a snapshot before the Lua query, then rereads the live input
  // for base state. Existing base records notify/copy unknown_000C only for
  // flag == 1; new records notify for any nonzero flag.
  bool Update(
    const CSkillInfo& value,
    unsigned char notify,
    unsigned char updateUnknown000C,
    LuaScript& script,
    const SkillUseLevels& levels,
    SkillWindows& windows
  );
};

static_assert(sizeof(ActiveSkillEffect) == 28);

#endif
