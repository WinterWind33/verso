// Copyright (c) 2025-2026 Andrea Ballestrazzi
#ifndef INCLUDE_VERSO_HPP
#define INCLUDE_VERSO_HPP

// C++ STL
#include <algorithm>
#include <compare>
#include <concepts>
#include <cstdint>
#include <format>
#include <optional>
#include <string>
#include <string_view>
#include <tuple>

namespace verso {

/**
 * @brief Concept for a normal version number component. A normal version number component MUST be
 * an integer and MUST NOT be negative.
 *
 *  Normal version numbers are used for the major, minor and patch version numbers.
 *
 * Reference: https://semver.org/#spec-item-2
 *
 * @tparam VersionNumberComponentType The type to check.
 */
template <typename VersionNumberComponentType>
concept NormalVersionNumberComponent = std::unsigned_integral<VersionNumberComponentType>;

// Some static assertions for documentation purposes.
static_assert(NormalVersionNumberComponent<std::uint8_t>);
static_assert(NormalVersionNumberComponent<std::uint16_t>);
static_assert(NormalVersionNumberComponent<std::uint32_t>);
static_assert(NormalVersionNumberComponent<std::uint64_t>);

/**
 * @brief Default normal version number component type, used when no other type is specified.
 */
using default_normal_version_number_component = std::uint32_t;

/**
 * @brief Concept representing a pre-release string. Pre-release identifiers are used in pre-release
 * versions, which are versions that are not considered stable and are usually used for testing or
 * development purposes.
 *
 * Pre-release data is regulated by the specification https://semver.org/#spec-item-9.
 *
 * @tparam PrereleaseStringType The type to check.
 */
template <typename PrereleaseStringType>
concept PrereleaseString = std::convertible_to<PrereleaseStringType, std::string_view>;

static_assert(PrereleaseString<std::string_view>);
static_assert(PrereleaseString<std::string>);

/**
 * @brief Concept representing a build metadata string. Build metadata is used to provide additional
 * information about a version, such as build date, commit hash, etc.. Build metadata is ignored
 * when determining version precedence.
 *
 *  Build metadata is regulated by the specification https://semver.org/#spec-item-10.
 *
 * @tparam BuildMetadataStringType The type to check.
 */
template <typename BuildMetadataStringType>
concept BuildMetadataString = std::convertible_to<BuildMetadataStringType, std::string_view>;

static_assert(BuildMetadataString<std::string_view>);
static_assert(BuildMetadataString<std::string>);

/**
 * @brief Concept for version traits. Version traits MUST define the types used for the major, minor
 * and patch version numbers, and these types MUST satisfy the NormalVersionNumberComponent concept.
 * It also defines the types used for pre-release and build metadata strings.
 *
 * @tparam TraitsT The traits type to check.
 */
template <typename TraitsT>
concept VersionTraits = requires {
    typename TraitsT::major_t;
    typename TraitsT::minor_t;
    typename TraitsT::patch_t;
    typename TraitsT::prerelease_string_t;
    typename TraitsT::build_metadata_string_t;

    requires NormalVersionNumberComponent<typename TraitsT::major_t>;
    requires NormalVersionNumberComponent<typename TraitsT::minor_t>;
    requires NormalVersionNumberComponent<typename TraitsT::patch_t>;
    requires PrereleaseString<typename TraitsT::prerelease_string_t>;
    requires BuildMetadataString<typename TraitsT::build_metadata_string_t>;
};

/**
 * @brief General traits for a version. It contains the types used for the major, minor and patch
 *  and other version numbers.
 *
 * @tparam MajorT The type used for the major version number.
 * @tparam MinorT The type used for the minor version number.
 * @tparam PatchT The type used for the patch version number.
 * @tparam PrereleaseStringT The type used for the pre-release string. It MUST satisfy the
 *  PrereleaseString concept. It defaults to std::string.
 * @tparam BuildMetadataStringT The type used for the build metadata string. It MUST satisfy the
 *  BuildMetadataString concept. It defaults to std::string.
 */
template <NormalVersionNumberComponent MajorT, NormalVersionNumberComponent MinorT,
          NormalVersionNumberComponent PatchT, PrereleaseString PrereleaseStringT = std::string,
          BuildMetadataString BuildMetadataStringT = std::string>
struct version_traits {
    using major_t = std::decay_t<MajorT>;
    using minor_t = std::decay_t<MinorT>;
    using patch_t = std::decay_t<PatchT>;
    using prerelease_string_t = std::decay_t<PrereleaseStringT>;
    using build_metadata_string_t = std::decay_t<BuildMetadataStringT>;
};

/**
 * @brief Default version traits, using the same type for major, minor and patch version numbers.
 *
 * @tparam NormalVersionNumberComponentType The type used for the major, minor and patch version
 *  numbers.
 */
template <NormalVersionNumberComponent NormalVersionNumberComponentType>
using default_version_traits =
    version_traits<NormalVersionNumberComponentType, NormalVersionNumberComponentType,
                   NormalVersionNumberComponentType>;

/**
 * @brief Main version class, parametrized by the version traits.
 *
 * @tparam TraitsT The version traits type. It MUST satisfy the VersionTraits concept.
 */
template <typename TraitsT>
    requires VersionTraits<TraitsT>
struct basic_version {
    using traits_t = std::decay_t<TraitsT>;
    using major_t = typename traits_t::major_t;
    using minor_t = typename traits_t::minor_t;
    using patch_t = typename traits_t::patch_t;
    using prerelease_string_t = typename traits_t::prerelease_string_t;
    using build_metadata_string_t = typename traits_t::build_metadata_string_t;

    /**
     * @brief Major version number.
     */
    major_t major{};

    /**
     * @brief Minor version number.
     */
    minor_t minor{};

    /**
     * @brief Patch version number.
     */
    patch_t patch{};

    /**
     * @brief Pre-release string. This is an optional field that can be used to indicate that a
     * version is a pre-release version. If this field is not empty, the version is considered a
     * pre-release version and has lower precedence than the same version without the pre-release
     * string.
     */
    std::optional<prerelease_string_t> prerelease_data{};

    /**
     * @brief Build metadata string. This is an optional field that can be used to indicate
     * additional build information. If this field is not empty, it does not affect the version
     * precedence.
     */
    std::optional<build_metadata_string_t> build_metadata{};

    constexpr bool operator==(const basic_version& other) const noexcept = default;
};

/**
 * @brief Concept for a version supported by this library. A version MUST have the following
 * members:
 * - major: the major version number, of a type that satisfies the NormalVersionNumberComponent
 * concept.
 * - minor: the minor version number, of a type that satisfies the NormalVersionNumberComponent
 * concept.
 * - patch: the patch version number, of a type that satisfies the NormalVersionNumberComponent
 * concept.
 * - prerelease_data: an optional field that can be used to indicate that a version is a pre-release
 * version, of a type that satisfies the PrereleaseString concept.
 * - build_metadata: an optional field that can be used to indicate additional build information, of
 * a type that satisfies the BuildMetadataString concept.
 *
 * @tparam VersionT The type to check.
 */
template <typename VersionT>
concept Version = requires {
    typename VersionT::major_t;
    typename VersionT::minor_t;
    typename VersionT::patch_t;
    typename VersionT::prerelease_string_t;
    typename VersionT::build_metadata_string_t;

    requires NormalVersionNumberComponent<typename VersionT::major_t>;
    requires NormalVersionNumberComponent<typename VersionT::minor_t>;
    requires NormalVersionNumberComponent<typename VersionT::patch_t>;
    requires PrereleaseString<typename VersionT::prerelease_string_t>;
    requires BuildMetadataString<typename VersionT::build_metadata_string_t>;
} && requires(VersionT v) {
    { v.major } -> std::same_as<typename VersionT::major_t&>;
    { v.minor } -> std::same_as<typename VersionT::minor_t&>;
    { v.patch } -> std::same_as<typename VersionT::patch_t&>;
    { v.prerelease_data } -> std::same_as<std::optional<typename VersionT::prerelease_string_t>&>;
    {
        v.build_metadata
    } -> std::same_as<std::optional<typename VersionT::build_metadata_string_t>&>;
};

/**
 * @brief Alias for version which has all the version numbers with the same types.
 *
 * @tparam NormalVersionNumberComponentT The type of the version numbers.
 */
template <NormalVersionNumberComponent NormalVersionNumberComponentT>
using uniform_version = basic_version<default_version_traits<NormalVersionNumberComponentT>>;

/**
 * @brief Special alias for version which has all the version numbers with the same types and all
 * the other components as constant strings. This can be useful to define versions that are known at
 * compile time.
 *
 * @note Be careful using this on runtime contexts, because prerelease and build metadata data are
 * stored as string views, so they must point to valid strings for the entire lifetime of the
 * version object.
 *
 * @tparam NormalVersionNumberComponentT The type of the version numbers.
 */
template <NormalVersionNumberComponent NormalVersionNumberComponentT>
using uniform_constant_version = basic_version<
    version_traits<NormalVersionNumberComponentT, NormalVersionNumberComponentT,
                   NormalVersionNumberComponentT, std::string_view, std::string_view>>;

/**
 * @brief Default specialization of basic_version with the default normal
 *  version number (std::uint32_t)
 */
using version = uniform_version<default_normal_version_number_component>;
static_assert(Version<version>);

/**
 * @brief Special alias for a version with all the version numbers as the default normal version
 * number component and all the other components as constant strings. This can be useful to define
 * versions that are known at compile time.
 *
 * @note Be careful using this on runtime contexts, because prerelease and build metadata data are
 * stored as string views, so they must point to valid strings for the entire lifetime of the
 * version object.
 */
using constant_version = uniform_constant_version<default_normal_version_number_component>;

/**
 * @brief Version with all normal version numbers type equal to std::uint8_t.
 */
using version8 = uniform_version<std::uint8_t>;

/**
 * @brief Version with all normal version numbers type equal to std::uint16_t.
 */
using version16 = uniform_version<std::uint16_t>;

/**
 * @brief Version with all normal version numbers type equal to std::uint32_t.
 */
using version32 = uniform_version<std::uint32_t>;

/**
 * @brief Version with all normal version numbers type equal to std::uint64_t.
 */
using version64 = uniform_version<std::uint64_t>;

/**
 * @brief Get the components of a version as a tuple (major, minor, patch, prerelease_data,
 * build_metadata).
 *
 * @tparam VersionT A type that satisfies the Version concept.
 * @param version The version instance.
 * @return A tuple containing the major, minor, patch version numbers, prerelease data, and build
 * metadata.
 */
constexpr auto components(const Version auto& version) {
    return std::make_tuple(version.major, version.minor, version.patch, version.prerelease_data,
                           version.build_metadata);
}

// ### This library version ###

/**
 * @brief The current version of the library, currently in development.
 *  To honor this library, when not in development, the library version will always be represented
 *  only by a "version" object.
 */
constexpr constant_version verso_version{0, 3, 0, "dev"};

// ### Comparison operators ###
// Reference: https://semver.org/#spec-item-11
// All the following operators strictly follow the specification.

/**
 * @brief Less-than comparison operator for versions.
 *
 * @param lhs The left-hand side version.
 * @param rhs The right-hand side version.
 * @return true if lhs is less than rhs, false otherwise.
 */
constexpr bool operator<(const Version auto& lhs, const Version auto& rhs) noexcept {
    if (lhs.major != rhs.major) {
        return lhs.major < rhs.major;
    }
    if (lhs.minor != rhs.minor) {
        return lhs.minor < rhs.minor;
    }
    return lhs.patch < rhs.patch;
}

// Static assertions for documentation purposes.
static_assert(version{} < version{1, 0, 0});
static_assert(version{1, 0, 0} < version{1, 1, 0});
static_assert(version{1, 1, 0} < version{1, 1, 1});
static_assert(version{1, 1, 1} < version{2, 0, 0});

/**
 * @brief Less-than-or-equal comparison operator for versions.
 *
 * @param lhs The left-hand side version.
 * @param rhs The right-hand side version.
 * @return true if lhs is less than or equal to rhs, false otherwise.
 */
constexpr bool operator<=(const Version auto& lhs, const Version auto& rhs) noexcept {
    return (lhs < rhs) || (lhs == rhs);
}

// Static assertions for documentation purposes.
static_assert(version{} <= version{});
static_assert(version{} <= version{1, 0, 0});
static_assert(version{1, 0, 0} <= version{1, 0, 0});
static_assert(version{1, 0, 0} <= version{1, 1, 0});
static_assert(version{1, 1, 0} <= version{1, 1, 0});
static_assert(version{1, 1, 1} <= version{1, 1, 1});
static_assert(version{1, 1, 1} <= version{2, 0, 0});

/**
 * @brief Greater-than-or-equal comparison operator for versions.
 *
 * @param lhs The left-hand side version.
 * @param rhs The right-hand side version.
 * @return true if lhs is greater than or equal to rhs, false otherwise.
 */
constexpr bool operator>=(const Version auto& lhs, const Version auto& rhs) noexcept {
    return !(lhs < rhs);
}

// Static assertions for documentation purposes.
static_assert(version{} >= version{});
static_assert(version{1, 0, 0} >= version{});
static_assert(version{1, 0, 0} >= version{1, 0, 0});
static_assert(version{1, 1, 0} >= version{1, 0, 0});
static_assert(version{1, 1, 0} >= version{1, 1, 0});
static_assert(version{1, 1, 1} >= version{1, 1, 0});
static_assert(version{1, 1, 1} >= version{1, 1, 1});
static_assert(version{2, 0, 0} >= version{1, 1, 1});

/**
 * @brief Greater-than comparison operator for versions.
 *
 * @param lhs The left-hand side version.
 * @param rhs The right-hand side version.
 * @return true if lhs is greater than rhs, false otherwise.
 */
constexpr bool operator>(const Version auto& lhs, const Version auto& rhs) noexcept {
    return !(lhs <= rhs);
}

// Static assertions for documentation purposes.
static_assert(version{1, 0, 0} > version{});
static_assert(version{1, 0, 0} > version{0, 1, 0});
static_assert(version{1, 1, 0} > version{1, 0, 0});
static_assert(version{1, 1, 1} > version{1, 1, 0});
static_assert(version{2, 0, 0} > version{1, 1, 1});

/**
 * @brief Three-way comparison operator for versions. This operator returns a std::strong_ordering
 * value that indicates the relative order of the two versions.
 *
 * @param lhs The left-hand side version.
 * @param rhs The right-hand side version.
 * @return A std::strong_ordering value indicating the relative order of the two versions.
 */
constexpr std::strong_ordering operator<=>(const Version auto& lhs,
                                           const Version auto& rhs) noexcept {
    if (lhs.major != rhs.major) {
        return lhs.major <=> rhs.major;
    }
    if (lhs.minor != rhs.minor) {
        return lhs.minor <=> rhs.minor;
    }
    return lhs.patch <=> rhs.patch;
}

static_assert((version{} <=> version{}) == std::strong_ordering::equal);
static_assert((version{} <=> version{1, 0, 0}) == std::strong_ordering::less);
static_assert((version{1, 0, 0} <=> version{1, 0, 0}) == std::strong_ordering::equal);
static_assert((version{1, 0, 0} <=> version{1, 1, 0}) == std::strong_ordering::less);
static_assert((version{1, 1, 0} <=> version{1, 1, 0}) == std::strong_ordering::equal);
static_assert((version{1, 1, 1} <=> version{1, 1, 0}) == std::strong_ordering::greater);
static_assert((version{1, 1, 1} <=> version{1, 1, 1}) == std::strong_ordering::equal);
static_assert((version{1, 1, 1} <=> version{2, 0, 0}) == std::strong_ordering::less);

// ### To and from string functions ###

/**
 * @brief Semantic version string separator.
 *
 * Reference: https://semver.org/#spec-item-2
 */
constexpr char VERSION_STRING_SEPARATOR{'.'};

/**
 * @brief Convert a version to a string in the format "major.minor.patch".
 *  This follows the semantic versioning specification which states that
 *  version numbers MUST NOT contain leading zeroes.
 *
 * Reference: https://semver.org/#spec-item-2
 *
 * @param version The version to convert.
 * @return A string representation of the version.
 */
auto to_string(const Version auto& version) {
    return std::format("{}{}{}{}{}", version.major, VERSION_STRING_SEPARATOR, version.minor,
                       VERSION_STRING_SEPARATOR, version.patch);
}

namespace details {
// Helper function to check if a character is a valid digit for a version number.
constexpr bool is_valid_version_number_digit(const char c) noexcept {
    return c >= '0' && c <= '9';
}

static_assert(is_valid_version_number_digit('0'));
static_assert(is_valid_version_number_digit('9'));
static_assert(!is_valid_version_number_digit('a'));
static_assert(!is_valid_version_number_digit(' '));
static_assert(!is_valid_version_number_digit('-'));

// Helper function to check if a string_view represents a valid normal version number
// (i.e., a non-negative integer without leading zeros unless it's "0").
constexpr bool is_valid_normal_version_number(const std::string_view str) {
    if (str.empty()) {
        return false;
    }
    if (str.size() > 1 && str[0] == '0') {
        return false; // Leading zero
    }
    for (const char c : str) {
        if (!is_valid_version_number_digit(c)) {
            return false;
        }
    }
    return true;
}

static_assert(is_valid_normal_version_number("0"));
static_assert(is_valid_normal_version_number("1"));
static_assert(is_valid_normal_version_number("1234567890"));
static_assert(!is_valid_normal_version_number(""));
static_assert(!is_valid_normal_version_number("01"));
static_assert(!is_valid_normal_version_number("a123"));
static_assert(!is_valid_normal_version_number("-23"));

} // namespace details

/**
 * @brief Convert a string to a version. The string MUST be in the format "major.minor.patch",
 *  where major, minor and patch are non-negative integers without leading zeros unless the number
 * is zero. If the given string is not valid, returns std::nullopt.
 *
 * @tparam VersionT A type that satisfies the Version concept.
 * @param versionStr The string to convert.
 * @return An optional containing the version if the conversion was successful, std::nullopt
 * otherwise.
 */
template <Version VersionT>
constexpr std::optional<VersionT> from_string(const std::string_view versionStr) {
    if (versionStr.empty()) {
        return std::nullopt;
    }

    std::optional<typename VersionT::major_t> major{};
    std::optional<typename VersionT::minor_t> minor{};
    std::optional<typename VersionT::patch_t> patch{};
    std::string_view::const_iterator startIt{std::cbegin(versionStr)};
    std::string_view::const_iterator it{
        std::find(startIt, std::cend(versionStr), VERSION_STRING_SEPARATOR)};
    for (; it != std::cend(versionStr);
         it = std::find(startIt, std::cend(versionStr), VERSION_STRING_SEPARATOR)) {
        const std::string_view token{startIt, it};
        if (!details::is_valid_normal_version_number(token)) {
            // If it's not a valid normal version number, return std::nullopt directly.
            return std::nullopt;
        }

        // Otherwise, convert it to the appropriate type.
        if (!major) {
            major = static_cast<typename VersionT::major_t>(std::stoul(std::string(token)));
        } else if (!minor) {
            minor = static_cast<typename VersionT::minor_t>(std::stoul(std::string(token)));
        } else {
            // More than 3 components, invalid version string.
            return std::nullopt;
        }

        // It's safe here to increment it because we can be here only if it != <end>.
        startIt = it + 1;
    }

    // Handle the last component (or the only one if there are no dots).
    if (const std::string_view lastToken{startIt, std::cend(versionStr)};
        details::is_valid_normal_version_number(lastToken)) {
        // This should only be the patch number because we suppose that major and minor are already
        // set.
        patch = static_cast<typename VersionT::patch_t>(std::stoul(std::string(lastToken)));
    }

    if (major && minor && patch) {
        return VersionT{*major, *minor, *patch};
    }

    return std::nullopt;
}

static_assert(from_string<version>("") == std::nullopt);
} // namespace verso

#endif // INCLUDE_VERSO_HPP
