// -*- C++ -*-
// Copyright (C) PathScale Inc. All rights reserved.

#ifndef CORE_RAPIDJSON_HPP
#define CORE_RAPIDJSON_HPP

#include <cstddef>
namespace rapidjson {
using SizeType = std::size_t;
}
#include <rapidjson/fwd.h>
#include <rapidjson/document.h>
#include <rapidjson/schema.h>
#include <rapidjson/stringbuffer.h>
#include <rapidjson/writer.h>

namespace core {

inline const rapidjson::Value NullJsonValue;

} // namespace core

#endif // CORE_RAPIDJSON_HPP
