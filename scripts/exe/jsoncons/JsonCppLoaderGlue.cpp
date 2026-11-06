// jsoncons v0.170.2 entry points used by Source/Utility/JsonCppLoader.cpp,
// linked only into the standalone exe built by scripts/exe/build_exe.py. Clank
// never compiles this file.
//
// The original statically links jsoncons; these ten functions are the
// instantiations JsonCppLoader calls by address (fn_00198 ... fn_00769).
// v0.170.2 is the last release whose basic_json storage tags (short string 6,
// long string 7, byte string 8, array 9, empty object 10, object 11, const
// pointer 12) and basic_json_decode_options layout (virtual base at +8)
// match JsonValue and the loader's option storage.
//
// JsonCppLoader declares them as extern "C" void(), takes their address and
// calls them through __thiscall or __cdecl pointers. The __thiscall ones are
// therefore __thiscall members of the *Thunk types here, and each plain C
// name is an /ALTERNATENAME for the member's decorated name (end of file).

#include <jsoncons/json.hpp>
#include <jsoncons_ext/bson/bson.hpp>

#include <fstream>
#include <functional>
#include <new>
#include <vector>

namespace jsoncons_glue {
  using Json = jsoncons::basic_json<char, jsoncons::sorted_policy>;
  using JsonOptions = jsoncons::basic_json_decode_options<char>;
  using JsonOptionsCommon = jsoncons::basic_json_options_common<char>;
  using BsonOptions = jsoncons::bson::bson_decode_options;
  using JsonHandler = std::function<bool(jsoncons::json_errc, const jsoncons::ser_context&)>;

  static_assert(sizeof(Json) == 0x10);
  static_assert(sizeof(JsonHandler) == 40);
  static_assert(sizeof(JsonOptions) <= 248);
  static_assert(sizeof(BsonOptions) <= 16);

  // Each member runs with the object the loader passes in ECX.
  struct HandlerThunk : JsonHandler {
    // fn_00198: std::function<...>(default_json_parsing), constructed in place
    // (the loader passes the storage twice: this and the source argument).
    void Construct(void*);
  };

  struct JsonOptionsThunk : JsonOptions {
    // fn_00321: constructor; the flag says whether to build the virtual base.
    JsonOptions* Construct(int);
  };

  struct JsonOptionsCommonThunk : JsonOptionsCommon {
    // fn_00368: destructor of the virtual base subobject (loader passes +8).
    void Destroy();
  };

  struct BsonOptionsThunk : BsonOptions {
    // fn_00349: constructor, same convention as fn_00321.
    BsonOptions* Construct(int);
  };

  struct JsonThunk : Json {
    // fn_00367: destructor.
    void Destroy();

    // fn_00769: swap, used to move the parsed document into place.
    void Swap(Json* other);
  };

  void HandlerThunk::Construct(void*) {
    new (static_cast<JsonHandler*>(this)) JsonHandler(jsoncons::default_json_parsing());
  }

  JsonOptions* JsonOptionsThunk::Construct(int) {
    return new (static_cast<JsonOptions*>(this)) JsonOptions();
  }

  void JsonOptionsCommonThunk::Destroy() {
    this->JsonOptionsCommon::~JsonOptionsCommon();
  }

  BsonOptions* BsonOptionsThunk::Construct(int) {
    return new (static_cast<BsonOptions*>(this)) BsonOptions();
  }

  void JsonThunk::Destroy() {
    static_cast<Json*>(this)->~Json();
  }

  void JsonThunk::Swap(Json* other) {
    static_cast<Json*>(this)->swap(*other);
  }
}

using namespace jsoncons_glue;

extern "C" {

// fn_00272: bson::decode_bson<json>(std::istream&, const bson_decode_options&).
Json* __cdecl fn_00272(Json* out, std::ifstream* file, BsonOptions* options) {
  return new (out) Json(jsoncons::bson::decode_bson<Json>(*file, *options));
}

// fn_00273: bson::decode_bson<json>(const std::vector<uint8_t>&, const bson_decode_options&).
Json* __cdecl fn_00273(Json* out, const std::vector<unsigned char>* buffer, BsonOptions* options) {
  return new (out) Json(jsoncons::bson::decode_bson<Json>(*buffer, *options));
}

// fn_00733: json::parse(std::istream&, const json_decode_options&, handler).
// The handler is passed by value: the callee owns and destroys it.
Json* __cdecl fn_00733(Json* out, std::ifstream* file, JsonOptions* options, JsonHandler handler) {
  return new (out) Json(Json::parse(*file, *options, handler));
}

// fn_00734: json::parse(const char*, const json_decode_options&, handler).
Json* __cdecl fn_00734(Json* out, const unsigned char* data, JsonOptions* options, JsonHandler handler) {
  return new (out) Json(Json::parse(jsoncons::basic_string_view<char>(reinterpret_cast<const char*>(data)), *options, handler));
}

}

#pragma comment(linker, "/ALTERNATENAME:_fn_00198=?Construct@HandlerThunk@jsoncons_glue@@QAEXPAX@Z")
#pragma comment(linker, "/ALTERNATENAME:_fn_00321=?Construct@JsonOptionsThunk@jsoncons_glue@@QAEPAV?$basic_json_decode_options@D@jsoncons@@H@Z")
#pragma comment(linker, "/ALTERNATENAME:_fn_00368=?Destroy@JsonOptionsCommonThunk@jsoncons_glue@@QAEXXZ")
#pragma comment(linker, "/ALTERNATENAME:_fn_00349=?Construct@BsonOptionsThunk@jsoncons_glue@@QAEPAVbson_decode_options@bson@jsoncons@@H@Z")
#pragma comment(linker, "/ALTERNATENAME:_fn_00367=?Destroy@JsonThunk@jsoncons_glue@@QAEXXZ")
#pragma comment(linker, "/ALTERNATENAME:_fn_00769=?Swap@JsonThunk@jsoncons_glue@@QAEXPAV?$basic_json@DUsorted_policy@jsoncons@@V?$allocator@D@std@@@jsoncons@@@Z")
