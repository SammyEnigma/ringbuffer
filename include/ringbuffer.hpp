//
// Created by megaxela on 11/23/17.
//

#pragma once

#include <cstdlib>
#include <memory>
#include <limits>
#include <sstream>
#include <iostream>
#include <iomanip>

template< typename T, typename Allocator = std::allocator< T > >
class ringbuffer
{
public:

    using value_type = T;

    using reference = T&;

    using const_reference = const T&;

    using size_type = std::size_t;

    using allocator_type = Allocator;

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
        template<typename, typename>
        friend class ringbuffer;

    public:
        explicit iterator(T* val,
                          size_type begin,
                          size_type capacity,
                          size_type fromBegin) :
            m_buffer(val),
            m_currentPos(begin),
            m_capacity(capacity),
            m_traverseCount(fromBegin),
            m_reverse(false)
        {

        }

        iterator& operator++()
        {
            m_currentPos = (m_currentPos + 1) % m_capacity;

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


            copy.m_currentPos = (copy.m_currentPos + val) % m_capacity;

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
                copy.m_currentPos = m_capacity - (val - copy.m_currentPos);
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
                m_currentPos = m_capacity - 1;
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

        iterator& operator--(int)
        {
            iterator retval = *this;

            --(*this);

            return retval;
        }

        // todo: optimize this
        iterator operator+(size_type value) const
        {
            iterator retval = *this;

            for (size_type i = 0; i < value; ++i)
            {
                ++retval;
            }

            return retval;
        }

        // todo: optimize this
        iterator operator-(size_type value) const
        {
            iterator retval = *this;

            for (size_type i = 0; i < value; ++i)
            {
                --retval;
            }

            return retval;
        }

        bool operator==(iterator other) const
        {
            auto equality =
                   m_buffer == other.m_buffer &&
                   m_currentPos == other.m_currentPos &&
                   m_capacity == other.m_capacity;

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
        size_type m_capacity;
        size_type m_traverseCount;
        bool m_reverse;
    };

    using const_iterator = const iterator;

    using reverse_iterator = std::reverse_iterator<iterator>;

    using const_reverse_iterator = const std::reverse_iterator<iterator>;

    /**
     * @brief Default constructor.
     * Does not allocate any heap memory.
     * @param alloc Allocator.
     */
    explicit ringbuffer(const allocator_type &alloc = allocator_type()) :
        m_allocator(alloc),
        m_buffer(nullptr),
        m_capacity(0),
        m_length(0),
        m_insertPosition(0),
        m_beginPosition(0)
    {

    }

    /**
     * @brief Fill constructor. Creates ringbuffer,
     * filled with val values.
     * @param n Number of elements (and capacity)
     * @param val Fill element value.
     * @param alloc Allocator.
     */
    explicit ringbuffer(size_type n,
                        const value_type& val = value_type(),
                        const allocator_type& alloc = allocator_type()) :
        m_allocator(alloc),
        m_buffer(m_allocator.allocate(n)),
        m_capacity(n),
        m_length(n),
        m_insertPosition(0),
        m_beginPosition(0)
    {
        for (size_type i = 0; i < n; ++i)
        {
            m_allocator.construct(&m_buffer[i], val);
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
               InputIterator last,
               const allocator_type& alloc = allocator_type()) :
        m_allocator(alloc),
        m_buffer(m_allocator.allocate(std::distance(first, last))),
        m_capacity(static_cast<size_type>(std::distance(first, last))),
        m_length(m_capacity),
        m_insertPosition(0),
        m_beginPosition(0)
    {
        for (size_type i = 0; first != last; ++first, ++i)
        {
            m_allocator.construct(&m_buffer[i], *first);
//            m_buffer[i] = *first;
        }
    }

    /**
     * @brief Move constructor.
     * @param x Rhs.
     */
    ringbuffer(ringbuffer&& x) noexcept :
        m_allocator(std::move(x.m_allocator)),
        m_buffer(std::move(x.m_buffer)),
        m_capacity(std::move(x.m_capacity)),
        m_length(std::move(x.m_length)),
        m_insertPosition(std::move(x.m_insertPosition)),
        m_beginPosition(std::move(x.m_beginPosition))
    {
        x.m_buffer = nullptr;
        x.m_capacity = 0;
        x.m_length = 0;
        x.m_insertPosition = 0;
        x.m_beginPosition = 0;
    }

    /**
     * @brief Initializer list constructor.
     * @param list Initializer list.
     * @param alloc Allocator.
     */
    ringbuffer(std::initializer_list<value_type> list,
               const allocator_type& alloc = allocator_type()) :
        m_allocator(alloc),
        m_buffer(m_allocator.allocate(list.size())),
        m_capacity(list.size()),
        m_length(m_capacity),
        m_insertPosition(0),
        m_beginPosition(0)
    {
        auto buffer = m_buffer;
        for (auto&& el : list)
        {
            m_allocator.construct(buffer, el);
            ++buffer;
        }
    }

    /**
     * @brief Copy constructor.
     * @param ringbuffer Right hand one.
     */
    ringbuffer(const ringbuffer& ringbuffer) :
        m_allocator(ringbuffer.m_allocator),
        m_buffer(m_allocator.allocate(ringbuffer.capacity())),
        m_capacity(ringbuffer.m_capacity),
        m_length(ringbuffer.m_length),
        m_insertPosition(ringbuffer.m_insertPosition),
        m_beginPosition(ringbuffer.m_beginPosition)
    {
        for (size_type i = 0; i < ringbuffer.size(); ++i)
        {
            size_type rounded_i = inc_index(m_beginPosition, i);

            m_allocator.construct(&m_buffer[rounded_i], ringbuffer.m_buffer[rounded_i]);
        }
    }

    /**
     * @brief Destructor.
     */
    ~ringbuffer()
    {
        if (m_buffer)
        {
            // Destroying elements
            for (size_type i = 0; i < size(); ++i)
            {
                m_allocator.destroy(&m_buffer[m_beginPosition + i]);
            }

            // Deallocating buffer
            m_allocator.deallocate(m_buffer, m_capacity);
        }
    }

    /**
     * @brief Copy operator.
     * @param rhs Right hand one.
     * @return Reference to this ring buffer.
     */
    ringbuffer& operator=(const ringbuffer& rhs)
    {
        if (m_buffer)
        {
            // Destroying elements
            for (size_type i = 0; i < size(); ++i)
            {
                m_allocator.destroy(&m_buffer[m_beginPosition + i]);
            }

            // Deallocating buffer
            m_allocator.deallocate(m_buffer, m_capacity);
        }

        m_allocator = rhs.m_allocator;
        m_buffer = m_allocator.allocate(rhs.m_capacity);
        m_capacity = rhs.m_capacity;
        m_length = rhs.m_length;
        m_insertPosition = rhs.m_insertPosition;
        m_beginPosition = rhs.m_beginPosition;

        for (size_type i = 0; i < rhs.size(); ++i)
        {
            size_type rounded_i = inc_index(m_beginPosition, i);

            m_allocator.construct(&m_buffer[rounded_i], rhs.m_buffer[rounded_i]);
        }

        return (*this);
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
                        m_capacity,
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
        return const_iterator(m_buffer,
                              m_beginPosition,
                              m_capacity,
                              0);
    }

    /**
     * @brief Method that returns iterator to end
     * @return Returns an iterator pointing
     * to the end of the ring buffer.
     */
    iterator end()
    {
        if (m_capacity == 0)
        {
            return iterator(nullptr, 0, 0, 0);
        }

        return iterator(m_buffer,
                        m_insertPosition % m_capacity,
                        m_capacity,
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
        return const_iterator(m_buffer,
                              m_insertPosition,
                              m_capacity,
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
        return std::numeric_limits<size_type>::max() / sizeof(T);
    }

    bool empty() const
    {
        return m_length == 0;
    }

    /**
     * @brief Method for resising content in buffer.
     * If n is bigger than capacity value, then
     * reallocation will happen.
     * @param n New size.
     * @param val Replace value.
     */
    void resize(size_type n, value_type val = value_type())
    {
        if (n > m_capacity)
        {
            auto newBuffer = m_allocator.allocate(n);

            size_type i = 0;
            for (auto&& el : *this)
            {
                m_allocator.construct(&newBuffer[i++], el);
            }

            // Deleting elements
            if (m_buffer)
            {
                // Destroying elements
                for (size_type j = 0; j < size(); ++j)
                {
                    m_allocator.destroy(&m_buffer[m_beginPosition + j]);
                }

                // Deallocating buffer
                m_allocator.deallocate(m_buffer, m_capacity);
            }

            m_buffer = newBuffer;
            m_capacity = n;
            m_insertPosition = m_length;
            m_beginPosition = 0;
        }

        for (size_type i = m_length; i < n; ++i)
        {
            m_allocator.construct(&m_buffer[m_insertPosition], val);
            m_insertPosition = inc_index(m_insertPosition);
        }

        m_length = n;
    }

    size_type capacity() const
    {
        return m_capacity;
    }

    void reserve(size_type n)
    {
        auto newBuffer = m_allocator.allocate(n);

        size_type i = 0;
        for (auto&& el : *this)
        {
            m_allocator.construct(&newBuffer[i++], el);
        }

        // Deleting elements
        if (m_buffer)
        {
            // Destroying elements
            for (size_type j = 0; j < size(); ++j)
            {
                m_allocator.destroy(&m_buffer[m_beginPosition + j]);
            }

            // Deallocating buffer
            m_allocator.deallocate(m_buffer, m_capacity);
        }

        m_buffer = newBuffer;
        m_capacity = n;
        m_beginPosition = 0;
        m_insertPosition = 0;
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
     * @brief Range assign method.
     * Modifying capacity accordingly.
     * @tparam InputIterator Input iterator type.
     * @param first First iterator.
     * @param last Last iterator.
     */
    template<class InputIterator, typename = std::_RequireInputIter<InputIterator>>
    void assign(InputIterator first, InputIterator last)
    {
        auto requiredElements = static_cast<size_type>(std::distance(first, last));

        // Reallocation
        if (requiredElements > m_capacity)
        {
            auto newBuffer = m_allocator.allocate(requiredElements);

            // Deleting elements
            if (m_buffer)
            {
                // Destroying elements
                for (size_type j = 0; j < size(); ++j)
                {
                    m_allocator.destroy(&m_buffer[m_beginPosition + j]);
                }

                // Deallocating buffer
                m_allocator.deallocate(m_buffer, m_capacity);
            }

            m_buffer = newBuffer;
            m_capacity = static_cast<size_type>(requiredElements);
        }
        else
        {
            // Destroying elements
            for (size_type j = 0; j < size(); ++j)
            {
                m_allocator.destroy(&m_buffer[m_beginPosition + j]);
            }
        }


        for (size_type i = 0;
             first != last;
             ++first, ++i)
        {
            m_allocator.construct(&m_buffer[i], *first);
        }

        m_length = requiredElements;
        m_insertPosition = inc_index(0, requiredElements);
    }

    /**
     * @brief Fill assign method.
     * @param n Number of elements.
     * @param val Value to fill container with.
     */
    void assign(size_type n, const value_type& val)
    {
        // Reallocation
        if (n > m_capacity)
        {
            auto newBuffer = m_allocator.allocate(n);

            // Deleting elements
            if (m_buffer)
            {
                // Destroying elements
                for (size_type j = 0; j < size(); ++j)
                {
                    m_allocator.destroy(&m_buffer[m_beginPosition + j]);
                }

                // Deallocating buffer
                m_allocator.deallocate(m_buffer, m_capacity);
            }

            m_buffer = newBuffer;
            m_capacity = static_cast<size_type>(n);
        }
        else
        {
            // Destroying elements
            for (size_type j = 0; j < size(); ++j)
            {
                m_allocator.destroy(&m_buffer[m_beginPosition + j]);
            }
        }


        for (size_type i = 0;
             i < n;
             ++i)
        {
            m_allocator.construct(&m_buffer[i], val);
        }

        m_length = n;
        m_insertPosition = inc_index(0, n);
    }

    /**
     * @brief Initializer assign method.
     * @param initializer_list Initializer list.
     */
    void assign(std::initializer_list<value_type> initializer_list)
    {
        // Reallocation
        if (initializer_list.size() > m_capacity)
        {
            auto newBuffer = m_allocator.allocate(initializer_list.size());

            // Deleting elements
            if (m_buffer)
            {
                // Destroying elements
                for (size_type j = 0; j < size(); ++j)
                {
                    m_allocator.destroy(&m_buffer[m_beginPosition + j]);
                }

                // Deallocating buffer
                m_allocator.deallocate(m_buffer, m_capacity);
            }

            m_buffer = newBuffer;
            m_capacity = static_cast<size_type>(initializer_list.size());
        }
        else
        {
            // Destroying elements
            for (size_type j = 0; j < size(); ++j)
            {
                m_allocator.destroy(&m_buffer[m_beginPosition + j]);
            }
        }


        std::size_t i = 0;
        for (auto&& val : initializer_list)
        {
            m_allocator.construct(&m_buffer[i++], val);
        }

        m_length = initializer_list.size();
        m_insertPosition = inc_index(0, initializer_list.size());
    }

    /**
     * @brief Method for pushing back element.
     * If not enough space left, std::overflow_error
     * exception will be thrown.
     * @param value Value.
     */
    void push_back(const value_type& value)
    {
        if (m_length == m_capacity)
        {
            throw std::overflow_error("No space left in buffer.");
        }

        m_allocator.construct(&m_buffer[m_insertPosition], value);

        m_insertPosition = inc_index(m_insertPosition);
        m_length++;
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

        m_allocator.destroy(&m_buffer[m_insertPosition]);
        --m_length;
    }

    template<typename... Args>
    void emplace_back(Args&&... args)
    {
        if (m_length == m_capacity)
        {
            throw std::overflow_error("No space left in buffer.");
        }

        m_allocator.construct(&m_buffer[m_insertPosition], args...);

        m_insertPosition = inc_index(m_insertPosition);
        m_length++;
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

        m_allocator.destroy(&m_buffer[m_beginPosition]);

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

        for (size_type i = 0; i < count; ++i)
        {
            m_allocator.destroy(&m_buffer[inc_index(m_beginPosition, i)]);
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
        for (size_type i = 0; i < m_length; ++i)
        {
            m_allocator.destroy(&m_buffer[inc_index(m_beginPosition, i)]);
        }

        m_beginPosition = 0;
        m_insertPosition = 0;
        m_length = 0;
    }

    /**
     * @brief Observer for allocator.
     * @return Allocator.
     */
    allocator_type get_allocator() const noexcept
    {
        return m_allocator;
    }

    /**
     * @brief Method for removing one element.
     * @param position Element position.
     * @return
     */
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
        m_allocator.destroy(&m_buffer[position.m_currentPos]);

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
        return (index + n) % m_capacity;
    }

    size_type dec_index(const size_type& index, const size_type& n = 1) const
    {
        if (n > index)
        {
            return m_capacity - (n - index);
        }
        else
        {
            return index - n;
        }
    }

    allocator_type m_allocator;

    value_type* m_buffer;
    size_type m_capacity;
    size_type m_length;
    size_type m_insertPosition;
    size_type m_beginPosition;
};

