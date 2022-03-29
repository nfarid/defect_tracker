
#ifndef HPP_UTIL_CSTRING_VIEW_
#define HPP_UTIL_CSTRING_VIEW_

#include "core.hpp"
#include "size.hpp"

#include <cstring>
#include <functional>
#include <iosfwd>
#include <string_view>
#include <string>
#include <type_traits>


namespace Util
{


/**
 * \brief A view to a contiguous sequences of char that ends with `\0`
 * \note based on the std::string_view interface from https://en.cppreference.com/w/cpp/string/basic_string_view
 */
typedef class CStringView CStringView;
class CStringView {
    using string_view = std::string_view;

public:
    // Typedefs

    using traits_type = string_view::traits_type;
    using value_type = string_view::value_type;
    using pointer = string_view::pointer;
    using const_pointer = string_view::const_pointer;
    using reference = string_view::reference;
    using const_reference = string_view::const_reference;
    using const_iterator = string_view::const_iterator;
    using iterator = string_view::iterator;
    using const_reverse_iterator = string_view::const_reverse_iterator;
    using reverse_iterator = string_view::reverse_iterator;
    using size_type = string_view::size_type;
    using difference_type = string_view::difference_type;

    // Static constants

    constexpr static inline size_type npos = string_view::npos;

    // Constructors

    /**
     * \brief Construct a string view that with data() at nullptr and size() at zero
     */
    UTIL_IMPLICIT constexpr CStringView() noexcept = default;

    /**
     * \brief Construct a string view from a null terminated C-array
     */
    template<size_type N>
    UTIL_IMPLICIT constexpr CStringView(const value_type(& arr)[N]) :
        m_sv{arr, N-1}
    {
        assert(arr[N-1] == '\0' && "must be null terminated");
    }

    /**
     * \brief Construct a string view from std::string
     */
    template<typename StrType, typename S_ = std::enable_if_t<std::is_same_v<StrType, std::string> > >
    UTIL_IMPLICIT constexpr CStringView(const StrType& str) :
        m_sv{str}
    {}

    /**
     * \brief Construct a string view from a null terminated c-string and it's strlen
     * \param arr - a pointer to a null terminated c-string
     * \param len - the length of the string (NOT including the null-char)
     * \note arr[len] == '\0'
     */
    explicit constexpr CStringView(const value_type* arr, const size_type len) :
        m_sv{arr, len}
    {
        assert(arr[len] == '\0' && "must be null terminated");
    }

    /**
     * \brief Construct a string view from a null terminated c-string
     * \param arr - a pointer to a null terminated c-string
     * \note arr[strlen(arr)] == '\0'
     */
    template<typename CPtr, typename C_ = std::enable_if_t<std::is_convertible_v<CPtr, const_pointer> > >
    explicit constexpr CStringView(CPtr arr) :
        m_sv{arr}
    {
        assert(arr[std::size(m_sv)] == '\0' && "must be null terminated");
    }

    // Iterators


    /**
     * \brief Gets an iterator to the beginning
     */
    constexpr const_iterator begin() const noexcept {
        return m_sv.begin();
    }

    /**
     * \brief Gets an iterator to the beginning
     */
    constexpr const_iterator cbegin() const noexcept {
        return m_sv.cbegin();
    }

    /**
     * \brief Gets a reverse iterator to the beginning
     */
    constexpr const_reverse_iterator rbegin() const noexcept {
        return m_sv.rbegin();
    }

    /**
     * \brief Gets a reverse iterator to the beginning
     */
    constexpr const_reverse_iterator crbegin() const noexcept {
        return m_sv.crbegin();
    }

    /**
     * \brief Gets an iterator to one past the end
     */
    constexpr const_iterator end() const noexcept {
        return m_sv.end();
    }

    /**
     * \brief Gets an iterator to one past the end
     */
    constexpr const_iterator cend() const noexcept {
        return m_sv.cend();
    }

    /**
     * \brief Gets a reverse iterator to the end
     */
    constexpr const_reverse_iterator rend() const noexcept {
        return m_sv.rend();
    }

    /**
     * \brief Gets a reverse iterator to the end
     */
    constexpr const_reverse_iterator crend() const noexcept {
        return m_sv.crend();
    }

    // Element access

    /**
     * \brief Access a code-unit at the specified position
     * \param pos - must be less than size()
     */
    constexpr const_reference operator[](size_type pos) const {
        return m_sv[pos];
    }

    /**
     * \brief Access a code-unit at the specified position
     * \param pos - must be less than size()
     * \throws std::out_of_range if pos >= size()
     */
    constexpr const_reference at(size_type pos) const {
        return m_sv.at(pos);
    }

    /**
     * \brief Access the first code-unit
     * \note Undefined behaviour if empty() is true
     */
    constexpr const_reference front() const {
        return m_sv.front();
    }

    /**
     * \brief Access the last code-unit
     * \note Undefined behaviour if empty() is true
     */
    constexpr const_reference back() const {
        return m_sv.back();
    }

    /**
     * \brief Gets a pointer to the underlying array
     */
    constexpr const_pointer data() const noexcept {
        return m_sv.data();
    }

    /**
     * \brief Gets a pointer to the underlying null-terminated char-array
     */
    constexpr const_pointer c_str() const noexcept {
        return data();
    }

    // Capacity

    /**
     * \brief Gets the number of code-units of the string view
     */
    constexpr size_type size() const noexcept {
        return m_sv.size();
    }

    /**
     * \brief Gets the number of code-units of the string view
     */
    constexpr size_type length() const noexcept {
        return m_sv.length();
    }

    /**
     * \brief Gets the the maximum number of code-units
     */
    constexpr size_type max_size() const noexcept {
        return m_sv.max_size();
    }

    /**
     * \brief Checks if this string view is empty
     */
    [[nodiscard]] constexpr bool empty() const noexcept {
        return m_sv.empty();
    }

    /**
     * \brief Gets the number of code-units of the string view
     */
    constexpr CStringView::difference_type ssize() const noexcept {
        return static_cast<CStringView::difference_type>(size() );
    }

    /**
     * \brief Gets the number of code-units of the string view
     */
    constexpr CStringView::difference_type slength() const noexcept {
        return static_cast<CStringView::difference_type>(length() );
    }

    /**
     * \brief Gets the the maximum number of code-units
     */
    constexpr CStringView::difference_type smax_size() const noexcept {
        return static_cast<CStringView::difference_type>(max_size() );
    }

    // Modifiers

    /**
     * \brief Shrinks the view by moving its start forward
     * \param n - number of characters to remove from the start of the view (needs to be less than size() )
     * \note does not return anything, this is a mutator
     */
    constexpr void remove_prefix(size_type n) {
        return m_sv.remove_prefix(n);
    }

    /**
     * \brief Exchange views
     */
    constexpr void swap(CStringView& rhs) noexcept {
        m_sv.swap(rhs.m_sv);
    }

    // Miscellaneous functions

    /**
     * \brief Copies this substring into the specified char-array
     * \param dest - pointer to the destination char-string
     * \param count - requested substring length
     * \param pos - starting position of this string view
     * \returns the number of code-units copied which is min(count, size() - pos)
     * \throws std::out_of_range if pos > size()
     */
    size_type copy(pointer dest, size_type count, size_type pos = 0) const {
        return m_sv.copy(dest, count, pos);
    }

    /**
     * \brief Gets a substring view
     * \param pos - starting position of this string view
     * \param count - requested length
     * \returns a view of the substring at range [pos, pos + min(count, size() - pos) )
     * \throws std::out_of_range if pos > size()
     */
    constexpr string_view substr(size_type pos = 0, size_type count = npos) const {
        return m_sv.substr(pos, count);
    }

    /**
     * \brief Checks if it's the first code-unit
     */
    constexpr bool starts_with(value_type c) const noexcept {
        return !empty() && c == front();
    }

    /**
     * \brief Lexicographically compare against another string-view
     * \return -1 if less than, 1 if greater than, 0 if equals
     */
    constexpr int compare(string_view sv) const noexcept {
        return m_sv.compare(sv);
    }

    /**
     * \brief Lexicographically compare substr(pos1,count1) against another string-view sv
     * \return -1 if less than, 1 if greater than, 0 if equals
     */
    constexpr int compare(size_type pos1, size_type count1, string_view sv) const {
        return m_sv.compare(pos1, count1, sv);
    }

    /**
     * \brief Lexicographically compare substr(pos1,count1) against another string-view sv.substr(pos2,count2)
     * \return -1 if less than, 1 if greater than, 0 if equals
     */
    constexpr int compare( size_type pos1, size_type count1, string_view sv,
            size_type pos2, size_type count2) const
    {
        return m_sv.compare(pos1, count1, sv, pos2, count2);
    }

    /**
     * \brief Lexicographically compare against a null-terminated c-string
     * \return -1 if less than, 1 if greater than, 0 if equals
     */
    constexpr int compare(const_pointer s) const {
        return m_sv.compare(s);
    }

    /**
     * \brief Lexicographically compare substr(pos1,count1) against a null-terminated c-string
     * \return -1 if less than, 1 if greater than, 0 if equals
     */
    constexpr int compare(size_type pos1, size_type count1, const_pointer s) const {
        return m_sv.compare(pos1, count1, s);
    }

    /**
     * \brief Lexicographically compare substr(pos1,count1) against a char-array s of length count2
     * \return -1 if less than, 1 if greater than, 0 if equals
     */
    constexpr int compare(size_type pos1, size_type count1, const_pointer s,
            size_type count2) const
    {
        return m_sv.compare(pos1, count1, s, count2);
    }

    // Find functions
    /**
     * \brief Finds the first substring equal to the specified string view
     * \param sv - the string to search for
     * \param pos - position at which to start the search
     * \return the position of the beginning of the found substring or npos if no such substring is found
     */
    constexpr size_type find(string_view sv, size_type pos = 0) const noexcept {
        return m_sv.find(sv, pos);
    }

    /**
     * \brief Finds the first substring equal to the specified code-unit
     * \param ch - the code-unit to search for
     * \param pos - position at which to start the search
     * \return the position of the beginning the found substring or npos if no such substring is found
     */
    constexpr size_type find(value_type ch, size_type pos = 0) const noexcept {
        return m_sv.find(ch, pos);
    }

    /**
     * \brief Finds the first substring equal to the specified char-array
     * \param s - a pointer to the beginning of the char-array
     * \param pos - position at which to start the search
     * \param count - the size of the char-array
     * \return the position of the beginning found substring or npos if no such substring is found
     */
    constexpr size_type find(const_pointer s, size_type pos, size_type count) const {
        return m_sv.find(s, pos, count);
    }

    /**
     * \brief Finds the first substring equal to the specified null-terminated c-string
     * \param s - a pointer to the beginning of c-string (must be null-terminated)
     * \param pos - position at which to start the search
     * \return the position of the beginning the found substring or npos if no such substring is found
     */
    constexpr size_type find(const_pointer s, size_type pos = 0) const {
        return m_sv.find(s, pos);
    }

    /**
     * \brief Finds the last substring equal to the specified string-view
     * \param v - the string to search for
     * \param pos - position at which to start the search
     * \return the position of the beginning of the found substring or npos if no such substring is found
     */
    constexpr size_type rfind(string_view v, size_type pos = npos) const noexcept {
        return m_sv.find(v, pos);
    }

    /**
     * \brief Finds the last substring equal to the specified code-unit
     * \param ch - the code-unit to search for
     * \param pos - position at which to start the search
     * \return the position of the beginning of the found substring or npos if no such substring is found
     */
    constexpr size_type rfind(value_type c, size_type pos = npos) const noexcept {
        return m_sv.rfind(c, pos);
    }

    /**
     * \brief Finds the last substring equal to the specified char-array
     * \param s - a pointer to the beginning of the char-array
     * \param pos - position at which to start the search
     * \param count - the size of the char-array
     * \return the position of the beginning of the found substring or npos if no such substring is found
     */
    constexpr size_type rfind(const_pointer s, size_type pos, size_type count) const {
        return m_sv.rfind(s, pos, count);
    }

    /**
     * \brief Finds the last substring equal to the specified null-terminated c-string
     * \param s - a pointer to the beginning of c-string (must be null-terminated)
     * \param pos - position at which to start the search
     * \return the position of the first code-unit of the found substring or npos if no such substring is found
     */
    constexpr size_type rfind(const_pointer s, size_type pos = npos) const {
        return m_sv.rfind(s, pos);
    }

    /**
     * \brief Finds the first code-unit equal to any of the code-units in the specified string-view
     * \param v - view to search for
     * \param pos - position at which to start the search
     * \return the position of the first occurrence of any code-unit of the string-view, or npos if no such code-unit is found
     */
    constexpr size_type find_first_of(string_view v, size_type pos = 0) const noexcept {
        return m_sv.find_first_of(v, pos);
    }

    /**
     * \brief Finds the first code-unit equal to the specified code-unit
     * \param ch - the code-unit to search for
     * \param pos - position at which to start the search
     * \return the position of the first occurrence of the code-unit, or npos if no such code-unit is found
     */
    constexpr size_type find_first_of(value_type c, size_type pos = 0) const noexcept {
        return m_sv.find_first_of(c, pos);
    }

    /**
     * \brief Finds the first code-unit equal to any of the code-units in the specified char-array
     * \param s - a pointer to the beginning of the char-array
     * \param pos - position at which to start the search
     * \param count - the size of the char-array
     * \return the position of the first occurrence of the code-unit, or npos if no such code-unit is found
     */
    constexpr size_type find_first_of(const_pointer s, size_type pos, size_type count) const {
        return m_sv.find_first_of(s, pos, count);
    }

    /**
     * \brief Finds the first code-unit equal to any of the code-units in the specified null-terminated c-string
     * \param s - a pointer to the beginning of c-string (must be null-terminated)
     * \param pos - position at which to start the search
     * \return the position of the first occurrence of the code-unit, or npos if no such code-unit is found
     */
    constexpr size_type find_first_of(const_pointer s, size_type pos = 0) const {
        return m_sv.find_first_of(s, pos);
    }

    /**
     * \brief Finds the last code-unit equal to any of the code-units in the specified string-view
     * \param v - view to search for
     * \param pos - position at which to start the search
     * \return the position of the last occurrence of any code-units of the string-view, or npos if no such code-unit is found
     */
    constexpr size_type find_last_of(string_view v, size_type pos = npos) const noexcept {
        return m_sv.find_last_of(v, pos);
    }

    /**
     * \brief Finds the last code-unit equal to the specified code-unit
     * \param ch - the code-unit to search for
     * \param pos - position at which to start the search
     * \return the position of the last occurrence of the code-unit, or npos if no such code-unit is found
     */
    constexpr size_type find_last_of(value_type c, size_type pos = npos) const noexcept {
        return m_sv.find_last_of(c, pos);
    }

    /**
     * \brief Finds the last code-unit equal to any of the code-units in the specified char-array
     * \param s - a pointer to the beginning of the char-array
     * \param pos - position at which to start the search
     * \param count - the size of the char-array
     * \return the position of the last occurrence of the code-unit, or npos if no such code-unit is found
     */
    constexpr size_type find_last_of(const_pointer s, size_type pos, size_type count) const {
        return m_sv.find_last_of(s, pos, count);
    }

    /**
     * \brief Finds the last code-unit equal to any of the code-unit in the specified null-terminated c-string
     * \param s - a pointer to the beginning of c-string (must be null-terminated)
     * \param pos - position at which to start the search
     * \return the position of the last occurrence of the code-unit, or npos if no such code-unit is found
     */
    constexpr size_type find_last_of(const_pointer s, size_type pos = npos) const {
        return m_sv.find_last_of(s, pos);
    }

    /**
     * \brief Finds the first code-unit not equal to any of the code-unit in the specified string-view
     * \param v - view to search against
     * \param pos - position at which to start the search
     * \return the position of the first code-unit that isn't in the of the specified string view, or npos if no such code-unit is found
     */
    constexpr size_type find_first_not_of(string_view v, size_type pos = 0) const noexcept {
        return m_sv.find_first_not_of(v, pos);
    }

    /**
     * \brief Finds the first code-unit not equal to the specified code-unit
     * \param ch - the code-unit to search against
     * \param pos - position at which to start the search
     * \return the position of the first code-unit that isn't the specified one, or npos if no such code-unit is found
     */
    constexpr size_type find_first_not_of(value_type c, size_type pos = 0) const noexcept {
        return m_sv.find_first_not_of(c, pos);
    }

    /**
     * \brief Finds the first code-unit not equal to any of the code-unit in the specified char-array
     * \param s - a pointer to the beginning of the char-array
     * \param pos - position at which to start the search
     * \param count - the size of the char-array
     * \return the position of the first code-unit that isn't in the of the specified char-array, or npos if no such code-unit is found
     */
    constexpr size_type find_first_not_of(const_pointer s, size_type pos, size_type count) const {
        return m_sv.find_first_not_of(s, pos, count);
    }

    /**
     * \brief Finds the first code-unit not equal to any of the code-unit in the specified null-terminated c-string
     * \param s - a pointer to the beginning of c-string (must be null-terminated)
     * \param pos - position at which to start the search
     * \return the position of the first code-unit that isn't in the of the specified c-string, or npos if no such code-unit is found
     */
    constexpr size_type find_first_not_of(const_pointer s, size_type pos = 0) const {
        return m_sv.find_first_not_of(s, pos);
    }

    /**
     * \brief Finds the last code-unit not equal to any of the code-unit in the specified string-view
     * \param v - string-view to search against
     * \param pos - position at which to start the search
     * \return the position of the last code-unit that isn't in the the specified string-view, or npos if no such code-unit is found
     */
    constexpr size_type find_last_not_of(string_view v, size_type pos = npos) const noexcept {
        return m_sv.find_last_not_of(v, pos);
    }

    /**
     * \brief Finds the last code-unit not equal to the specified code-unit
     * \param ch - the code-unit to search against
     * \param pos - position at which to start the search
     * \return the position of the last code-unit that isn't the specified one, or npos if no such code-unit is found
     */
    constexpr size_type find_last_not_of(value_type c, size_type pos = npos) const noexcept {
        return m_sv.find_last_not_of(c, pos);
    }

    /**
     * \brief Finds the last code-unit not equal to any of the code-unit in the specified char-array
     * \param s - a pointer to the beginning of the char-array
     * \param pos - position at which to start the search
     * \param count - the size of the char-array
     * \return the position of the last code-unit that isn't in the of the specified char-array, or npos if no such code-unit is found
     */
    constexpr size_type find_last_not_of(const_pointer s, size_type pos, size_type count) const {
        return m_sv.find_last_not_of(s, pos, count);
    }

    /**
     * \brief Finds the last code-unit not equal to any of the code-unit in the specified null terminated c-string
     * \param s - a pointer to the beginning of c-string (must be null terminated)
     * \param pos - position at which to start the search
     * \return the position of the last code-unit that isn't in the of the specified c-string, or npos if no such code-unit is found
     */
    constexpr size_type find_last_not_of(const_pointer s, size_type pos = npos) const {
        return m_sv.find_last_not_of(s, pos);
    }

    // Operators

    /**
     * \brief Implicitly converts a CStringView to a string_view
     */
    UTIL_IMPLICIT constexpr operator string_view() const noexcept {
        return m_sv;
    }

    /**
     * \brief Checks if 2 views are equal
     */
    friend constexpr bool operator==(CStringView lhs, CStringView rhs) noexcept {
        return lhs.m_sv == rhs.m_sv;
    }

    /**
     * \brief Checks if 2 views are not equal
     */
    friend constexpr bool operator!=(CStringView lhs, CStringView rhs) noexcept {
        return lhs.m_sv != rhs.m_sv;
    }

    /**
     * \brief Checks if lhs is less than rhs lexicographically
     */
    friend constexpr bool operator<(CStringView lhs, CStringView rhs) noexcept {
        return lhs.m_sv < rhs.m_sv;
    }

    /**
     * \brief Checks if lhs is less than or equal to rhs lexicographically
     */
    friend constexpr bool operator<=(CStringView lhs, CStringView rhs) noexcept {
        return lhs.m_sv <= rhs.m_sv;
    }

    /**
     * \brief Checks if lhs is greater than rhs lexicographically
     */
    friend constexpr bool operator>(CStringView lhs, CStringView rhs) noexcept {
        return lhs.m_sv > rhs.m_sv;
    }

    /**
     * \brief Checks if lhs is greater than or equal to rhs lexicographically
     */
    friend constexpr bool operator>=(CStringView lhs, CStringView rhs) noexcept {
        return lhs.m_sv >= rhs.m_sv;
    }

    /**
     * \brief Performs stream operation on this string view
     * \return os - the ouput stream
     */
    friend std::ostream& operator<<(std::ostream& os, CStringView cs) {
        return os<<cs.m_sv;
    }

private:
    string_view m_sv;
};

inline std::string to_string(CStringView cs) {
    return std::string(cs);
}

inline namespace Udl
{


/**
 * \brief Creates a string view from a string literal
 * \param str - pointer to the beginning of the raw char-array literal
 * \param len - length of the c-string (not including the null char)
 *
 * \example const auto my_string = "foo bar"_cs;
 */
UTIL_PURE constexpr
CStringView operator""_cs(CStringView::const_pointer str, CStringView::size_type len) {
    return CStringView{str, len};
}


}  // namespace Udl


}// namespace Util



namespace std
{


/**
 *\brief A specialisation of std::hash functor for CStringView
 *\note custom specialisations of std::hash can be injected in namespace std
 */
template<>
struct hash<Util::CStringView> {
    /**
     * \brief calculates the hash of the CStringView
     */
    std::size_t operator()(Util::CStringView csv) const noexcept {
        return std::hash<std::string_view>{}(csv);
    }
};


}  // namespace std

#endif  // HPP_UTIL_CSTRING_VIEW_
