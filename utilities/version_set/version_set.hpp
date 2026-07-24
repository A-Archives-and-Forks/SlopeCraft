//
// Created by Joseph on 2024/9/15.
//

#ifndef SLOPECRAFT_VERSION_SET_H
#define SLOPECRAFT_VERSION_SET_H

#include <cstdint>
#include <bitset>
#include <magic_enum/magic_enum.hpp>

#include "SC_GlobalEnums.h"

constexpr inline size_t major_version_to_idx(SCL_gameVersion v) noexcept {
  switch (v) {
    case SCL_gameVersion::ANCIENT:
      return 0;
    case SCL_gameVersion::MC12:
      return 1;
    case SCL_gameVersion::MC13:
      return 2;
    case SCL_gameVersion::MC14:
      return 3;
    case SCL_gameVersion::MC15:
      return 4;
    case SCL_gameVersion::MC16:
      return 5;
    case SCL_gameVersion::MC17:
      return 6;
    case SCL_gameVersion::MC18:
      return 7;
    case SCL_gameVersion::MC19:
      return 8;
    case SCL_gameVersion::MC20:
      return 9;
    case SCL_gameVersion::MC21:
      return 10;
    case SCL_gameVersion::MC26_1_2:
      return 11;
    default:
      return 31;
  }
}
#warning \
    "TODO: rewrite this. Use std::vector or std::set. We mustn't use version for index anymore"
class version_set {
 private:
  std::bitset<32> set{0};

 public:
  version_set() = default;

  explicit version_set(uint32_t val) : set(val) {}

  static version_set all() noexcept {
    version_set ret(~uint32_t(0));
    return ret;
  }

  inline bool match(SCL_gameVersion v) const noexcept {
    return set[major_version_to_idx(v)];
  }

  inline auto operator[](SCL_gameVersion v) noexcept {
    return set[major_version_to_idx(v)];
  }

  inline auto operator[](SCL_gameVersion v) const noexcept {
    return set[major_version_to_idx(v)];
  }

  inline uint64_t to_u32() const noexcept { return set.to_ulong(); }

  inline bool operator==(const version_set &vs) const noexcept {
    return this->to_u32() == vs.to_u32();
  }

  inline SCL_gameVersion introduced_version() const noexcept {
    for (auto v : magic_enum::enum_values<SCL_gameVersion>()) {
      if (match(v)) {
        return v;
      }
    }
    return SCL_gameVersion::FUTURE;
  }
};

#endif  // SLOPECRAFT_VERSION_SET_H
