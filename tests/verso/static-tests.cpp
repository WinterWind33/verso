// Copyright (c) 2025-2026 Andrea Ballestrazzi
#include "verso/verso.hpp"

// C++ STL
#include <string_view>

// clang rejects the destructor of a std::optional holding non-trivially-destructible members
// inside a constant expression when the MSVC STL is in use. Assertions that evaluate such an
// optional cannot be compiled in that configuration.
#if defined(__clang__) && defined(_MSC_VER)
#define VERSO_NO_CONSTEXPR_NON_TRIVIAL_OPTIONAL
#endif

// This file is used to perform static assertions on concepts and other compile-time
namespace verso::tests {

static_assert(verso_version.major() == 0);
static_assert(verso_version.minor() == 3);
static_assert(verso_version.patch() == 0);

namespace library_implementation_tests {

static_assert(details::is_valid_version_number_digit('0'));
static_assert(details::is_valid_version_number_digit('9'));
static_assert(!details::is_valid_version_number_digit('a'));
static_assert(!details::is_valid_version_number_digit(' '));
static_assert(!details::is_valid_version_number_digit('-'));

static_assert(details::is_number("0"));
static_assert(details::is_number("1234567890"));
static_assert(details::is_number("0000000000"));
static_assert(details::is_number("01"));
static_assert(details::is_number("00000001"));
static_assert(!details::is_number(""));

static_assert(details::trim_leading_zeros("0") == "0");
static_assert(details::trim_leading_zeros("0000000000") == "0");
static_assert(details::trim_leading_zeros("00000001") == "1");
static_assert(details::trim_leading_zeros("000000001234567890") == "1234567890");
static_assert(details::trim_leading_zeros("1234567890") == "1234567890");

static_assert(details::is_number_str_numerically_lower_than("1", "2"));
static_assert(details::is_number_str_numerically_lower_than("1", "10"));
static_assert(details::is_number_str_numerically_lower_than("001", "002"));
static_assert(!details::is_number_str_numerically_lower_than("2", "1"));
static_assert(!details::is_number_str_numerically_lower_than("10", "1"));
static_assert(!details::is_number_str_numerically_lower_than("001", "0001"));

static_assert(details::is_numeric_identifier<std::string_view>("0"));
static_assert(details::is_numeric_identifier<std::string_view>("1234567890"));
static_assert(!details::is_numeric_identifier<std::string_view>(""));
static_assert(!details::is_numeric_identifier<std::string_view>("a123"));
static_assert(!details::is_numeric_identifier<std::string_view>("-23"));
static_assert(!details::is_numeric_identifier<std::string_view>("12.3"));
static_assert(!details::is_numeric_identifier<std::string_view>("01"));

static_assert(details::is_alnum_or_hyphen('a'));
static_assert(details::is_alnum_or_hyphen('Z'));
static_assert(details::is_alnum_or_hyphen('0'));
static_assert(details::is_alnum_or_hyphen('-'));
static_assert(!details::is_alnum_or_hyphen(' '));
static_assert(!details::is_alnum_or_hyphen('!'));
static_assert(!details::is_alnum_or_hyphen(VERSION_STRING_SEPARATOR));

static_assert(details::is_identifier_valid<std::string_view>("0", false));
static_assert(details::is_identifier_valid<std::string_view>("alpha", false));
static_assert(details::is_identifier_valid<std::string_view>("alpha-1", false));
static_assert(details::is_identifier_valid<std::string_view>("01alpha", false));
static_assert(details::is_identifier_valid<std::string_view>("0alpha", false));
static_assert(!details::is_identifier_valid<std::string_view>("", false));
static_assert(!details::is_identifier_valid<std::string_view>("01", false));
static_assert(!details::is_identifier_valid<std::string_view>("00000001", false));
static_assert(!details::is_identifier_valid<std::string_view>("+1", false));
static_assert(!details::is_identifier_valid<std::string_view>("1gfd!.", false));
static_assert(details::is_identifier_valid<std::string_view>("alpha", true));
static_assert(details::is_identifier_valid<std::string_view>("alpha-1", true));
static_assert(details::is_identifier_valid<std::string_view>("01", true));
static_assert(details::is_identifier_valid<std::string_view>("00000001", true));
static_assert(!details::is_identifier_valid<std::string_view>("", false));

static_assert(details::check_string_identifiers<std::string_view>("alpha.1", false));
static_assert(details::check_string_identifiers<std::string_view>("beta-54", false));
static_assert(details::check_string_identifiers<std::string_view>("01.0.0", true));
static_assert(!details::check_string_identifiers<std::string_view>("", false));
static_assert(!details::check_string_identifiers<std::string_view>("alpha..1", false));
static_assert(!details::check_string_identifiers<std::string_view>("alpha.01", false));
static_assert(!details::check_string_identifiers<std::string_view>("alpha.", false));
static_assert(!details::check_string_identifiers<std::string_view>(".alpha", false));

static_assert(details::is_prerelease_strictly_lower_than<std::string_view>("alpha", "beta"));
static_assert(details::is_prerelease_strictly_lower_than<std::string_view>("alpha", "alpha.1"));
static_assert(details::is_prerelease_strictly_lower_than<std::string_view>("alpha.1", "alpha.2"));
static_assert(details::is_prerelease_strictly_lower_than<std::string_view>("alpha.1",
                                                                           "alpha.beta"));
static_assert(details::is_prerelease_strictly_lower_than<std::string_view>("13", "alpha"));
static_assert(details::is_prerelease_strictly_lower_than<std::string_view>("13", "001alpha"));
static_assert(details::is_prerelease_strictly_lower_than<std::string_view>("13", "45"));
static_assert(details::is_prerelease_strictly_lower_than<std::string_view>("beta.rc.1",
                                                                           "beta.rc.3A"));
static_assert(details::is_prerelease_strictly_lower_than<std::string_view>("alpha.beta", "beta"));
static_assert(!details::is_prerelease_strictly_lower_than<std::string_view>("001alpha", "42"));
static_assert(!details::is_prerelease_strictly_lower_than<std::string_view>("alpha.beta.54",
                                                                            "alpha.beta.54"));

static_assert(details::is_valid_char_for_version_string('a'));
static_assert(details::is_valid_char_for_version_string('Z'));
static_assert(details::is_valid_char_for_version_string('0'));
static_assert(details::is_valid_char_for_version_string('-'));
static_assert(details::is_valid_char_for_version_string(VERSION_STRING_SEPARATOR));
static_assert(details::is_valid_char_for_version_string('+'));
static_assert(!details::is_valid_char_for_version_string(' '));
static_assert(!details::is_valid_char_for_version_string('!'));

static_assert(details::is_valid_version_string("1.0.0"));
static_assert(details::is_valid_version_string("1.0.0-alpha"));
static_assert(details::is_valid_version_string("1.0.0+build.123"));
static_assert(!details::is_valid_version_string(""));
static_assert(!details::is_valid_version_string("1.0.0 "));
static_assert(!details::is_valid_version_string("1.0.0!"));

} // namespace library_implementation_tests

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

namespace constant_version_concept_tests {
static_assert(Version<constant_version>);
} // namespace constant_version_concept_tests

namespace components_function_tests {
static_assert(std::same_as<decltype(components(version{})),
                           std::tuple<default_normal_version_number_component,
                                      default_normal_version_number_component,
                                      default_normal_version_number_component,
                                      std::optional<std::string>, std::optional<std::string>>>);
static_assert(std::get<0>(components(version{1, 2, 3})) == 1);
static_assert(std::get<1>(components(version{1, 2, 3})) == 2);
static_assert(std::get<2>(components(version{1, 2, 3})) == 3);

static_assert(
    std::same_as<
        decltype(components(constant_version{})),
        std::tuple<default_normal_version_number_component, default_normal_version_number_component,
                   default_normal_version_number_component, std::optional<std::string_view>,
                   std::optional<std::string_view>>>);
static_assert(std::get<0>(components(constant_version{1, 2, 3, "alpha", "build"})) == 1);
static_assert(std::get<1>(components(constant_version{1, 2, 3, "alpha", "build"})) == 2);
static_assert(std::get<2>(components(constant_version{1, 2, 3, "alpha", "build"})) == 3);
static_assert(std::get<3>(components(constant_version{1, 2, 3, "alpha", "build"})) == "alpha");
static_assert(std::get<4>(components(constant_version{1, 2, 3, "alpha", "build"})) == "build");
} // namespace components_function_tests

#ifndef VERSO_NO_CONSTEXPR_NON_TRIVIAL_OPTIONAL
namespace from_string_function_tests {
static_assert(from_string<version>("") == std::nullopt);
static_assert(from_string<version>("1") == std::nullopt);
static_assert(from_string<version>("1.0") == std::nullopt);
static_assert(from_string<version>("1.0.0.0") == std::nullopt);
static_assert(from_string<version>("....") == std::nullopt);
static_assert(from_string<version>("junk-data") == std::nullopt);
} // namespace from_string_function_tests
#endif // VERSO_NO_CONSTEXPR_NON_TRIVIAL_OPTIONAL

} // namespace verso::tests
