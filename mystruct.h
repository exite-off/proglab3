// Made by Vitalii Fedin (group K-16, variant 9)

#pragma once

#include <cstddef>
#include <initializer_list>

class MyStruct {
public:
    using value_type = long long;
    using reference = value_type&;
    using const_reference = const value_type&;
    using size_type = std::size_t;

private:
    size_type first_level_count;
    size_type second_level_capacity;
    size_type total_size;
    static constexpr size_type second_level_blocks_count = 4;

    struct SecondLevelBlock {
        value_type* data;
        size_type capacity;

        SecondLevelBlock(size_type cap);
        ~SecondLevelBlock();
    };

    struct FirstLevelBlock {
        SecondLevelBlock* blocks[second_level_blocks_count];

        FirstLevelBlock(size_type second_level_capacity);
        ~FirstLevelBlock();
    };

    FirstLevelBlock** first_level_blocks;

    void shift_right(size_type from);
    void shift_left(size_type from);
    void grow();

public:
    MyStruct(size_type second_level_cap = 8);
    MyStruct(std::initializer_list<value_type> init);
    MyStruct(const MyStruct& other);
    MyStruct(MyStruct&& other) noexcept;
    MyStruct& operator=(const MyStruct& other);
    MyStruct& operator=(MyStruct&& other) noexcept;
    ~MyStruct();

    class iterator {
    public:
        iterator(MyStruct* s, size_type i);
        reference operator*();
        iterator& operator++();
        iterator operator++(int);
        value_type* operator->();
        bool operator!=(const iterator& other) const;
        size_type get_index() const;

    private:
        MyStruct* container;
        size_type index;
    };
    class const_iterator {
    public:
        const_iterator(const MyStruct* s, size_type i);
        const_reference operator*() const;
        const_iterator& operator++();
        const_iterator operator++(int);
        const value_type* operator->() const;
        bool operator!=(const const_iterator& other) const;
        size_type get_index() const;

    private:
        const MyStruct* container;
        size_type index;
    };

    size_type size() const;
    size_type capacity() const;
    bool empty() const;

    iterator begin();
    iterator end();
    const_iterator begin() const;
    const_iterator end() const;
    const_iterator cbegin() const;
    const_iterator cend() const;

    void push_back(const value_type& value);
    void push_front(const value_type& value);
    void pop_back();
    void pop_front();
    reference front();
    reference back();
    const_reference front() const;
    const_reference back() const;
    iterator insert(iterator pos, const value_type& value);
    iterator erase(iterator pos);
    void clear();
    void shrink_to_fit();
};
