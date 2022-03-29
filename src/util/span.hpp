
#ifndef HPP_UTIL_SPAN_
#define HPP_UTIL_SPAN_

#include "core.hpp"
#include "size.hpp"

#include <array>
#include <cassert>
#include <iterator>
#include <stdexcept>
#include <string>
#include <type_traits>
#include <utility>
#include <vector>


namespace Util
{


/**
 *\brief A view of a contiguous sequence in memory
 *
 *As std::span is only available for C++20, this will have to do
 *
 *\note T should be const for an immutable view
 *
 *\example:
 *int arr[] = {2,3,5,7};
 *Span<int> s1 = arr; //a mutable view of arr - akin to int*
 *const Span<int> s2 = arr; //also a mutable view of arr - akin to int* const
 *Span<const int> s3 = arr; //an immutable view of arr - akin to const int*
 *
 */
template<typename T>
class Span final {
public:
    using element_type = T;
    using value_type = std::remove_cv_t<element_type>;
    using size_type = size_t;
    using difference_type = ptrdiff_t;
    using pointer = T*;
    using const_pointer = const T*;
    using reference = T&;
    using const_reference = const T&;
    using iterator = pointer;
    using reverse_iterator = std::reverse_iterator<iterator>;

    template<size_t N>
    UTIL_IMPLICIT Span( T(& arr )[N] ) :
        m_begin_iter{arr },
        m_end_iter{ arr + N}
    {}

    template<typename U, size_t N, typename U_ = std::enable_if_t<std::is_convertible_v<U*, T*> > >
    UTIL_IMPLICIT Span( std::array<T, N>& arr ) :
        m_begin_iter{arr.data()},
        m_end_iter{arr.data() + N}
    {}

    template<typename U, size_t N, typename U_ = std::enable_if_t<std::is_convertible_v<U*, T*> > >
    UTIL_IMPLICIT Span(const std::array<U, N>& arr ) :
        m_begin_iter{arr.data()},
        m_end_iter{arr.data() + N}
    {}

    template<typename U, typename U_ = std::enable_if_t<std::is_convertible_v<U*, T*> > >
    UTIL_IMPLICIT Span(const std::vector<U>& arr) :
        m_begin_iter{arr.data()},
        m_end_iter{arr.data() + std::size(arr)}
    {}

    explicit Span(pointer ptr, size_type n ) :
        m_begin_iter{ptr},
        m_end_iter{ptr + n}
    {}

    explicit Span(iterator begin_iter_, iterator end_iter_) :
        m_begin_iter{begin_iter_},
        m_end_iter{end_iter_}
    {
        assert(end_iter_ >= begin_iter_);
    }

    pointer data() const {
        return m_begin_iter;
    }

    iterator begin() const {
        return m_begin_iter;
    }

    iterator end() const {
        return m_end_iter;
    }

    reverse_iterator rbegin() const {
        return std::reverse_iterator{end()};
    }

    reverse_iterator rend() const {
        return std::reverse_iterator{begin()};
    }

    // Span should not be empty
    reference front() const {
        assert(!empty() );
        return *m_begin_iter;
    }

    // Span should not be empty
    reference back() const {
        assert(!empty() );
        return *( m_end_iter - 1 );
    }

    // i should not be out of range
    reference operator[]( size_type i ) const {
        assert(i < size() );
        return m_begin_iter[i];
    }

    // throws out_of_range if i is out of range
    reference at( size_type i ) const {
        if(i >= size() ) {
            throw std::out_of_range("Span of size " + std::to_string(size() ) +
                          " is index at position " + std::to_string(i) );
        }
        return m_begin_iter[i];
    }

    size_type size() const noexcept {
        return static_cast<size_type>(ssize() );
    }

    difference_type ssize() const noexcept {
        return m_end_iter - m_begin_iter;
    }

    size_type size_bytes() const noexcept {
        return size() * sizeof(T);
    }

    difference_type ssize_bytes() const noexcept {
        return size_bytes();
    }

    bool empty() const noexcept {
        return m_begin_iter == m_end_iter;
    }

    // Get the first n elements of a span
    // n should not be greater than the size
    Span first(size_type n) const {
        assert(n <= size() );
        return Span{m_begin_iter, m_begin_iter + n};
    }

    // Get the last n elements of a span
    // n should not be greater than the size
    Span last(size_type n) const {
        assert(n <= size() );
        return Span{m_end_iter - n, m_end_iter};
    }

    // Obtains a partial view of the span starting at the offset and a size of n
    // The end of the subspan should not go pass the end of the full span
    Span subspan(size_type offset, size_type n) const {
        assert(offset + n <= size() );
        return Span{m_begin_iter + offset, m_begin_iter + offset + n};
    }

private:
    iterator m_begin_iter = nullptr;
    iterator m_end_iter = nullptr;
};

using std::byte;

/**
 *\brief Obtain a byte view of the span
 */
template<typename T>
Span<const byte> as_bytes( Span<T> sp ) {
    return Span{ reinterpret_cast<const byte*>( sp.data() ), sp.size_bytes()};
}

/**
 *\brief Obtain a byte view of contiguous sequence
 */
template<typename T>
Span<const byte> as_bytes(const T* ptr, size_t n) {
    return Span{ reinterpret_cast<const byte*>(ptr), n};
}

/**
 *\brief Obtain a mutable byte view of the span
 */
template<typename T>
Span<byte> as_writable_bytes( Span<T> sp ) {
    return Span{ reinterpret_cast<byte*>( sp.data() ), sp.size_bytes()};
}

/**
 *\brief Obtain a mutable byte view of contiguous sequence
 */
template<typename T>
Span<byte> as_writable_bytes(T* ptr, size_t n) {
    return Span{ reinterpret_cast<byte*>(ptr), n};
}


}    // namespace Util

#endif    // HPP_UTIL_SPAN_
