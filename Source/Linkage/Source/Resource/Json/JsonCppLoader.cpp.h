#pragma once

#include "Resource/Json/JsonCppLoader.h"

template __declspec(noinline) jsoncons::json& jsoncons::json::at(const jsoncons::json::string_view_type&);
template __declspec(noinline) jsoncons::range<jsoncons::json::object_iterator, jsoncons::json::const_object_iterator> jsoncons::json::object_range();
