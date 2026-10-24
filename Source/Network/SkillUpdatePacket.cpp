#include "Network/SkillUpdatePacket.h"
#include "UI/WindowOrdering.h"

#include <cstring>

namespace {
  template <class T> T Read(const unsigned char* bytes) {
    T value;
    std::memcpy(&value, bytes, sizeof(value));
    return value;
  }
}

// OFFSET: 0x00D42AA0
// FUNCTION: fn_21829
void SkillUpdatePacket::Apply(const void* packet) {
  this->pending = 0;

  const auto* bytes = static_cast<const unsigned char*>(packet);
  const auto opcode = Read<std::uint16_t>(bytes);
  const auto id = Read<std::uint16_t>(bytes + 2);
  CSkillInfo value;
  switch (opcode) {
  case 270:
    value = this->client.bundle.Lookup(
      id, this->client.classMode, this->client.HasEffect(1717), this->client.HasEffect(1716)
    );
    break;
  case 569:
    value = this->client.LookupFirst(id);
    break;
  case 670:
    value = this->client.LookupOther(id);
    break;
  }

  value.id = id;
  value.level = Read<std::int16_t>(bytes + 4);
  value.unknown_0014 = static_cast<std::int32_t>(Read<std::int16_t>(bytes + 6));
  value.unknown_0018 = bytes[10];
  value.unknown_001C = static_cast<std::int32_t>(Read<std::int16_t>(bytes + 8));
  value.unknown_0034 = value.UseApAmount(this->script, value.level);
  this->client.bundle.ApplyLevelChange(value);

  switch (opcode) {
  case 270:
    this->client.Update(value, 1, 0, this->script, this->levels, this->windows);
    break;
  case 569:
    this->first.Update(value, 1);
    break;
  case 670:
    this->other.Update(value, 1);
    break;
  }
}

// OFFSET: 0x00D06AF0
// FUNCTION: fn_21577
void LearnedSkillReceiver::Apply(
  std::uint16_t id,
  std::int16_t level,
  int unknown000C,
  std::uint8_t unknown0018,
  std::int16_t unknown0014,
  std::int16_t unknown001C,
  std::int16_t unknown0030
) {
  CSkillInfo value;
  value.unknown_000C = unknown000C;
  value.id = id;
  value.level = level;
  value.unknown_0018 = unknown0018;
  value.name = value.IdName(this->script);
  value.unknown_0014 = unknown0014;
  value.unknown_001C = unknown001C;
  value.unknown_0030 = static_cast<std::uint16_t>(unknown0030);
  value.unknown_0034 = value.UseApAmount(this->script, level);

  this->client.bundle.RecordLevelChange(value);
  if (value.id == 336) {
    this->skill336 = 1;
  }

  this->client.Update(value, 1, 0, this->script, this->levels, this->windows);
}

// OFFSET: 0x00D2B5C0
// FUNCTION: fn_45442
void AdditionalSkillPacket::Apply(const void* packet) {
  this->pending = 0;
  this->client.additional.clear();

  const auto* bytes = static_cast<const unsigned char*>(packet);
  // Original divides a zero-extended (signed length - 6) by 37.
  const auto length = static_cast<std::uint32_t>(static_cast<int>(Read<std::int16_t>(bytes + 2)) - 6);
  const std::uint32_t count = length / 37;
  const unsigned char* record = bytes + 6;
  for (std::uint32_t index = 0; index < count; ++index, record += 37) {
    CSkillInfo value;
    value.level = Read<std::int16_t>(record + 6);
    value.id = Read<std::uint16_t>(record);
    value.unknown_0014 = static_cast<std::int32_t>(Read<std::int16_t>(record + 8));
    value.unknown_000C = Read<std::uint32_t>(record + 2);
    value.unknown_0018 = record[36];
    value.name = value.IdName(this->script);
    value.unknown_0034 = value.UseApAmount(this->script, value.level);

    if (value.id - 10000u <= 19u) {
      this->client.UpdateAdditional(value, 1);
    }
  }

  this->capacity = Read<std::int16_t>(bytes + 4);
  this->ordering.InvalidateFrames(this->frameLatch);
  this->windows.Notify();
}
