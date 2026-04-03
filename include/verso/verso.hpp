// Copyright (c) 2025-2026 Andrea Ballestrazzi
#ifndef INCLUDE_VERSO_HPP
#define INCLUDE_VERSO_HPP

// C++ STL
#include <algorithm>
#include <compare>
#include <concepts>
#include <cstdint>
#include <format>
#include <numeric> // for std::accumulate
#include <optional>
#include <ranges>
#include <stdexcept>
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
 * @brief Concept for a supported string type in this library. Supported string types are those that
 * can be used for pre-release and build metadata strings, and they MUST be convertible to
 * std::string_view.
 *
 * @tparam SupportedStringType The type to check.
 */
template <typename SupportedStringType>
concept SupportedString = std::convertible_to<SupportedStringType, std::string_view>;

static_assert(SupportedString<std::string_view>);
static_assert(SupportedString<std::string>);

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
concept PrereleaseString = SupportedString<PrereleaseStringType>;

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
concept BuildMetadataString = SupportedString<BuildMetadataStringType>;

static_assert(BuildMetadataString<std::string_view>);
static_assert(BuildMetadataString<std::string>);

/**
 * @brief Concept for version traits. Version traits MUST define the types used for the major, minor
 * and patch numbers, along with pre-release and build metadata strings, and these types MUST
 * satisfy their respective concepts.
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
 * This concept enforces only getters for these members, but the actual version class can have
 * setters as well, as long as it satisfies the requirements of this concept.
 *
 * @tparam VersionT The type to check.
 */
template <typename VersionT>
concept Version =
    requires {
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
    } &&
    requires(VersionT v) {
        { v.major() } -> std::same_as<typename VersionT::major_t>;
        { v.minor() } -> std::same_as<typename VersionT::minor_t>;
        { v.patch() } -> std::same_as<typename VersionT::patch_t>;
        {
            v.prerelease_data()
        } -> std::same_as<std::optional<typename VersionT::prerelease_string_t>>;
        {
            v.build_metadata()
        } -> std::same_as<std::optional<typename VersionT::build_metadata_string_t>>;
    } &&
    // A version supported by verso should be constructible with all the sem ver components, in
    // various ways.
    requires(typename VersionT::major_t major, typename VersionT::minor_t minor,
             typename VersionT::patch_t patch,
             std::optional<typename VersionT::prerelease_string_t> prerelease_data,
             std::optional<typename VersionT::build_metadata_string_t> build_metadata) {
        { VersionT{major, minor, patch} } -> std::same_as<VersionT>;
        { VersionT{major, minor, patch, prerelease_data} } -> std::same_as<VersionT>;
        {
            VersionT{major, minor, patch, prerelease_data, build_metadata}
        } -> std::same_as<VersionT>;
    };

/**
 * @brief Semantic version string separator.
 *
 * Reference: https://semver.org/#spec-item-2
 */
constexpr char VERSION_STRING_SEPARATOR{'.'};

namespace details {

// Helper function to check if a character is a valid digit for a version number.
constexpr bool is_valid_version_number_digit(const char c) noexcept {
    return c >= '0' && c <= '9';
}

/**
 * @brief Check if an identifier is numeric.
 *
 * @param identifier The identifier string.
 * @return true if the identifier is numeric, false otherwise.
 */
constexpr bool is_numeric_identifier(const SupportedString auto& identifier) {
    if (identifier.empty()) {
        return false;
    }

    // The Backus-Naur Form for a numeric identifier is the following:
    // <numeric identifier> ::= "0"
    //                | <positive digit>
    //                | <positive digit><digits>
    // So we cannot have leading zeros nor negative numbers, and we must have at least one digit.

    if (identifier.size() > 1 && identifier[0] == '0') {
        return false; // Leading zero
    }

    for (const char c : identifier) {
        if (!is_valid_version_number_digit(c)) {
            return false;
        }
    }
    return true;
}

/**
 * @brief Check if a character is an alphanumeric character or a hyphen, which are the only valid
 * characters for identifiers in pre-release and build metadata strings, according to the
 * specification https://semver.org/#spec-item-9 and https://semver.org/#spec-item-10.
 *
 * @param c The character to check.
 * @return true if the character is an alphanumeric character or a hyphen, false otherwise.
 */
constexpr bool is_alnum_or_hyphen(const char c) noexcept {
    return c == '-' || (c >= '0' && c <= '9') || (c >= 'A' && c <= 'Z') || (c >= 'a' && c <= 'z');
}

/**
 * @brief Check if an identifier is valid according to the specification
 * https://semver.org/#spec-item-9 and https://semver.org/#spec-item-10. An identifier is valid if
 * it is a non-empty string of alphanumeric characters and hyphens.
 *
 * @param id The identifier string.
 * @param canIncludeLeadingZeroes Whether leading zeroes are allowed for numeric identifiers.
 * @return true if the identifier is valid, false otherwise.

 */
constexpr bool is_identifier_valid(const SupportedString auto& id,
                                   const bool canIncludeLeadingZeroes) noexcept {
    if (id.empty()) {
        return false; // Identifiers must not be empty.
    }

    if (!canIncludeLeadingZeroes && id.size() > 1 && id[0] == '0') {
        return false; // Leading zeroes are not allowed for normal version numbers.
    }

    for (const char c : id) {
        if (!details::is_alnum_or_hyphen(c)) {
            return false; // Identifiers must consist of alphanumeric characters and hyphens only.
        }
    }
    return true;
}

/**
 * @brief Check if a string of dot-separated identifiers is valid according to the specification.
 *
 * @param str The string to check.
 * @param canIncludeLeadingZeroes Whether leading zeroes are allowed for numeric identifiers.
 * @return true if the string is valid, false otherwise.
 */
constexpr bool check_string_identifiers(const SupportedString auto& str,
                                        const bool canIncludeLeadingZeroes) noexcept {
    auto parts =
        str | std::views::split(VERSION_STRING_SEPARATOR) | std::views::transform([](auto&& part) {
            return std::string_view{part.data(), part.size()};
        });
    if (parts.empty()) {
        return false; // The string must not be empty.
    }

    for (const std::string_view identifier : parts) {
        if (!details::is_identifier_valid(identifier, canIncludeLeadingZeroes)) {
            return false; // Identifiers must be valid according to the specification.
        }
    }
    return true;
}

} // namespace details

/**
 * @brief Main version class, parametrized by the version traits.
 *
 * @tparam TraitsT The version traits type. It MUST satisfy the VersionTraits concept.
 */
template <typename TraitsT>
    requires VersionTraits<TraitsT>
class basic_version {
public:
    using traits_t = std::decay_t<TraitsT>;
    using major_t = typename traits_t::major_t;
    using minor_t = typename traits_t::minor_t;
    using patch_t = typename traits_t::patch_t;
    using prerelease_string_t = typename traits_t::prerelease_string_t;
    using build_metadata_string_t = typename traits_t::build_metadata_string_t;

    /**
     * @brief Constructs a zeroed version with no pre-release data and no build metadata.
     */
    constexpr basic_version() noexcept = default;

    /**
     * @brief Constructs a version with the given major, minor and patch version numbers, and
     * optional pre-release data and build metadata. The version is validated according to the
     * specification, and if it is not valid, an std::invalid_argument exception is thrown.
     *
     * @param major The major version number.
     * @param minor The minor version number.
     * @param patch The patch version number.
     * @param prerelease_data The pre-release data string. This is an optional field that can be
     * used to indicate that a version is a pre-release version.
     * @param build_metadata The build metadata string. This is an optional field that can be used
     * to indicate additional build information.
     */
    constexpr basic_version(
        const major_t major, const minor_t minor, const patch_t patch,
        const std::optional<prerelease_string_t> prerelease_data = std::nullopt,
        const std::optional<build_metadata_string_t> build_metadata = std::nullopt)
        : m_major{major},
          m_minor{minor},
          m_patch{patch},
          m_prerelease_data{prerelease_data},
          m_build_metadata{build_metadata} {
        if (!is_valid()) {
            throw std::invalid_argument(
                "Invalid version format. Please, check pre-release and build metadata format "
                "according "
                "to the specification.");
        }
    }

    /**
     * @brief Retrieves the major version number.
     *
     * @return major_t The major version number.
     */
    constexpr major_t major() const noexcept {
        return m_major;
    }

    /**
     * @brief Retrieves the minor version number.
     *
     * @return minor_t The minor version number.
     */
    constexpr minor_t minor() const noexcept {
        return m_minor;
    }

    /**
     * @brief Retrieves the patch version number.
     *
     * @return patch_t The patch version number.
     */
    constexpr patch_t patch() const noexcept {
        return m_patch;
    }

    /**
     * @brief Retrieves the pre-release data string.
     *
     * @return std::optional<prerelease_string_t> The pre-release data string.
     */
    constexpr std::optional<prerelease_string_t> prerelease_data() const noexcept {
        return m_prerelease_data;
    }

    /**
     * @brief Retrieves the build metadata string.
     *
     * @return std::optional<build_metadata_string_t> The build metadata string.
     */
    constexpr std::optional<build_metadata_string_t> build_metadata() const noexcept {
        return m_build_metadata;
    }

    /**
     * @brief Equality operator. Two versions are considered equal if their major, minor, patch and
     * pre-release data are equal. Build metadata is ignored for equality and precedence purposes,
     * as per specification https://semver.org/#spec-item-10.
     *
     * @param other The other version to compare with.
     * @return true if the versions are equal, false otherwise.
     */
    constexpr bool operator==(const basic_version& other) const noexcept {
        return m_major == other.m_major && m_minor == other.m_minor && m_patch == other.m_patch &&
               m_prerelease_data == other.m_prerelease_data;
    }

    /**
     * @brief Sets the major version number.
     *
     * @param major The major version number to set.
     */
    constexpr void major(const major_t major) noexcept {
        m_major = major;
    }

    /**
     * @brief Sets the minor version number.
     *
     * @param minor The minor version number to set.
     */
    constexpr void minor(const minor_t minor) noexcept {
        m_minor = minor;
    }

    /**
     * @brief Sets the patch version number.
     *
     * @param patch The patch version number to set.
     */
    constexpr void patch(const patch_t patch) noexcept {
        m_patch = patch;
    }

    /**
     * @brief Sets the pre-release data string. The pre-release data string is validated according
     * to the specification, and if it is not valid, an std::invalid_argument exception is thrown.
     *
     * Refer to https://semver.org/#spec-item-9 for the pre-release data format specification.
     *
     * @param prerelease_data The pre-release data string to set. This is an optional field that can
     * be used to indicate that a version is a pre-release version.
     */
    constexpr void prerelease_data(std::optional<prerelease_string_t> prerelease_data) {
        if (prerelease_data &&
            !details::check_string_identifiers(prerelease_data.value(),
                                               /* can include leading zeros */ false)) {
            throw std::invalid_argument(
                "Invalid pre-release data format. Please, check pre-release data format according "
                "to the specification.");
        }
        m_prerelease_data = std::move(prerelease_data);
    }

    /**
     * @brief Sets the build metadata string. The build metadata string is validated according to
     * the specification, and if it is not valid, an std::invalid_argument exception is thrown.
     *
     * Refer to https://semver.org/#spec-item-10 for the build metadata format specification.
     *
     * @param build_metadata The build metadata string to set. This is an optional field that can be
     * used to indicate additional build information.
     */
    constexpr void build_metadata(std::optional<build_metadata_string_t> build_metadata) {
        if (build_metadata &&
            !details::check_string_identifiers(build_metadata.value(),
                                               /* can include leading zeros */ true)) {
            throw std::invalid_argument(
                "Invalid build metadata format. Please, check build metadata format according "
                "to the specification.");
        }
        m_build_metadata = std::move(build_metadata);
    }

private:
    major_t m_major{};
    minor_t m_minor{};
    patch_t m_patch{};
    std::optional<prerelease_string_t> m_prerelease_data{};
    std::optional<build_metadata_string_t> m_build_metadata{};

    // Performs validation of the version according to the specification.
    constexpr bool is_valid() const noexcept {
        // A version is valid if its major, minor and patch version numbers are valid, and if its
        // pre-release data and build metadata are valid according to the specification.

        // Check if pre-release data is valid. It must consist of dot-separated identifiers, where
        // each identifier is either a non-empty string of alphanumeric characters and hyphens, or a
        // numeric identifier.
        if (m_prerelease_data &&
            !details::check_string_identifiers(m_prerelease_data.value(),
                                               /* can include leading zeros */ false)) {
            return false; // Identifiers must be valid according to the specification.
        }

        // Check if build metadata is valid. It must consist of dot-separated identifiers, where
        // each identifier is a non-empty string of alphanumeric characters and hyphens.
        if (m_build_metadata &&
            !details::check_string_identifiers(m_build_metadata.value(),
                                               /* can include leading zeros */ true)) {
            return false; // Identifiers must be valid according to the specification.
        }

        return true;
    }
};

/**
 * @brief Alias for version which has all the version numbers with the same types.
 *
 * @tparam NormalVersionNumberComponentT The type of the version numbers.
 */
template <NormalVersionNumberComponent NormalVersionNumberComponentT>
using uniform_version = basic_version<default_version_traits<NormalVersionNumberComponentT>>;

/**
 * @brief Special alias for version which has all the version numbers with the same types and
 * pre-release and build metadata as std::string.
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
 * immutable versions that are known at compile time.
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
    return std::make_tuple(version.major(), version.minor(), version.patch(),
                           version.prerelease_data(), version.build_metadata());
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

namespace details {
constexpr bool is_prelease_strictly_lower_than(const PrereleaseString auto& lhs,
                                               const PrereleaseString auto& rhs) {
    // Precedence for two pre-release versions with the same major, minor, and patch version MUST be
    // determined by comparing each dot separated identifier from left to right until a difference
    // is found as follows:
    // 1. Identifiers consisting of only digits are compared numerically.
    // 2. Identifiers with letters or hyphens are compared lexically in ASCII sort order.
    // 3. Numeric identifiers always have lower precedence than non-numeric identifiers.
    // 4. A larger set of pre-release fields has a higher precedence than a smaller set, if all of
    // the preceding identifiers are equal.
    // Reference: https://semver.org/#spec-item-11
    auto parts1 =
        lhs | std::views::split(VERSION_STRING_SEPARATOR) | std::views::transform([](auto&& part) {
            return std::string_view{part.data(), part.size()};
        });

    auto parts2 =
        rhs | std::views::split(VERSION_STRING_SEPARATOR) | std::views::transform([](auto&& part) {
            return std::string_view{part.data(), part.size()};
        });

    auto it1 = std::ranges::begin(parts1);
    auto it2 = std::ranges::begin(parts2);
    for (; it1 != std::ranges::end(parts1) && it2 != std::ranges::end(parts2); ++it1, ++it2) {
        const std::string_view identifier1{*it1};
        const std::string_view identifier2{*it2};

        if (is_numeric_identifier<std::string_view>(identifier1) &&
            is_numeric_identifier<std::string_view>(identifier2)) {
            // As per specification 11.4.1, numeric identifiers are compared numerically.
            const std::uint64_t num1{std::stoull(std::string(identifier1))};
            const std::uint64_t num2{std::stoull(std::string(identifier2))};
            // We can't return directly, because if numbers are equal, we need to continue to the
            // next identifier.
            if (num1 != num2) {
                return num1 < num2;
            }
        } else {
            // We can't just compare with operator < directly because if the identifiers are equal,
            // we need to continue to the next identifier.
            if (identifier1 != identifier2) {
                // Just compare them lexicographically, as per specification 11.4.2.
                return identifier1 < identifier2;
            }
        }
    }

    // If we're here, it means that all the compared identifiers are equal. In this case, the
    // version with more identifiers has higher precedence, as per specification 11.4.4.
    return std::ranges::distance(parts1) < std::ranges::distance(parts2);
}

} // namespace details

/**
 * @brief Less-than comparison operator for versions.
 *
 * @param lhs The left-hand side version.
 * @param rhs The right-hand side version.
 * @return true if lhs is less than rhs, false otherwise.
 */
constexpr bool operator<(const Version auto& lhs, const Version auto& rhs) noexcept {
    if (lhs.major() != rhs.major()) {
        return lhs.major() < rhs.major();
    }
    if (lhs.minor() != rhs.minor()) {
        return lhs.minor() < rhs.minor();
    }
    if (lhs.patch() != rhs.patch()) {
        return lhs.patch() < rhs.patch();
    }
    if (lhs.prerelease_data() && !rhs.prerelease_data()) {
        // A version with a pre-release string has lower precedence than the same version without
        // a pre-release string, as per specification 11.3.
        return true;
    }
    if (!lhs.prerelease_data() && rhs.prerelease_data()) {
        // A version without a pre-release string has higher precedence than the same version with
        // a pre-release string, as per specification 11.3.
        return false;
    }
    if (lhs.prerelease_data() && rhs.prerelease_data()) {
        // If both versions have a pre-release string, we need to compare them according to the
        // specification 11.4.
        return details::is_prelease_strictly_lower_than(lhs.prerelease_data().value(),
                                                        rhs.prerelease_data().value());
    }

    return false;
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
 * @brief Three-way comparison operator for versions. This operator returns a
 * std::strong_ordering value that indicates the relative order of the two versions.
 *
 * @param lhs The left-hand side version.
 * @param rhs The right-hand side version.
 * @return A std::strong_ordering value indicating the relative order of the two versions.
 */
constexpr std::strong_ordering operator<=>(const Version auto& lhs,
                                           const Version auto& rhs) noexcept {
    if (lhs < rhs) {
        return std::strong_ordering::less;
    }
    if (lhs > rhs) {
        return std::strong_ordering::greater;
    }
    return std::strong_ordering::equal;
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
 * @brief Convert a version to a string in the format "major.minor.patch" (or
 * "major.minor.patch-prerelease_data+build_metadata" if pre-release data and build metadata are
 * present). This follows the semantic versioning specification which states that version numbers
 * MUST NOT contain leading zeroes. Pre-release data and build metadata are included in the string
 * if they are present, and formatted according the specification, with a hyphen before the
 * pre-release data and a plus sign before the build metadata, appending pre-release data first and
 * then build metadata, if both are present.
 *
 * Reference: https://semver.org/#spec-item-2, https://semver.org/#spec-item-9,
 * https://semver.org/#spec-item-10
 *
 * @param version The version to convert.
 * @return A string representation of the version.
 */
auto to_string(const Version auto& ver) {
    auto formatStr{std::format("{}{}{}{}{}", ver.major(), VERSION_STRING_SEPARATOR, ver.minor(),
                               VERSION_STRING_SEPARATOR, ver.patch())};
    if (ver.prerelease_data()) {
        formatStr += std::format("-{}", ver.prerelease_data().value());
    }

    if (ver.build_metadata()) {
        formatStr += std::format("+{}", ver.build_metadata().value());
    }

    return formatStr;
}

namespace details {
/**
 * @brief Check if a character is valid for a version string. Valid characters are alphanumeric
 * characters, hyphens, the version string separator ('.') and the plus sign ('+').
 *
 * @param c The character to check.
 * @return true if the character is valid for a version string, false otherwise.
 */
constexpr bool is_valid_char_for_version_string(const char c) noexcept {
    return is_alnum_or_hyphen(c) || c == VERSION_STRING_SEPARATOR || c == '+';
}

/**
 * @brief Check if a version string is valid. A valid version string must not be empty and must
 * consist of valid characters for version strings only (alphanumeric characters, hyphens, the
 * version string separator ('.') and the plus sign ('+')). This is a sanity check to avoid parsing
 * strings that do not conform to the expected format.
 *
 * @param versionStr The version string to check.
 * @return true if the version string is valid, false otherwise.
 */
constexpr bool is_valid_version_string(const std::string_view versionStr) noexcept {
    if (versionStr.empty()) {
        return false; // The version string must not be empty.
    }

    for (const char c : versionStr) {
        if (!is_valid_char_for_version_string(c)) {
            return false; // The version string contains invalid characters.
        }
    }
    return true;
}
} // namespace details

/**
 * @brief Convert a string to a version. The string MUST be in the format
 * "major.minor.patch-<prerelease_data>+<build_metadata>", where major, minor and patch are
 * non-negative integers without leading zeros unless the number is zero and the pre-release data
 * and build metadata are optional and can be any string that follows the specification for
 * pre-release data and build metadata, respectively. The function will perform a sanity check on
 * the input string, and if it does not follow the expected format, it will return std::nullopt.
 *
 * If parsing fails, it returns std::nullopt.
 *
 * @tparam VersionT A type that satisfies the Version concept.
 * @param versionStr The string to convert.
 * @return An optional containing the version if the conversion was successful, std::nullopt
 * otherwise.
 */
template <Version VersionT>
constexpr std::optional<VersionT> from_string(const std::string_view versionStr) {
    if (!details::is_valid_version_string(versionStr)) {
        return std::nullopt;
    }

    // Firstly, we parse the core version string.
    auto coreVersionView = versionStr | std::views::take_while([](const char c) {
                               return c != '-' && c != '+';
                           });

    auto coreVersionParts = coreVersionView | std::views::split(VERSION_STRING_SEPARATOR);
    if (std::ranges::distance(coreVersionParts.begin(), coreVersionParts.end()) != 3) {
        // The core version string must have exactly 3 parts: major, minor and
        // patch.
        return std::nullopt;
    }
    typename VersionT::major_t major{};
    typename VersionT::minor_t minor{};
    typename VersionT::patch_t patch{};
    for (std::uint8_t compIdx{};
         const auto part : coreVersionParts | std::views::transform([](auto&& part) {
                               return std::string_view{part.data(), part.size()};
                           })) {
        if (!details::is_numeric_identifier(part)) {
            // The major, minor and patch version numbers must be numeric
            return std::nullopt;
        }
        if (compIdx == 0) {
            major = static_cast<typename VersionT::major_t>(std::stoull(std::string{part}));
        } else if (compIdx == 1) {
            minor = static_cast<typename VersionT::minor_t>(std::stoull(std::string{part}));
        } else if (compIdx == 2) {
            patch = static_cast<typename VersionT::patch_t>(std::stoull(std::string{part}));
        }
        compIdx++;
    }

    // Now we parse the pre-release data and build metadata, if they are present.
    std::optional<typename VersionT::prerelease_string_t> prereleaseData{};
    std::optional<typename VersionT::build_metadata_string_t> buildMetadata{};
    auto prereleaseAndBuildMetadataView = versionStr | std::views::drop_while([](const char c) {
                                              return c != '-' && c != '+';
                                          });
    if (prereleaseAndBuildMetadataView.empty()) {
        // If there is no pre-release data and build metadata, we can return the version now.
        return VersionT{major, minor, patch};
    }

    std::size_t dropCount{};
    if (const auto selector = prereleaseAndBuildMetadataView[0]; selector == '-') {
        // Prerelease data is present.
        // Take while it encounters a '+' character, which indicates the start of the build
        // metadata.
        auto prereleaseDataView = prereleaseAndBuildMetadataView | std::views::drop(1) |
                                  std::views::take_while([](const char c) {
                                      return c != '+';
                                  });
        std::string prereleaseDataStr{};
        std::ranges::copy(prereleaseDataView, std::back_inserter(prereleaseDataStr));
        if (prereleaseDataStr.empty() ||
            !details::check_string_identifiers(prereleaseDataStr,
                                               /* can include leading zeros */ false)) {
            // If there is a '-' character, there must be pre-release data after it.
            return std::nullopt;
        }
        prereleaseData = std::move(prereleaseDataStr);
        // The "1" is for the '-' character that we need to drop, and the rest is for the
        // pre-release data that we just dropped.
        dropCount = 1 + std::ranges::distance(prereleaseDataView.begin(), prereleaseDataView.end());
    }

    // Build metadata.
    // We need to drop the pre-release data (which contains the '-' in the dropCount) and the build
    // metadata selector (which is the '+' character).
    auto buildMetadataSelectorView = prereleaseAndBuildMetadataView | std::views::drop(dropCount);
    if (buildMetadataSelectorView.empty()) {
        // In this case there is no more data to parse, so we can return the version now.
        return VersionT{major, minor, patch, std::move(prereleaseData)};
    }

    // Remove the selector.
    auto buildMetadataView = buildMetadataSelectorView | std::views::drop(1);
    std::string buildMetadataStr{};
    std::ranges::copy(buildMetadataView, std::back_inserter(buildMetadataStr));
    if (buildMetadataStr.empty() ||
        !details::check_string_identifiers(buildMetadataStr,
                                           /* can include leading zeros */ true)) {
        return std::nullopt;
    }
    buildMetadata = std::move(buildMetadataStr);

    return VersionT{major, minor, patch, std::move(prereleaseData), std::move(buildMetadata)};
}

static_assert(from_string<version>("") == std::nullopt);
static_assert(from_string<version>("1") == std::nullopt);
static_assert(from_string<version>("1.0") == std::nullopt);
static_assert(from_string<version>("1.0.0.0") == std::nullopt);
static_assert(from_string<version>("....") == std::nullopt);
static_assert(from_string<version>("..") == std::nullopt);
static_assert(from_string<version>("junk-data") == std::nullopt);
static_assert(from_string<version>("nan.nan.nan") == std::nullopt);
} // namespace verso

#endif // INCLUDE_VERSO_HPP
