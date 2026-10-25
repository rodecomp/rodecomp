#include "Resource/ResourceByteReader.h"
#include "Resource/World/WorldResource.h"

#include <array>
#include <cstring>

// OFFSET: 0x00761CC0
// FUNCTION: fn_08073

void C3dWorldRes::ReadObjects(ResourceByteReader& input, unsigned version, unsigned build, char* lastName) {
  int count;
  input.Read(&count, sizeof(count));

  for (int index = 0; index < count; ++index) {
    int type;
    input.Read(&type, sizeof(type));

    switch (type) {
    case 1: {
      int animation = 0;
      int unknown = 0;
      int modelIndex = -1;
      float speed = 1.0f;
      unsigned char flag = 0;

      if (version >= 0x103u) {
        input.Read(lastName, 40);
        input.Read(&animation, sizeof(animation));
        input.Read(&speed, sizeof(speed));
        input.Read(&unknown, sizeof(unknown));

        if (version >= 0x206u && build >= 186) {
          input.Read(&flag, 1);
        }

        if (version >= 0x207u) {
          input.Read(&modelIndex, sizeof(modelIndex));
        }

        if (speed > 100.0f || !(0.0f < speed)) {
          speed = 1.0f;
        }
      }

      std::array<unsigned char, 196> wire;
      std::memset(wire.data() + 160, 0, 36);
      input.Read(wire.data(), static_cast<unsigned>(wire.size()));
      auto* record = new WorldModelRecord;
      std::strcpy(record->name, lastName);
      std::strcpy(record->model, reinterpret_cast<const char*>(wire.data()));
      std::strcpy(record->node, reinterpret_cast<const char*>(wire.data() + 80));
      std::memcpy(record->position, wire.data() + 160, 12);
      std::memcpy(record->rotation, wire.data() + 172, 12);
      std::memcpy(record->scale, wire.data() + 184, 12);
      record->animationSpeed = speed;
      record->animation = animation;
      record->unknown_00F0 = unknown;
      record->flag = flag != 0;
      record->index = modelIndex;
      this->models.push_back(record);
      break;
    }

    case 2: {
      unsigned char light[108];
      std::memset(light + 72, 0, 12);
      input.Read(light, sizeof(light));
      break;
    }

    case 3: {
      auto* record = new WorldSoundRecord;
      std::memset(record->position, 0, sizeof(record->position));

      if (version < 0x200u) {
        unsigned char wire[188];
        std::memset(wire + 160, 0, 12);
        input.Read(wire, sizeof(wire));
        std::strncpy(record->name, reinterpret_cast<const char*>(wire), 79);
        std::strncpy(record->file, reinterpret_cast<const char*>(wire + 80), 79);
        std::memcpy(record->position, wire + 160, 12);
        std::memcpy(&record->volume, wire + 172, 4);
        std::memcpy(&record->width, wire + 176, 4);
        std::memcpy(&record->height, wire + 180, 4);
        std::memcpy(&record->range, wire + 184, 4);
        record->cycle = 4.0f;
      } else {
        input.Read(record, sizeof(*record));
      }

      this->sounds.push_back(record);
      break;
    }

    case 4: {
      auto* record = new WorldEffectRecord;
      std::memset(record->position, 0, sizeof(record->position));
      input.Read(record, sizeof(*record));
      this->effects.push_back(record);
      break;
    }

    default:
      break;
    }
  }
}
