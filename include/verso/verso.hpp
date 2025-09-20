// Copyright (c) 2025 Andrea Ballestrazzi
#ifndef INCLUDE_VERSO_HPP
#define INCLUDE_VERSO_HPP

// C++ STL
#include <concepts>
#include <cstdint>

namespace verso {

/**
 * @brief Concept for a normal version number. Normal version number, as defined in
 *  https://semver.org/#spec-item-2, MUST be an integer and MUST NOT be negative.
 *
 *  Normal version numbers are used for the major, minor and patch version numbers.
 *
 * @tparam VersionNumberType The type to check.
 */
template <typename VersionNumberType>
concept NormalVersionNumber = std::unsigned_integral<VersionNumberType>;

// Some static assertions for documentation purposes.
static_assert(NormalVersionNumber<std::uint8_t>);
static_assert(NormalVersionNumber<std::uint16_t>);
static_assert(NormalVersionNumber<std::uint32_t>);
static_assert(NormalVersionNumber<std::uint64_t>);

/**
 * @brief Concept for version traits. Version traits MUST define the types used for the major, minor
 * and patch version numbers, and these types MUST satisfy the NormalVersionNumber concept.
 *
 * @tparam TraitsT The traits type to check.
 */
template <typename TraitsT>
concept VersionTraits = requires {
    typename TraitsT::major_t;
    typename TraitsT::minor_t;
    typename TraitsT::patch_t;

    requires NormalVersionNumber<typename TraitsT::major_t>;
    requires NormalVersionNumber<typename TraitsT::minor_t>;
    requires NormalVersionNumber<typename TraitsT::patch_t>;
};

/**
 * @brief General traits for a version. It contains the types used for the major, minor and patch
 *  and other version numbers.
 *
 * @tparam MajorT The type used for the major version number.
 * @tparam MinorT The type used for the minor version number.
 * @tparam PatchT The type used for the patch version number.
 */
template <NormalVersionNumber MajorT, NormalVersionNumber MinorT, NormalVersionNumber PatchT>
struct version_traits {
    using major_t = std::decay_t<MajorT>;
    using minor_t = std::decay_t<MinorT>;
    using patch_t = std::decay_t<PatchT>;
};

/**
 * @brief Default version traits, using the same type for major, minor and patch version numbers.
 *
 * @tparam NormalVersionNumberType The type used for the major, minor and patch version numbers.
 */
template <NormalVersionNumber NormalVersionNumberType>
using default_version_traits =
    version_traits<NormalVersionNumberType, NormalVersionNumberType, NormalVersionNumberType>;

} // namespace verso

#endif // INCLUDE_VERSO_HPP
