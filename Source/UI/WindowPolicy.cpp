#include "UI/WindowPolicy.h"

// OFFSET: 0x00A970E0
// FUNCTION: fn_17200
int WindowPolicy(const int* table, bool restricted, unsigned action, unsigned id) {
  if (action == 3) {
    if (!restricted) {
      return 1;
    }
  } else if (action >= 8) {
    return 0;
  }
  if (id <= 10040) {
    return table[8 * id + action];
  }
  return 0;
}
