#ifndef RAGNAROK_SOURCE_RESOURCE_IMFRESOURCE_H
#define RAGNAROK_SOURCE_RESOURCE_IMFRESOURCE_H

#include "Resource/Resource.h"

#include <windows.h>

#include <vector>

struct ImfData {
  /* 0000 */ std::vector<int> priority;
  /* 000C */ std::vector<int> cx;
  /* 0018 */ std::vector<int> cy;
};

class CImfRes : public CRes {
public:
  CImfRes();
  CImfRes* Create() override;
  bool Load(const char* name) override;

  void Reset();
  int GetLayer(int priority, int action, int motion);
  POINT GetLongPoint(int layer, int action, int motion);

private:
  /* 0110 */ std::vector<std::vector<ImfData>> m_imfData;
  /* 011C */ int m_maxAction[15];
  /* 0158 */ int m_maxMotion[15][104];
};

#if defined(_WIN32) && !defined(_WIN64)
static_assert(sizeof(ImfData) == 36);
static_assert(sizeof(CImfRes) == 0x19B8);
#endif

#endif
