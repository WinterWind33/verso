// Copyright (c) 2025 Andrea Ballestrazzi
#ifndef INCLUDE_VERSO_HPP
#define INCLUDE_VERSO_HPP

// C++ STL
#include <algorithm>
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
 * @brief Concept for version traits. Version traits MUST define the types used for the major, minor
 * and patch version numbers, and these types MUST satisfy the NormalVersionNumberComponent concept.
 *
 * @tparam TraitsT The traits type to check.
 */
template <typename TraitsT>
concept VersionTraits = requires {
    typename TraitsT::major_t;
    typename TraitsT::minor_t;
    typename TraitsT::patch_t;

    requires NormalVersionNumberComponent<typename TraitsT::major_t>;
    requires NormalVersionNumberComponent<typename TraitsT::minor_t>;
    requires NormalVersionNumberComponent<typename TraitsT::patch_t>;
};

/**
 * @brief General traits for a version. It contains the types used for the major, minor and patch
 *  and other version numbers.
 *
 * @tparam MajorT The type used for the major version number.
 * @tparam MinorT The type used for the minor version number.
 * @tparam PatchT The type used for the patch version number.
 */
template <NormalVersionNumberComponent MajorT, NormalVersionNumberComponent MinorT,
          NormalVersionNumberComponent PatchT>
struct version_traits {
    using major_t = std::decay_t<MajorT>;
    using minor_t = std::decay_t<MinorT>;
    using patch_t = std::decay_t<PatchT>;
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
class basic_version final {
public:
    using traits_t = std::decay_t<TraitsT>;
    using major_t = typename traits_t::major_t;
    using minor_t = typename traits_t::minor_t;
    using patch_t = typename traits_t::patch_t;

    /**
     * @brief Default constructor, initializes the version to 0.0.0.
     */
    constexpr basic_version() noexcept = default;

    /**
     * @brief Constructor with major, minor and patch version numbers.
     */
    constexpr basic_version(major_t major, minor_t minor, patch_t patch) noexcept
        : m_major{major},
          m_minor{minor},
          m_patch{patch} {}

    // ### Getters ###

    /**
     * @brief Get the major version number.
     *
     * @return The major version number.
     */
    [[nodiscard]] constexpr major_t major() const noexcept {
        return m_major;
    }

    /**
     * @brief Get the minor version number.
     *
     * @return The minor version number.
     */
    [[nodiscard]] constexpr minor_t minor() const noexcept {
        return m_minor;
    }

    /**
     * @brief Get the patch version number.
     *
     * @return The patch version number.
     */
    [[nodiscard]] constexpr patch_t patch() const noexcept {
        return m_patch;
    }

    // ### Setters ###

    /**
     * @brief Set the major version number.
     *
     * @param major The new major version number.
     */
    constexpr void major(const major_t major) noexcept {
        m_major = major;
    }

    /**
     * @brief Set the minor version number.
     *
     * @param minor The new minor version number.
     */
    constexpr void minor(const minor_t minor) noexcept {
        m_minor = minor;
    }

    /**
     * @brief Set the patch version number.
     *
     * @param patch The new patch version number.
     */
    constexpr void patch(const patch_t patch) noexcept {
        m_patch = patch;
    }

    // ### Comparison operators ###

    [[nodiscard]] constexpr bool operator==(const basic_version& other) const noexcept = default;

private:
    major_t m_major{};
    minor_t m_minor{};
    patch_t m_patch{};
};

/**
 * @brief Concept for a version. A version MUST be a basic_version instantiated with
 *  a type that satisfies the VersionTraits concept.
 *
 * @tparam VersionT The type to check.
 */
template <typename VersionT>
concept Version = requires {
    // Must have a nested traits_t type that satisfies the VersionTraits concept.
    typename std::decay_t<VersionT>::traits_t;
    requires VersionTraits<typename std::decay_t<VersionT>::traits_t>;
} && std::same_as<VersionT, basic_version<typename std::decay_t<VersionT>::traits_t>>;

/**
 * @brief Alias for version which has all the version numbers with the same types.
 *
 * @tparam NormalVersionNumberComponentT The type of the version numbers.
 */
template <NormalVersionNumberComponent NormalVersionNumberComponentT>
using uniform_version = basic_version<default_version_traits<NormalVersionNumberComponentT>>;

/**
 * @brief Default specialization of basic_version with the default normal
 *  version number (std::uint32_t)
 */
using version = uniform_version<default_normal_version_number_component>;

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
 * @brief Get the components of a version as a tuple (major, minor, patch).
 *  This can be useful for structured bindings.
 *
 * @tparam VersionT A type that satisfies the Version concept.
 * @param version The version instance.
 * @return A tuple containing the major, minor and patch version numbers.
 */
constexpr auto components(const Version auto& version) {
    return std::make_tuple(version.major(), version.minor(), version.patch());
}

// ### This library version ###

/**
 * @brief The current version of the library, using the default version type.
 *  To honor this library, the library version will always be represented only by a "version"
 *  object, single components (major, minor, patch) will not be exposed directly.
 */
constexpr version verso_version{0, 1, 0};

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
    if (lhs.major() != rhs.major()) {
        return lhs.major() < rhs.major();
    }
    if (lhs.minor() != rhs.minor()) {
        return lhs.minor() < rhs.minor();
    }
    return lhs.patch() < rhs.patch();
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
    return std::format("{}.{}.{}", version.major(), version.minor(), version.patch());
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
    std::string_view::const_iterator it{std::find(startIt, std::cend(versionStr), '.')};
    for (; it != std::cend(versionStr); it = std::find(startIt, std::cend(versionStr), '.')) {
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
