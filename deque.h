//https://github.com/Mogby/deque
#pragma once

#ifndef DEQUE_H
#define DEQUE_H

#include <iterator>

template <typename DataType>
class Deque {

    template<typename ValueType, typename ContainerType>
        class deque_iterator : public std::iterator<std::random_access_iterator_tag, ValueType> {

            typedef deque_iterator<ValueType, ContainerType> iterator;

            private:

            ContainerType *container;
            int offset;

            size_t getIndex() const {
                int index = offset;

                if (index < 0) {
                    index += container->size();
                }

                return index;
            }

            public:

            iterator& operator++() {
                ++offset;

                return *this;
            }

            const iterator operator++(int) {
                iterator result = *this;

                ++offset;

                return result;
            }

            iterator& operator--() {
                --offset;

                return *this;
            }

            const iterator operator--(int) {
                iterator result = *this;

                --offset;

                return result;
            }

            iterator& operator+=(int delta_offset) {
                offset += delta_offset;

                return *this;
            }

            const iterator operator+(int delta_offset) const {
                iterator result = *this;
                result += delta_offset;

                return result;
            }

            iterator& operator-=(int delta_offset) {
                offset -= delta_offset;

                return *this;
            }

            const iterator operator-(int delta_offset) const {
                iterator result = *this;
                result -= delta_offset;

                return result;
            }

            int operator-(const iterator &other) const {
                int result = offset - other.offset;
                return result;
            }

            ValueType& operator*() const {
                return (*container)[getIndex()];
            }

            ValueType& operator[](int delta_offset) const {
                return *(*this + delta_offset);
            }

            ValueType* operator->() const {
                return &((*container)[getIndex()]);
            }

            iterator& operator=(const iterator &other) {
                container = other.container;
                offset = other.offset;

                return *this;
            }

            bool operator==(const iterator &other) const {
                return container == other.container &&
                    offset == other.offset;
            }

            bool operator!=(const iterator &other) const {
                return !(*this == other);
            }

            bool operator<(const iterator &other) const {
                return offset < other.offset;
            }

            bool operator<=(const iterator &other) const {
                return offset <= other.offset;
            }

            bool operator>(const iterator &other) const {
                return offset > other.offset;
            }

            bool operator>=(const iterator &other) const {
                return offset >= other.offset;
            }



            deque_iterator(ContainerType *container, int offset) : 
                container(container), offset(offset) {}

            deque_iterator(const iterator &other) {
                *this = other;
            }

            deque_iterator() {}



            virtual ~deque_iterator() {}

        };

private:

    DataType* vector;

    size_t vector_size;

    size_t vector_capacity;

    size_t begin_offset;

    size_t end_offset;



    void resize_vector(size_t new_capacity);

    inline void adapt_vector(size_t new_size);



    inline size_t move_index(size_t index, int offset) const;

    inline size_t next_index(size_t index) const;

    inline size_t previous_index(size_t index) const;

public:

    typedef deque_iterator<DataType, Deque<DataType>> iterator;

    typedef deque_iterator<const DataType, const Deque<DataType>> const_iterator;

    typedef std::reverse_iterator<iterator> reverse_iterator;

    typedef std::reverse_iterator<const_iterator> const_reverse_iterator;



    void push_back(const DataType& new_element);

    void pop_back();

    void push_front(const DataType& new_element);

    void pop_front();



    DataType& back();

    const DataType& back() const;

    DataType& front();

    const DataType& front() const;



    bool empty() const;

    size_t size() const;



    iterator begin();

    const_iterator begin() const;

    const_iterator cbegin() const;

    iterator end();

    const_iterator end() const;

    const_iterator cend() const;

    reverse_iterator rbegin();

    const_reverse_iterator rbegin() const;

    const_reverse_iterator crbegin() const;

    reverse_iterator rend();

    const_reverse_iterator rend() const;

    const_reverse_iterator crend() const;



    DataType& operator[](size_t index);

    const DataType& operator[](size_t index) const;

    bool operator==(const Deque<DataType> &other) const;



    Deque();

    Deque(const Deque& other);

    virtual ~Deque();

};

template <typename DataType, typename ValueType, typename ContainerType>
typename Deque<DataType>::template deque_iterator<ValueType, ContainerType>
    operator+(int offset, 
    const typename Deque<DataType>::template deque_iterator<ValueType, ContainerType> &iterator) {
    return iterator + offset;
}

template <typename DataType>
void Deque<DataType>::resize_vector(size_t new_capacity) {
    DataType* new_vector = new DataType[new_capacity];

    for (size_t offset = 0; offset < vector_size; ++offset) {
        new_vector[offset] = vector[move_index(begin_offset, offset)];
    }

    delete[] vector;

    vector = new_vector;
    vector_capacity = new_capacity;

    begin_offset = 0;
    end_offset = vector_size;

    return;
}

template <typename DataType>
inline void Deque<DataType>::adapt_vector(size_t new_size) {
    vector_size = new_size;

    if (new_size * 4 < vector_capacity && vector_capacity / 4 >= 4) {
        resize_vector(vector_capacity / 4);
    } else if (new_size * 2 > vector_capacity) {
        resize_vector(vector_capacity * 2);
    }
}

template <typename DataType>
inline size_t Deque<DataType>::move_index(size_t index, int offset) const {
    bool offset_negative = offset < 0;

    if (offset_negative) {
        offset = -offset;
    }

    offset %= vector_capacity;

    if (offset_negative && offset) {
        offset = vector_capacity - offset;
    }

    return (index + offset) % vector_capacity;
}

template <typename DataType>
inline size_t Deque<DataType>::next_index(size_t index) const {
    return move_index(index, 1);
}

template <typename DataType>
inline size_t Deque<DataType>::previous_index(size_t index) const {
    return move_index(index, -1);
}



template <typename DataType>
void Deque<DataType>::push_back(const DataType& new_element) {
    vector[end_offset] = new_element;
    end_offset = next_index(end_offset);

    adapt_vector(vector_size + 1);
}

template <typename DataType>
void Deque<DataType>::pop_back() {
    end_offset = previous_index(end_offset);

    adapt_vector(vector_size - 1);
}

template <typename DataType>
void Deque<DataType>::push_front(const DataType& new_element) {
    begin_offset = previous_index(begin_offset);
    vector[begin_offset] = new_element;

    adapt_vector(vector_size + 1);
}

template <typename DataType>
void Deque<DataType>::pop_front() {
    begin_offset = next_index(begin_offset);

    adapt_vector(vector_size - 1);
}



template <typename DataType>
DataType& Deque<DataType>::back() {
    return vector[previous_index(end_offset)];
}

template <typename DataType>
const DataType& Deque<DataType>::back() const {
    return vector[previous_index(end_offset)];
}

template <typename DataType>
DataType& Deque<DataType>::front() {
    return vector[begin_offset];
}

template <typename DataType>
const DataType& Deque<DataType>::front() const {
    return vector[begin_offset];
}



template <typename DataType>
bool Deque<DataType>::empty() const {
    return !vector_size;
}

template <typename DataType>
size_t Deque<DataType>::size() const {
    return vector_size;
}



template <typename DataType>
typename Deque<DataType>::iterator Deque<DataType>::begin() {
    return typename Deque<DataType>::iterator(this, 0);
}

template <typename DataType>
typename Deque<DataType>::const_iterator Deque<DataType>::begin() const {
    return typename Deque<DataType>::const_iterator(this, 0);
}

template <typename DataType>
typename Deque<DataType>::const_iterator Deque<DataType>::cbegin() const {
    return typename Deque<DataType>::const_iterator(this, 0);
}

template <typename DataType>
typename Deque<DataType>::iterator Deque<DataType>::end() {
    return typename Deque<DataType>::iterator(this, vector_size);
}

template <typename DataType>
typename Deque<DataType>::const_iterator Deque<DataType>::end() const {
    return typename Deque<DataType>::const_iterator(this, vector_size);
}

template <typename DataType>
typename Deque<DataType>::const_iterator Deque<DataType>::cend() const {
    return typename Deque<DataType>::const_iterator(this, vector_size);
}

template <typename DataType>
typename Deque<DataType>::reverse_iterator Deque<DataType>::rbegin() {
    return typename Deque<DataType>::reverse_iterator(end());
}

template <typename DataType>
typename Deque<DataType>::const_reverse_iterator Deque<DataType>::rbegin() const {
    return typename Deque<DataType>::const_reverse_iterator(cend());
}

template <typename DataType>
typename Deque<DataType>::const_reverse_iterator Deque<DataType>::crbegin() const {
    return typename Deque<DataType>::const_reverse_iterator(cend());
}

template <typename DataType>
typename Deque<DataType>::reverse_iterator Deque<DataType>::rend() {
    return typename Deque<DataType>::reverse_iterator(begin());
}

template <typename DataType>
typename Deque<DataType>::const_reverse_iterator Deque<DataType>::rend() const {
    return typename Deque<DataType>::const_reverse_iterator(cbegin());
}

template <typename DataType>
typename Deque<DataType>::const_reverse_iterator Deque<DataType>::crend() const {
    return typename Deque<DataType>::const_reverse_iterator(cbegin());
}


template <typename DataType>
DataType& Deque<DataType>::operator[](size_t index) {
    return vector[move_index(begin_offset, index)];
}

template <typename DataType>
const DataType& Deque<DataType>::operator[](size_t index) const {
    return vector[move_index(begin_offset, index)];
}



template <typename DataType>
Deque<DataType>::Deque() {
    vector = new DataType[4];

    vector_capacity = 4;

    vector_size = 0;

    begin_offset = end_offset = 0;
}

template <typename DataType>
Deque<DataType>::Deque(const Deque& other) {
    vector = new DataType[other.vector_capacity];

    vector_capacity = other.vector_capacity;

    vector_size = other.vector_size;

    for (size_t offset = 0; offset < other.size(); ++offset) {
        vector[offset] = other[offset];
    }

    begin_offset = 0;
    end_offset = vector_size;
}

template <typename DataType>
Deque<DataType>::~Deque() {
    delete[] vector;
}

#endif
