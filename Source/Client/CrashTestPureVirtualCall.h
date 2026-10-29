#ifndef RAGNAROK_SOURCE_CLIENT_CRASHTESTPUREVIRTUALCALL_H
#define RAGNAROK_SOURCE_CLIENT_CRASHTESTPUREVIRTUALCALL_H

class CrashTestPureVirtualCallBase {
public:
  virtual void Invoke() = 0;
};

class CrashTestPureVirtualCallDerived : public CrashTestPureVirtualCallBase {
public:
  void Invoke() override;
};

#endif
