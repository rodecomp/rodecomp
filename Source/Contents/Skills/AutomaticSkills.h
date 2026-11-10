#ifndef RAGNAROK_SOURCE_CONTENTS_SKILLS_AUTOMATICSKILLS_H
#define RAGNAROK_SOURCE_CONTENTS_SKILLS_AUTOMATICSKILLS_H

#include <cstdint>
#include <set>

class AutomaticSkills {
public:
  AutomaticSkills();
  ~AutomaticSkills();
  __declspec(noinline) bool Contains(int id) const;
  int CancelOption(int id) const;
  void DisableAll();
  void Use(std::int16_t id, std::int16_t level, std::uint32_t target);
  void Complete(int id);

private:
  /* 0000 */ std::set<int> m_ids;
  AutomaticSkills(const AutomaticSkills&) = delete;
  AutomaticSkills& operator=(const AutomaticSkills&) = delete;
};

#if defined(_WIN32) && !defined(_WIN64)
static_assert(sizeof(AutomaticSkills) == 8);
#endif

#endif
