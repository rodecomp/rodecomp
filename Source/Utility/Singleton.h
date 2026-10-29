#ifndef RAGNAROK_SOURCE_UTILITY_SINGLETON_H
#define RAGNAROK_SOURCE_UTILITY_SINGLETON_H

template <typename T>
class Singleton {
public:
  static T* s_instance;

  static T* GetInstance();

  virtual ~Singleton() = default;
};

#endif
