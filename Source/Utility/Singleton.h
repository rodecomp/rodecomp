#ifndef RAGNAROK_SOURCE_UTILITY_SINGLETON_H
#define RAGNAROK_SOURCE_UTILITY_SINGLETON_H

#include <new>

template <typename T>
class Singleton {
public:
  static T* s_instance;

  // OFFSET: 0x005A2630
  // FUNCTION: fn_03115
  // OFFSET: 0x00610D60
  // FUNCTION: fn_04320
  // OFFSET: 0x0061F9A0
  // FUNCTION: fn_04454
  // OFFSET: 0x006863F0
  // FUNCTION: fn_05565
  // OFFSET: 0x00AB1040
  // FUNCTION: fn_17330
  // OFFSET: 0x00AB10B0
  // FUNCTION: fn_17331
  // OFFSET: 0x00E5CED0
  // FUNCTION: fn_24374
  // OFFSET: 0x00E5D020
  // FUNCTION: fn_24376
  // OFFSET: 0x00E5D090
  // FUNCTION: fn_24377
  static T* GetInstance() {
    if (!s_instance) {
      s_instance = new (std::nothrow) T;
    }

    return s_instance;
  }

  virtual ~Singleton() = default;
};

#endif
