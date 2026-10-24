#include "Contents/JobNames.h"
#include "Script/LuaScript.h"

// OFFSET: 0x00DE67E0
// FUNCTION: fn_23495
void JobNames::Initialize(JobNameInitialization& context) {
  this->names.resize(32003, "Poring");
  const char* name = nullptr;
  const auto player = [&](int first, int last) {
    for (int id = first; id <= last; ++id) {
      if (context.playerScript.Call("ReqPCJobName", "d>s", id, &name) && *name != '\0') {
        this->names[id] = name;
      }

      if (context.playerScript.Call("ReqPCJobNameMan", "d>s", id, &name) && *name != '\0') {
        this->male[id] = name;
      }

      if (context.playerScript.Call("ReqPCJobNameWoman", "d>s", id, &name) && *name != '\0') {
        this->female[id] = name;
      }
    }
  };
  player(0, 30);
  player(4001, 4329);
  player(4332, 4349);
  player(4351, 4360);
  const auto other = [&](int first, int last) {
    for (int id = first; id <= last; ++id) {
      // Unlike player names, empty returned strings are retained here.
      context.otherScript.Call("ReqJobName", "d>s", id, &this->names[id]);
    }
  };
  other(45, 125);
  other(400, 999);
  other(1001, 3998);
  other(10000, 19998);
  other(20021, 31999);

  // Literal stores. Keep these bytes
  // independent of the current language/code page and script table content.
  const char* const homunculi[] = {"LIF",    "AMISTR",      "FILIR",    "VANILMIRTH",   "LIF2",    "AMISTR2",
                                   "FILIR2", "VANILMIRTH2", "LIF_H",    "AMISTR_H",     "FILIR_H", "VANILMIRTH_H",
                                   "LIF_H2", "AMISTR_H2",   "FILIR_H2", "VANILMIRTH_H2"};

  for (unsigned index = 0; index < 16; ++index) {
    this->names[6001 + index] = homunculi[index];
  }

  this->names[6048] = "MER_EIRA";
  this->names[6051] = "MER_DIETER";
  this->names[6049] = "MER_BAYERI";
  this->names[6052] = "MER_ELEANOR";
  this->names[6050] = "MER_SERA";

  for (unsigned id = 6017; id <= 6026; ++id) {
    this->names[id] = "\xBF\xA9\\\xC8\xB0\xBF\xEB\xBA\xB4";
  }

  for (unsigned id = 6027; id <= 6036; ++id) {
    this->names[id] = "\xB3\xB2\\\xC3\xA2\xBF\xEB\xBA\xB4";
  }

  for (unsigned id = 6037; id <= 6046; ++id) {
    this->names[id] = "\xB3\xB2\\\xB0\xCB\xBF\xEB\xBA\xB4";
  }

  context.sexLabels.resize(2, "F");
  context.sexLabels[0] = "F";
  context.sexLabels[1] = "M";
  context.levels20.resize(20, 0);
  context.useLevels.resize(7999, 0);
  context.otherLevels.resize(258, 0);
  context.homunculusLevels.resize(61, 0);
}
