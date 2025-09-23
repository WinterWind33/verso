// Copyright (c) 2025 Andrea Ballestrazzi
#include "verso/verso.hpp"

// This file is used to perform static assertions on concepts and other compile-time
namespace verso::tests {
namespace normal_version_number_concept_tests {
static_assert(NormalVersionNumberComponent<unsigned int>);
static_assert(NormalVersionNumberComponent<unsigned long>);
static_assert(NormalVersionNumberComponent<unsigned long long>);
static_assert(NormalVersionNumberComponent<std::uint8_t>);
static_assert(NormalVersionNumberComponent<std::uint16_t>);
static_assert(NormalVersionNumberComponent<std::uint32_t>);
static_assert(NormalVersionNumberComponent<std::uint64_t>);
static_assert(NormalVersionNumberComponent<std::uint_fast8_t>);
static_assert(NormalVersionNumberComponent<std::uint_fast16_t>);
static_assert(NormalVersionNumberComponent<std::uint_fast32_t>);
static_assert(NormalVersionNumberComponent<std::uint_fast64_t>);
static_assert(NormalVersionNumberComponent<std::uint_least8_t>);
static_assert(NormalVersionNumberComponent<std::uint_least16_t>);
static_assert(NormalVersionNumberComponent<std::uint_least32_t>);
static_assert(NormalVersionNumberComponent<std::uint_least64_t>);

// Not version numbers
static_assert(!NormalVersionNumberComponent<int>);
static_assert(!NormalVersionNumberComponent<long>);
static_assert(!NormalVersionNumberComponent<long long>);
static_assert(!NormalVersionNumberComponent<std::int8_t>);
static_assert(!NormalVersionNumberComponent<std::int16_t>);
static_assert(!NormalVersionNumberComponent<std::int32_t>);
static_assert(!NormalVersionNumberComponent<std::int64_t>);
static_assert(!NormalVersionNumberComponent<std::int_fast8_t>);
static_assert(!NormalVersionNumberComponent<std::int_fast16_t>);
static_assert(!NormalVersionNumberComponent<std::int_fast32_t>);
static_assert(!NormalVersionNumberComponent<std::int_fast64_t>);
static_assert(!NormalVersionNumberComponent<std::int_least8_t>);
static_assert(!NormalVersionNumberComponent<std::int_least16_t>);
static_assert(!NormalVersionNumberComponent<std::int_least32_t>);
static_assert(!NormalVersionNumberComponent<std::int_least64_t>);
static_assert(!NormalVersionNumberComponent<float>);
static_assert(!NormalVersionNumberComponent<double>);
static_assert(!NormalVersionNumberComponent<long double>);
static_assert(!NormalVersionNumberComponent<char>);
} // namespace normal_version_number_concept_tests

namespace version_traits_concept_tests {
static_assert(VersionTraits<default_version_traits<std::uint8_t>>);
static_assert(VersionTraits<default_version_traits<std::uint16_t>>);
static_assert(VersionTraits<default_version_traits<std::uint32_t>>);
static_assert(VersionTraits<default_version_traits<std::uint64_t>>);
} // namespace version_traits_concept_tests

namespace version_concept_tests {
static_assert(Version<version>);
static_assert(Version<version8>);
static_assert(Version<version16>);
static_assert(Version<version32>);
static_assert(Version<version64>);
} // namespace version_concept_tests

namespace components_function_tests {
static_assert(std::same_as<decltype(components(version{})),
                           std::tuple<default_normal_version_number_component,
                                      default_normal_version_number_component,
                                      default_normal_version_number_component>>);
static_assert(std::get<0>(components(version{1, 2, 3})) == 1);
static_assert(std::get<1>(components(version{1, 2, 3})) == 2);
static_assert(std::get<2>(components(version{1, 2, 3})) == 3);
} // namespace components_function_tests

} // namespace verso::tests
