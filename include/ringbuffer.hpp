#pragma once

#include <cstdlib>
#include <memory>
#include <limits>
#include <sstream>
#include <iostream>
#include <iomanip>

/**
 * @brief Class, that describes classic ringbuffer
 * data structure.
 * @tparam T Value type.
 * @tparam Size Ringbuffer size.
 */
template< typename T, std::size_t Size>
class ringbuffer
{

    static_assert(Size > 0, "Empty ringbuffer is not allowed.");

public:

    using value_type = T;

    using reference = T&;

    using const_reference = const T&;

    using size_type = std::size_t;

    /**
     * @brief Iterator type.
     */
    class iterator : public std::iterator <
        std::input_iterator_tag, // Input iterator
        value_type,              // Value type
        std::ptrdiff_t,          // Difference
        value_type*,             // Pointer type
        reference                // Reference type
    >
    {
        template<typename, std::size_t>
        friend class ringbuffer;

    public:
        explicit iterator(T* val,
                          size_type begin,
                          size_type fromBegin) :
            m_buffer(val),
            m_currentPos(begin),
            m_traverseCount(fromBegin),
            m_reverse(false)
        {

        }

        iterator& operator++()
        {
            m_currentPos = (m_currentPos + 1) % Size;

            if (m_traverseCount == 0)
            {
                m_reverse = false;
                m_traverseCount++;
            }
            else
            {
                if (m_reverse)
                {
                    m_traverseCount--;
                }
                else
                {
                    m_traverseCount++;
                }
            }

            return *this;
        }

        iterator operator+(size_type val)
        {
            iterator copy = *this;

            // Changing traverse count
            if (!copy.m_reverse)
            {
                copy.m_traverseCount += val;
            }
            else
            {
                if (copy.m_traverseCount > val)
                {
                    copy.m_traverseCount -= val;
                }
                else
                {
                    copy.m_traverseCount = val - copy.m_traverseCount;
                    copy.m_reverse = false;
                }
            }


            copy.m_currentPos = (copy.m_currentPos + val) % Size;

            return copy;
        }

        iterator operator-(size_type val)
        {
            iterator copy = *this;

            if (copy.m_reverse)
            {
                copy.m_traverseCount += val;
            }
            else
            {
                if (copy.m_traverseCount > val)
                {
                    copy.m_traverseCount -= val;
                }
                else
                {
                    copy.m_traverseCount = val - copy.m_traverseCount;
                    copy.m_reverse = false;
                }
            }

            if (val > copy.m_currentPos)
            {
                copy.m_currentPos = Size - (val - copy.m_currentPos);
            }
            else
            {
                copy.m_currentPos = copy.m_currentPos - val;
            }

            return copy;
        }

        iterator operator++(int)
        {
            iterator retval = *this;

            ++(*this);

            return retval;
        }

        iterator& operator--()
        {
            if (m_currentPos == 0)
            {
                m_currentPos = Size - 1;
            }
            else
            {
                --m_currentPos;
            }

            if (m_traverseCount == 0)
            {
                m_reverse = true;
                m_traverseCount++;
            }
            else
            {
                if (m_reverse)
                {
                    m_traverseCount++;
                }
                else
                {
                    m_traverseCount--;
                }
            }

            return *this;
        }

        iterator operator--(int)
        {
            iterator retval = *this;

            --(*this);

            return retval;
        }

        // todo: optimize this
        iterator operator+(size_type value) const
        {
            iterator retval = *this;

            return retval + value;
        }

        // todo: optimize this
        iterator operator-(size_type value) const
        {
            iterator retval = *this;

            return retval - value;
        }

        bool operator==(iterator other) const
        {
            auto equality =
                   m_buffer == other.m_buffer &&
                   m_currentPos == other.m_currentPos;

            if (equality)
            {
                return m_traverseCount == other.m_traverseCount;
            }
            else
            {
                return false;
            }
        }

        bool operator!=(iterator other) const
        {
            return !(*this == other);
        }

        reference operator*()
        {
            return *(m_buffer + m_currentPos);
        }

        const_reference operator*() const
        {
            return *(m_buffer + m_currentPos);
        }

    private:
        value_type* m_buffer;
        size_type m_currentPos;
        size_type m_traverseCount;
        bool m_reverse;
    };

    using const_iterator = const iterator;

    using reverse_iterator = std::reverse_iterator<iterator>;

    using const_reverse_iterator = const std::reverse_iterator<iterator>;

    /**
     * @brief Default constructor.
     */
    ringbuffer() :
        m_buffer(),
        m_length(0),
        m_insertPosition(0),
        m_beginPosition(0)
    {

    }

    /**
     * @brief Fill constructor. Creates ringbuffer,
     * filled with val values.
     * @todo Make `val` param copyable to allow std::move usage from above.
     * @param n Number of elements (and capacity)
     * @param val Fill element value.
     * @param alloc Allocator.
     */
    explicit ringbuffer(size_type n,
                        const value_type& val = value_type()) :
        m_buffer(),
        m_length(n),
        m_insertPosition(0),
        m_beginPosition(0)
    {
        auto* pointer = m_buffer;

        for (int i = 0; i < n; ++i)
        {
            (*pointer++) = val;
        }
    }

    /**
     * @brief Range constructor. Creates ringbuffer with
     * size of elements between iterators and fills it with
     * elements.
     * @tparam InputIterator Iterator type.
     * @param first Begin iterator.
     * @param last End iterator.
     * @param alloc Allocator.
     */
    template<typename InputIterator, typename = std::_RequireInputIter<InputIterator>>
     ringbuffer(InputIterator first,
                InputIterator last) :
        m_buffer(),
        m_length(static_cast<size_type>(std::distance(first, last))),
        m_insertPosition(0),
        m_beginPosition(0)
    {
        for (auto* pointer = m_buffer; first != last; ++first)
        {
            (*pointer++) = *first;
        }
    }

    ringbuffer& operator=(ringbuffer&& x) noexcept
    {
        m_length = std::move(x.m_length);
        m_insertPosition = std::move(x.m_insertPosition);
        m_beginPosition = std::move(x.m_beginPosition);

        auto* pointer = m_buffer;

        for (auto&& val : x.m_buffer)
        {
            (*pointer++) = std::move(val);
        }

        x.m_length = 0;
        x.m_insertPosition = 0;
        x.m_beginPosition = 0;
    }

    /**
     * @brief Move constructor.
     * @param x Rhs.
     */
    ringbuffer(ringbuffer&& x) noexcept :
        m_buffer(),
        m_length(std::move(x.m_length)),
        m_insertPosition(std::move(x.m_insertPosition)),
        m_beginPosition(std::move(x.m_beginPosition))
    {
        auto* pointer = m_buffer;
        for (auto&& val : x.m_buffer)
        {
            (*pointer++) = std::move(val);
        }

        x.m_length = 0;
        x.m_insertPosition = 0;
        x.m_beginPosition = 0;
    }

    /**
     * @brief Initializer list constructor.
     * @param list Initializer list.
     * @param alloc Allocator.
     */
    ringbuffer(std::initializer_list<value_type> list) :
        m_buffer(),
        m_length(static_cast<size_type>(std::distance(list.begin(), list.end()))),
        m_insertPosition(0),
        m_beginPosition(0)
    {
        auto* pointer = m_buffer;
        for (auto&& el : list)
        {
            (*pointer++) = el;
        }
    }

    /**
     * @brief Method that returns iterator to beginning
     * @return Returns an iterator pointing
     * to the first element in the ring buffer.
     */
    iterator begin()
    {
        return iterator(m_buffer,
                        m_beginPosition,
                        0);
    }

    /**
     * @brief Method that returns const iterator to beginning
     * @return Returns a const iterator pointing
     * to the first element in the ring buffer.
     */
    const_iterator begin() const
    {
        return cbegin();
    }

    /**
     * @brief Method that returns const iterator to beginning
     * @return Returns a const iterator pointing
     * to the first element in the ring buffer.
     */
    const_iterator cbegin() const
    {
        return const_iterator(const_cast<T*>(m_buffer),
                              m_beginPosition,
                              0);
    }

    /**
     * @brief Method that returns iterator to end
     * @return Returns an iterator pointing
     * to the end of the ring buffer.
     */
    iterator end()
    {
        if (Size == 0)
        {
            return iterator(nullptr, 0, 0);
        }

        return iterator(m_buffer,
                        m_insertPosition % Size,
                        m_length);
    }

    /**
     * @brief Method that returns const iterator to end
     * @return Returns a const iterator pointing
     * to the end of the ring buffer.
     */
    const_iterator end() const
    {
        return cend();
    }

    /**
     * @brief Method that returns const iterator to end
     * @return Returns a const iterator pointing
     * to the end of the ring buffer.
     */
    const_iterator cend() const
    {
        return const_iterator(const_cast<T*>(m_buffer),
                              m_insertPosition,
                              m_length);
    }

    /**
     * @brief Method that returns reverse iterator
     * to begin.
     * @return Returns a reverse iterator that's pointing
     * to the end element.
     */
    reverse_iterator rbegin()
    {
        return std::reverse_iterator<iterator>(end());
    }

    /**
     * @brief Method that returns reverse iterator
     * to begin.
     * @return Returns a reverse iterator that's pointing
     * to the last element.
     */
    const_reverse_iterator rbegin() const
    {
        return crbegin();
    }

    /**
     * @brief Method that returns constant reverse iterator.
     * @return Returns const reverse iterator that's pointing to
     * the last element.
     */
    const_reverse_iterator crbegin() const
    {
        return std::reverse_iterator<const_iterator>(end());
    }

    /**
     * @brief Method that returns reversed iterator.
     * @return Returns reverse iterator that's pointing to
     * the first element.
     */
    reverse_iterator rend()
    {
        return std::reverse_iterator<iterator>(begin());
    }

    /**
     * @brief Method that returns const reversed iterator.
     * @return Returns const reverse iterator that's pointing to
     * the first element.
     */
    const_reverse_iterator rend() const
    {
        return crend();
    }

    /**
     * @brief Method that returns const reversed iterator.
     * @return Returns const reverse iterator that's pointing to
     * the first element.
     */
    const_reverse_iterator crend() const
    {
        return std::reverse_iterator<iterator>(begin());
    }

    /**
     * @brief Method for getting number of elements.
     * @return Number of elements.
     */
    size_type size() const
    {
        return m_length;
    }

    /**
     * @brief Return maximum size.
     * @return Returns the maximum number of elements that the vector can hold.
     */
    size_type max_size() const
    {
        return Size;
    }

    bool empty() const
    {
        return m_length == 0;
    }

    reference front()
    {
        return m_buffer[m_beginPosition];
    }

    const_reference front() const
    {
        return m_buffer[m_beginPosition];
    }

    reference back()
    {
        return m_buffer[dec_index(m_insertPosition)];
    }

    const_reference back() const
    {
        return m_buffer[dec_index(m_insertPosition)];
    }

    reference operator[](size_type n)
    {
        return m_buffer[inc_index(m_beginPosition, n)];
    }

    const_reference operator[](size_type n) const
    {
        return m_buffer[inc_index(m_beginPosition, n)];
    }

    reference at(size_type n)
    {
        if (n >= m_length)
        {
            throw std::out_of_range("Index is out of range.");
        }

        return (*this)[n];
    }

    const_reference at(size_type n) const
    {
        if (n >= m_length)
        {
            throw std::out_of_range("Index is out of range.");
        }

        return (*this)[n];
    }

    /**
     * @brief Method for pushing back element.
     * If not enough space left, elements will be overwritten.
     * @param value Value.
     */
    void push_back(const value_type& value)
    {
        m_buffer[m_insertPosition] = value;

        m_insertPosition = inc_index(m_insertPosition);

        if (m_length < Size)
        {
            m_length++;
        }
    }

    /**
     * @brief Method for popping element from back.
     */
    void pop_back()
    {
        if (empty())
        {
            throw std::overflow_error("There is no elements.");
        }

        m_insertPosition = dec_index(m_insertPosition);

        --m_length;
    }

    template<typename... Args>
    void emplace_back(Args&&... args)
    {
        m_buffer[m_insertPosition] = T(args...);

        m_insertPosition = inc_index(m_insertPosition);

        if (m_length < Size)
        {
            m_length++;
        }
    }

    /**
     * @brief Method for popping element from front.
     */
    void pop_front()
    {
        if (empty())
        {
            throw std::overflow_error("There is no elements.");
        }

//        m_allocator.destroy(&m_buffer[m_beginPosition]);

        m_beginPosition = inc_index(m_beginPosition);
        --m_length;
    }

    /**
     * @brief Method for popping several elements from front.
     */
    void pop_front(size_type count)
    {
        if (m_length < count)
        {
            throw std::overflow_error("Not enough elements.");
        }

        m_beginPosition = inc_index(m_beginPosition, count);
        m_length -= count;
    }

    /**
     * @brief Method for clearing
     * container.
     */
    void clear()
    {
        // Destroying objects

        m_beginPosition = 0;
        m_insertPosition = 0;
        m_length = 0;
    }

    iterator erase(const_iterator position)
    {
        if (position.m_currentPos == 0)
        {
            pop_front();
            return begin();
        }

        if (position.m_currentPos % m_length == m_length - 1)
        {
            pop_back();
            return end();
        }

        // Erasing from middle
        m_buffer[position.m_currentPos] = value_type();

        for (auto iterator = position,
                 nextIterator = position + 1,
                 endIterator = end();
             nextIterator != endIterator;
             ++iterator, ++nextIterator)
        {
            m_buffer[iterator.m_currentPos] = m_buffer[nextIterator.m_currentPos];
        }

        --m_length;
        m_insertPosition = dec_index(m_insertPosition);

        return begin() + position.m_traverseCount;
    }

private:

    size_type inc_index(const size_type& index, const size_type& n = 1) const
    {
        return (index + n) % Size;
    }

    size_type dec_index(const size_type& index, const size_type& n = 1) const
    {
        if (n > index)
        {
            return Size - (n - index);
        }
        else
        {
            return index - n;
        }
    }

    value_type m_buffer[Size];
    size_type m_length;
    size_type m_insertPosition;
    size_type m_beginPosition;
};

