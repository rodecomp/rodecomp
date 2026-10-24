#ifndef RAGNAROK_SOURCE_CONTENTS_AUTOMATICSKILLS_H
#define RAGNAROK_SOURCE_CONTENTS_AUTOMATICSKILLS_H

#include "Network/Connection.h"

#include <cstdint>
#include <set>

struct AutomaticSkillContext {
  ClientOptions& options;
  CConnection& connection;
  int& activeKey;
  const int& currentKey;
};

// Context borrows the actual option/socket/key owners.
class AutomaticSkills {
public:
  explicit AutomaticSkills(AutomaticSkillContext& context);
  ~AutomaticSkills();
  bool Contains(int id) const;
  static int CancelOption(std::int16_t id);
  void DisableAll();
  void Use(std::int16_t id, std::int16_t level, std::uint32_t target);
  void Complete(int id);

private:
  std::set<int> m_ids;
  AutomaticSkillContext* m_context;
  AutomaticSkills(const AutomaticSkills&) = delete;
  AutomaticSkills& operator=(const AutomaticSkills&) = delete;
};

#if defined(_WIN32) && !defined(_WIN64)
static_assert(sizeof(AutomaticSkills) == 12);
#endif

#endif
