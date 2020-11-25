// -*- C++ -*-
// Copyright (C) PathScale Inc. All rights reserved.

#ifndef CORE_CONVERSIONS_HPP
#define CORE_CONVERSIONS_HPP

#include "rapidjson.hpp"
// #include "types_fwd.hpp"

#include <arpa/inet.h>

#include <cassert>
#include <cstdint>
#include <string>
#include <string_view>
#include <type_traits>
#include <vector>
#include <optional>

namespace core {

// -----------------------------------------------------------------------------
// JSON specific conversions
// -----------------------------------------------------------------------------

/// @returns The result of conversion of `value` to the JSON string.
template<class Encoding, class Allocator>
std::string toJsonString(const rapidjson::GenericValue<Encoding, Allocator>& value) {
  /*
   * Note: GCC 7.4 has a bug and fails when try to deduce
   * template parameter of rapidjson::Writer implicitly as
   * allowed in C++17.
   */
  rapidjson::StringBuffer buffer;
  rapidjson::Writer<rapidjson::StringBuffer> writer{buffer};
  value.Accept(writer);
  return std::string{buffer.GetString(), buffer.GetSize()};
}

/// @returns The instance of JSON document constructed by parsing the `input`.
inline rapidjson::Document toJsonDocument(const std::string_view input) {
  rapidjson::Document result;
  result.Parse(input.data(), input.size());
  return result;
}

// -----------------------------------------------------------------------------
// Generalized conversions
// -----------------------------------------------------------------------------

/// Specializations of this structure implements the conversion algorithms.
template<typename> struct Conversions;

/**
 * @returns The result of conversion from `value` of type `FromType` to the
 * value of type `ToType` by using specializations of template structure
 * Conversions.
 */
template<typename ToType, typename FromType, typename... Types>
ToType to(FromType&& value, Types&&... extraArgs) {
  return Conversions<ToType>::to(std::forward<FromType>(value), std::forward<Types>(extraArgs)...);
}

template<typename ToType, typename Encoding, typename Allocator, typename... Types>
std::optional<ToType> optional_to(const rapidjson::GenericValue<Encoding, Allocator>& value, Types&&... extraArgs) {
  if (value.IsNull())
    return std::optional<ToType>{};
  return Conversions<ToType>::to(value, std::forward<Types>(extraArgs)...);
}

/// Specialization for bool.
template<> struct Conversions<bool> final {
  template<class Encoding, class Allocator>
  static bool to(const rapidjson::GenericValue<Encoding, Allocator>& value) {
    return value.GetBool();
  }
};

/// Specialization for int.
template<> struct Conversions<int> final {
  template<class Encoding, class Allocator>
  static int to(const rapidjson::GenericValue<Encoding, Allocator>& value) {
    return value.GetInt();
  }
};

/// Specialization for double.
template<> struct Conversions<double> final {
  template<class Encoding, class Allocator>
  static double to(const rapidjson::GenericValue<Encoding, Allocator>& value) {
    return value.GetDouble();
  }
};

/// Specialization for std::int_fast64_t.
template<> struct Conversions<std::int_fast64_t> final {
  template<class Encoding, class Allocator>
  static std::int_fast64_t to(const rapidjson::GenericValue<Encoding, Allocator>& value) {
    return value.GetInt64();
  }
};

/// Specialization for std::int_fast64_t.
template<> struct Conversions<std::uint_fast64_t> final {
  template<class Encoding, class Allocator>
  static std::uint_fast64_t to(const rapidjson::GenericValue<Encoding, Allocator>& value) {
    return value.GetUint64();
  }
};

/// Specialization for std::string.
template<> struct Conversions<std::string> final {
  template<class Encoding, class Allocator>
  static std::string to(const rapidjson::GenericValue<Encoding, Allocator>& value) {
    return std::string{value.GetString(), value.GetStringLength()};
  }

  template<typename T>
  static std::enable_if_t<std::is_signed_v<T>, std::string> to(const T value) {
    return std::to_string(value);
  }
};

/// Specialization for std::string_view.-
template<> struct Conversions<std::string_view> final {
  template<class Encoding, class Allocator>
  static std::string_view to(const rapidjson::GenericValue<Encoding, Allocator>& value) {
    return std::string_view{value.GetString(), value.GetStringLength()};
  }
};

/// Specialization for std::optional.
template<typename T>
struct Conversions<std::optional<T>> final {
    template<class Encoding, class Allocator>
    static std::optional<T> to(const rapidjson::GenericValue<Encoding, Allocator>& value) {
        return std::optional<T>{Conversions<T>::to(value)};
    }
};

/// Specialization for std::vector.-
template<typename T>
struct Conversions<std::vector<T>> final {
  template<class Encoding, class Allocator>
  static std::vector<T> to(const rapidjson::GenericValue<Encoding, Allocator>& value) {
    std::vector<T> vec;
    auto arr = value.GetArray();
    for (int a = 0; a < arr.Size(); a++) {
      auto& el = arr[a];
      vec.emplace_back(Conversions<T>::to(el));
    }
    return std::move(vec);
  }
};

/// Specialization for rapidjson::GenericValue
template<class Encoding, class Allocator>
struct Conversions<rapidjson::GenericValue<Encoding, Allocator>> final {
  template<typename T>
  static auto to(T&& value, Allocator& alloc) {
    if constexpr (!isRequiresAllocator<T>())
      return rapidjson::GenericValue<Encoding, Allocator>{std::forward<T>(value)};
    else
      return rapidjson::GenericValue<Encoding, Allocator>{std::forward<T>(value), alloc};
  }

  static auto to(const std::string_view value, Allocator& alloc) {
    return rapidjson::GenericValue<Encoding, Allocator>{value.data(), value.size()};
  }

  template<typename T>
  static auto to(const std::optional<T> value, Allocator& alloc) {
    if (!value)
      return rapidjson::GenericValue<Encoding, Allocator>{rapidjson::kNullType};
    return to(value.value(), alloc); // rapidjson::GenericValue<Encoding, Allocator>{value.data(), value.size()};
  }

private:
  template<typename T>
  constexpr static bool isRequiresAllocator() {
    using U = std::decay_t<T>;
    return !std::is_arithmetic_v<U>;
  }
};

// -----------------------------------------------------------------------------
// Network
// -----------------------------------------------------------------------------

/// @returns The text representation of the IP address.
inline std::string ip_to_string(const std::string_view binary) {
  const auto binsz = binary.size();
  assert(binsz == 4 || binsz == 6);
  std::string result(binsz == 4 ? 16 : 46, '\0');
  const auto* const r = ::inet_ntop(binsz == 4 ? AF_INET : AF_INET6, binary.data(), result.data(), result.size());
  assert(r);
  (void)r;
  return result;
}

// -----------------------------------------------------------------------------
// Numerics
// -----------------------------------------------------------------------------

/// @returns `std::stoi(str, pos, 10)`.
inline auto toInt10(const std::string& str, std::size_t* const pos) {
  return std::stoi(str, pos);
}

/// @returns `std::stoul(str, pos, 10)`.
inline auto toUnsignedLong10(const std::string& str, std::size_t* const pos) {
  return std::stoul(str, pos);
}

} // namespace core

#endif // CORE_CONVERSIONS_HPP
