#ifndef RAGNAROK_SOURCE_CLIENT_MODEEVENTS_H
#define RAGNAROK_SOURCE_CLIENT_MODEEVENTS_H

#include <cstdint>

// Source view of active CMode's virtual slot6, not a replacement CMode owner.
// Concrete mode implementations must supply their real event handling.
class ModeEvents {
public:
  virtual std::intptr_t SendModeMessage(
    int event,
    std::intptr_t argument1,
    std::intptr_t argument2,
    std::intptr_t argument3,
    std::intptr_t argument4
  ) = 0;

protected:
  ~ModeEvents() = default;
};

#endif
