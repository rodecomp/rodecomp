#ifndef RAGNAROK_SOURCE_RESOURCE_EMBLEMRESOURCE_H
#define RAGNAROK_SOURCE_RESOURCE_EMBLEMRESOURCE_H

#include "Resource/GifResource.h"
#include "Resource/Resource.h"

class CEmblemRes : public CRes {
public:
  CEmblemRes();
  CEmblemRes(const char* extension, const char* prefix);
  ~CEmblemRes() override;
  void OnProcess() override;
  bool IsTransient() override;
  CEmblemRes* Create() override;
  bool Load(const char* name) override;
  void Unload() override;
  void MissingFile(const char* name) override;

  /* 0110 */ CGifRes gif;
};

#endif
