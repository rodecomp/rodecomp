#ifndef RAGNAROK_SOURCE_CLIENT_MODE_MODEEVENTS_H
#define RAGNAROK_SOURCE_CLIENT_MODE_MODEEVENTS_H

#include <cstdint>

class ModeEvents {
public:
  virtual std::intptr_t SendModeMessage(int event, std::intptr_t argument1, std::intptr_t argument2, std::intptr_t argument3, std::intptr_t argument4) = 0;

protected:
  ~ModeEvents() = default;
};

#endif
