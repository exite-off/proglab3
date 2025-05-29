#include "mystruct.h"
#include <stdexcept>

MyStruct::SecondLevelBlock::SecondLevelBlock(size_type cap) : capacity(cap) {
    data = new value_type[capacity];
}

MyStruct::SecondLevelBlock::~SecondLevelBlock() {
    delete[] data;
    capacity = 0;
}

MyStruct::FirstLevelBlock::FirstLevelBlock(size_type second_level_capacity) {
    for (size_type i = 0; i < second_level_blocks_count; ++i) {
        blocks[i] = new SecondLevelBlock(second_level_capacity);
    }
}

MyStruct::FirstLevelBlock::~FirstLevelBlock() {
    for (size_type i = 0; i < second_level_blocks_count; ++i) {
        delete blocks[i];
    }
}

MyStruct::MyStruct(size_type second_level_cap) : first_level_blocks(new FirstLevelBlock*[1]),
    first_level_count(1), second_level_capacity(second_level_cap), total_size(0) {
    first_level_blocks[0] = new FirstLevelBlock(second_level_capacity);
}

MyStruct::MyStruct(std::initializer_list<value_type> init) : MyStruct() {
    for (const value_type& val : init) {
        push_back(val);
    }
}

MyStruct::MyStruct(const MyStruct& other) : MyStruct(other.second_level_capacity) {
    *this = other;
}

MyStruct::MyStruct(MyStruct&& other) noexcept
    : first_level_blocks(other.first_level_blocks),
      first_level_count(other.first_level_count),
      second_level_capacity(other.second_level_capacity),
      total_size(other.total_size) {
    other.first_level_blocks = nullptr;
    other.first_level_count = 0;
    other.total_size = 0;
}

MyStruct& MyStruct::operator=(const MyStruct& other) {
    if (this != &other) {
        for (size_type i = 0; i < first_level_count; ++i) {
            delete first_level_blocks[i];
        }
        delete[] first_level_blocks;

        first_level_count = other.first_level_count;
        second_level_capacity = other.second_level_capacity;
        total_size = other.total_size;
        first_level_blocks = new FirstLevelBlock*[first_level_count];
        for (size_type i = 0; i < first_level_count; ++i) {
            first_level_blocks[i] = new FirstLevelBlock(second_level_capacity);
            for (size_type j = 0; j < second_level_blocks_count; ++j) {
                for (size_type k = 0; k < second_level_capacity; ++k) {
                    first_level_blocks[i]->blocks[j]->data[k] = other.first_level_blocks[i]->blocks[j]->data[k];
                }
            }
        }
    }
    return *this;
}

MyStruct& MyStruct::operator=(MyStruct&& other) noexcept {
    if (this != &other) {
        for (size_type i = 0; i < first_level_count; ++i) {
            delete first_level_blocks[i];
        }
        delete[] first_level_blocks;

        first_level_blocks = other.first_level_blocks;
        first_level_count = other.first_level_count;
        second_level_capacity = other.second_level_capacity;
        total_size = other.total_size;

        other.first_level_blocks = nullptr;
        other.first_level_count = 0;
        other.total_size = 0;
    }
    return *this;
}

MyStruct::~MyStruct() {
    for (size_type i = 0; i < first_level_count; ++i) {
        delete first_level_blocks[i];
    }
    delete[] first_level_blocks;
}

MyStruct::size_type MyStruct::size() const {
    return total_size;
}

MyStruct::size_type MyStruct::capacity() const {
    return first_level_count * second_level_blocks_count * second_level_capacity;
}

bool MyStruct::empty() const {
    return total_size == 0;
}

void MyStruct::shift_right(size_type from) {
    for (size_type i = total_size; i > from; --i) {
        size_type from_index = i - 1;

        size_type lvl1_to = i / (second_level_blocks_count * second_level_capacity);
        size_type lvl2_to = (i / second_level_capacity) % second_level_blocks_count;
        size_type offset_to = i % second_level_capacity;

        size_type lvl1_from = from_index / (second_level_blocks_count * second_level_capacity);
        size_type lvl2_from = (from_index / second_level_capacity) % second_level_blocks_count;
        size_type offset_from = from_index % second_level_capacity;

        first_level_blocks[lvl1_to]->blocks[lvl2_to]->data[offset_to] =
            first_level_blocks[lvl1_from]->blocks[lvl2_from]->data[offset_from];
    }

    ++total_size;
}

void MyStruct::shift_left(size_type from) {
    for (size_type i = from; i < total_size - 1; ++i) {
        size_type from_index = i + 1;

        size_type lvl1_to = i / (second_level_blocks_count * second_level_capacity);
        size_type lvl2_to = (i / second_level_capacity) % second_level_blocks_count;
        size_type offset_to = i % second_level_capacity;

        size_type lvl1_from = from_index / (second_level_blocks_count * second_level_capacity);
        size_type lvl2_from = (from_index / second_level_capacity) % second_level_blocks_count;
        size_type offset_from = from_index % second_level_capacity;

        first_level_blocks[lvl1_to]->blocks[lvl2_to]->data[offset_to] =
            first_level_blocks[lvl1_from]->blocks[lvl2_from]->data[offset_from];
    }

    --total_size;
}

void MyStruct::grow() {
    auto** new_blocks = new FirstLevelBlock*[first_level_count + 1];
    for (size_type i = 0; i < first_level_count; ++i) {
        new_blocks[i] = first_level_blocks[i];
    }

    new_blocks[first_level_count] = new FirstLevelBlock(second_level_capacity);
    ++first_level_count;
    delete[] first_level_blocks;
    first_level_blocks = new_blocks;
}

void MyStruct::push_back(const value_type& value) {
    if (total_size == capacity()) {
        grow();
    }

    size_type index = total_size;
    size_type lvl1 = index / (second_level_blocks_count * second_level_capacity);
    size_type lvl2 = (index / second_level_capacity) % second_level_blocks_count;
    size_type offset = index % second_level_capacity;

    first_level_blocks[lvl1]->blocks[lvl2]->data[offset] = value;
    ++total_size;
}

void MyStruct::push_front(const value_type& value) {
    if (total_size == capacity()) {
        grow();
    }

    shift_right(0);

    first_level_blocks[0]->blocks[0]->data[0] = value;
}

MyStruct::iterator::iterator(MyStruct* s, size_type i) : container(s), index(i) {};

MyStruct::reference MyStruct::iterator::operator*() {
    size_type lvl1 = index / (second_level_blocks_count * container->second_level_capacity);
    size_type lvl2 = (index / container->second_level_capacity) % second_level_blocks_count;
    size_type offset = index % container->second_level_capacity;
    return container->first_level_blocks[lvl1]->blocks[lvl2]->data[offset];
}

MyStruct::iterator& MyStruct::iterator::operator++() {
    ++index;
    return *this;
}

MyStruct::iterator MyStruct::iterator::operator++(int) {
    iterator temp = *this;
    ++index;
    return temp;
}

MyStruct::value_type* MyStruct::iterator::operator->() {
    size_type lvl1 = index / (second_level_blocks_count * container->second_level_capacity);
    size_type lvl2 = (index / container->second_level_capacity) % second_level_blocks_count;
    size_type offset = index % container->second_level_capacity;
    return &container->first_level_blocks[lvl1]->blocks[lvl2]->data[offset];
}

bool MyStruct::iterator::operator!=(const iterator& other) const {
    return index != other.index || container != other.container;
}

MyStruct::size_type MyStruct::iterator::get_index() const {
    return index;
}

MyStruct::const_iterator::const_iterator(const MyStruct* s, size_type i) : container(s), index(i) {}

MyStruct::const_reference MyStruct::const_iterator::operator*() const {
    size_type lvl1 = index / (second_level_blocks_count * container->second_level_capacity);
    size_type lvl2 = (index / container->second_level_capacity) % second_level_blocks_count;
    size_type offset = index % container->second_level_capacity;
    return container->first_level_blocks[lvl1]->blocks[lvl2]->data[offset];
}

MyStruct::const_iterator& MyStruct::const_iterator::operator++() {
    ++index;
    return *this;
}

MyStruct::const_iterator MyStruct::const_iterator::operator++(int) {
    const_iterator temp = *this;
    ++index;
    return temp;
}

const MyStruct::value_type* MyStruct::const_iterator::operator->() const {
    size_type lvl1 = index / (second_level_blocks_count * container->second_level_capacity);
    size_type lvl2 = (index / container->second_level_capacity) % second_level_blocks_count;
    size_type offset = index % container->second_level_capacity;
    return &container->first_level_blocks[lvl1]->blocks[lvl2]->data[offset];
}

bool MyStruct::const_iterator::operator!=(const const_iterator& other) const {
    return index != other.index || container != other.container;
}

MyStruct::size_type MyStruct::const_iterator::get_index() const {
    return index;
}

void MyStruct::clear() {
    total_size = 0;
    shrink_to_fit();
}

void MyStruct::shrink_to_fit() {
    if (total_size == 0) {
        for (size_type i = 0; i < first_level_count; ++i) {
            delete first_level_blocks[i];
        }
        delete[] first_level_blocks;
        first_level_blocks = new FirstLevelBlock*[1];
        first_level_blocks[0] = new FirstLevelBlock(second_level_capacity);
        first_level_count = 1;
    }
    else if (total_size <= (first_level_count - 1) * second_level_blocks_count * second_level_capacity) {
        size_type first_level_block_capacity = second_level_blocks_count * second_level_capacity;
        size_type new_first_count = total_size % (first_level_block_capacity) == 0 ?
            total_size / (first_level_block_capacity) : total_size / (first_level_block_capacity) + 1;

        auto** new_blocks = new FirstLevelBlock*[new_first_count];
        for (size_type i = 0; i < new_first_count; ++i) {
            new_blocks[i] = first_level_blocks[i];
        }
        for (size_type i = new_first_count; i < first_level_count; ++i) {
            delete first_level_blocks[i];
        }
        delete[] first_level_blocks;
        first_level_blocks = new_blocks;
        first_level_count = new_first_count;
    }
}

MyStruct::reference MyStruct::front() {
    if (empty()) {
        throw std::out_of_range("MyStruct is empty");
    }
    return first_level_blocks[0]->blocks[0]->data[0];
}

MyStruct::const_reference MyStruct::front() const {
    if (empty()) {
        throw std::out_of_range("MyStruct is empty");
    }
    return first_level_blocks[0]->blocks[0]->data[0];
}

MyStruct::reference MyStruct::back() {
    if (empty()) {
        throw std::out_of_range("MyStruct is empty");
    }
    size_type index = total_size - 1;
    size_type lvl1 = index / (second_level_blocks_count * second_level_capacity);
    size_type lvl2 = (index / second_level_capacity) % second_level_blocks_count;
    size_type offset = index % second_level_capacity;
    return first_level_blocks[lvl1]->blocks[lvl2]->data[offset];
}

MyStruct::const_reference MyStruct::back() const {
    if (empty()) {
        throw std::out_of_range("MyStruct is empty");
    }
    size_type index = total_size - 1;
    size_type lvl1 = index / (second_level_blocks_count * second_level_capacity);
    size_type lvl2 = (index / second_level_capacity) % second_level_blocks_count;
    size_type offset = index % second_level_capacity;
    return first_level_blocks[lvl1]->blocks[lvl2]->data[offset];
}

MyStruct::iterator MyStruct::begin() {
    return {this, 0};
}

MyStruct::iterator MyStruct::end() {
    return {this, total_size};
}

MyStruct::const_iterator MyStruct::begin() const {
    return {this, 0};
}

MyStruct::const_iterator MyStruct::end() const {
    return {this, total_size};
}

MyStruct::const_iterator MyStruct::cbegin() const {
    return {this, 0};
}

MyStruct::const_iterator MyStruct::cend() const {
    return {this, total_size};
}

void MyStruct::pop_back() {
    if (empty()) {
        throw std::out_of_range("MyStruct is empty");
    }
    --total_size;
}

void MyStruct::pop_front() {
    if (empty()) {
        throw std::out_of_range("MyStruct is empty");
    }
    shift_left(0);
}

MyStruct::iterator MyStruct::insert(iterator pos, const value_type& value) {
    size_type iter_index = pos.get_index();
    if (total_size == capacity()) {
        grow();
    }

    shift_right(iter_index);

    size_type lvl1 = iter_index / (second_level_blocks_count * second_level_capacity);
    size_type lvl2 = (iter_index / second_level_capacity) % second_level_blocks_count;
    size_type offset = iter_index % second_level_capacity;

    first_level_blocks[lvl1]->blocks[lvl2]->data[offset] = value;

    return {this, iter_index};
}

MyStruct::iterator MyStruct::erase(iterator pos) {
    size_type iter_index = pos.get_index();
    if (iter_index >= total_size) {
        throw std::out_of_range("Iterator out of range");
    }

    shift_left(iter_index);

    return {this, iter_index};
}
